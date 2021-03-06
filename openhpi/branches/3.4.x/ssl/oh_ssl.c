/*
 * Copyright (C) 2007-2008, Hewlett-Packard Development Company, LLP
 *		       All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Hewlett-Packard Corporation, nor the names
 * of its contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author(s)
 *     Bryan Sutula <Bryan.Sutula@hp.com>
 *     Shuah Khan <shuah.khan@hp.com>
 *     Richard White <richard.white@hp.com>
 *
 *
 * This file implements OpenSSL initialization and connection management
 * functionality such as open, close, send, and receive.
 *
 * The initialization is done here and called by the OpenHPI infrastructure,
 * so that individual modules and plugins don't have to worry about any
 * global OpenSSL initialization.  Some of this initialization can only be
 * done once, so this is the best place for it.
 *
 * OpenSSL is a complex library, and the code to reliably communicate over
 * an SSL channel includes a number of subtle factors.  Rather than having
 * each plug-in duplicate the effort to get this right, it's easier to
 * centralize SSL communication support in this library.
 *
 * The following functions are provided:
 *
 * oh_ssl_init()		- Intializes the OpenSSL library (called by the
 * 				  OpenHPI infrastructure only)
 * oh_ssl_ctx_init()            - Creates a new SSL_CTX object
 * oh_ssl_ctx_free()            - Free an SSL_CTX object
 * oh_ssl_connect()             - Create and open a new ssl conection
 * oh_ssl_disconnect()          - Close and free an SSL connection
 * oh_ssl_read()                - Read from an SSL connection
 * oh_ssl_write()               - Write to an SSL connection
 */


/* OpenSSL and other header files */
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/engine.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <glib.h>
#include <oh_ssl.h>
#include <oh_error.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

/* Data types used by this module */
struct CRYPTO_dynlock_value {
	GMutex	*mutex;
};


/* Global (static) data for this module */
static int	oh_ssl_init_done = 0;	/* Will be set true when done */
static GMutex **mutexes = NULL;		/* Holds array of SSL mutexes */
static GStaticMutex ssl_mutexes = G_STATIC_MUTEX_INIT; /* Lock for above */


/* Local (static) functions, used by this module.  Note that these aren't
 * necessary if we aren't compiling as a threaded implementation, so we
 * skip them in that case.
 */


/**
 * id_function
 *
 * SSL thread ID function
 *
 * Return value: a unique thread identifier, as an unsigned long
 **/
static unsigned long id_function(void)
{
	return((unsigned long) g_thread_self());
}


/**
 * lock_function
 * @mode:	Includes the CRYPTO_LOCK bit if a lock is intended.  Otherwise,
 *		an unlock is desired.
 * @type:	Ordinal number of the mutex being manipulated
 * @file:	(unused)
 * @line:	(unused)
 *
 * SSL mutex lock and unlock function.  This is complicated, somewhat,
 * because we're trying to defer allocation of memory and mutexes until
 * they're actually needed.
 *
 * Note that OpenSSL defines that this function has no error return.  In the
 * case where we can't allocate memory, we'll just have to return, pretending
 * that we did the lock.  This will be a silent failure.  The alternative
 * would be to allocate the array of mutex pointers during thread_setup().
 *
 * Return value: (none)
 **/
static void lock_function(int mode, int type, const char * file, int line)
{
	/* Do we have an array of mutex pointers yet? */
	if (! mutexes) {
		/* Messing with this requires the static lock */
		g_static_mutex_lock(&ssl_mutexes);
		if (! mutexes) {	/* Need to check again */
			mutexes = (GMutex **)g_malloc0(CRYPTO_num_locks() *
						       sizeof(GMutex *));
			if (! mutexes) {
				CRIT("out of memory");
				g_static_mutex_unlock(&ssl_mutexes);
				return;
			}
		}
		g_static_mutex_unlock(&ssl_mutexes);
	}

	/* Have we initialized this particular mutex? */
	if (! mutexes[type]) {
		/* Same firedrill as above */
		g_static_mutex_lock(&ssl_mutexes);
		if (! mutexes[type]) {
			mutexes[type] = g_mutex_new();
		}
		g_static_mutex_unlock(&ssl_mutexes);
	}

	/* Finally, go ahead and lock or unlock it */
	if (mode & CRYPTO_LOCK) {
		g_mutex_lock(mutexes[type]);
	}
	else {
		g_mutex_unlock(mutexes[type]);
	}
}


/**
 * dyn_create_function
 * @file:	(unused)
 * @line:	(unused)
 *
 * Function to create and initialize dynlock mutexes
 *
 * Return value: pointer to dynlock structure, or NULL on failure (out of mem)
 **/
static struct CRYPTO_dynlock_value *dyn_create_function(const char *file,
							int line)
{
	struct CRYPTO_dynlock_value *value;

	if ((value = (struct CRYPTO_dynlock_value *)
			g_malloc(sizeof(struct CRYPTO_dynlock_value)))) {
		value->mutex = g_mutex_new();
	}
	else {
		CRIT("out of memory");
	}

	return(value);
}


/**
 * dyn_lock_function
 * @mode:	Includes the CRYPTO_LOCK bit if a lock is intended.  Otherwise,
 *		an unlock is desired.
 * @l:		Pointer to dynlock structure returned by dyn_create_function()
 * @file:	(unused)
 * @line:	(unused)
 *
 * Function to lock and unlock dynlock mutexes
 *
 * Return value: (none)
 **/
static void dyn_lock_function(int mode, struct CRYPTO_dynlock_value *l,
			      const char *file, int line)
{
	if (mode & CRYPTO_LOCK) {
		g_mutex_lock(l->mutex);
	}
	else {
		g_mutex_unlock(l->mutex);
	}
}


/**
 * dyn_destroy_function
 * @l:		Pointer to dynlock structure returned by dyn_create_function()
 * @file:	(unused)
 * @line:	(unused)
 *
 * Function to destroy dynlock mutexes
 *
 * Return value: (none)
 **/
static void	dyn_destroy_function(struct CRYPTO_dynlock_value *l,
				     const char *file, int line)
{
	g_mutex_free(l->mutex);
	g_free(l);
}


/**
 * thread_setup
 *
 * Set up multi-thread protection used by the SSL library
 *
 * Return value: 0 for success, -1 for failure
 **/
static int	thread_setup(void)
{
	/* Register our locking functions with the SSL library */
	CRYPTO_set_id_callback(id_function);
	CRYPTO_set_locking_callback(lock_function);
	CRYPTO_set_dynlock_create_callback(dyn_create_function);
	CRYPTO_set_dynlock_lock_callback(dyn_lock_function);
	CRYPTO_set_dynlock_destroy_callback(dyn_destroy_function);

	return(0);			/* No errors */
}


/**
 * thread_cleanup
 *
 * Clean up multi-thread protection used by the SSL library.
 *
 * Note that this function is not currently used because there is no shutdown
 * code for plugins.  It is left here in case that happens in the future.
 *
 * Return value: 0 for success, -1 for failure (though it currently can't fail)
 **/
static int	thread_cleanup(void)
{
	int		i;

	/* Nullify the locking functions we registered with the SSL library */
	CRYPTO_set_id_callback(NULL);
	CRYPTO_set_locking_callback(NULL);
	CRYPTO_set_dynlock_create_callback(NULL);
	CRYPTO_set_dynlock_lock_callback(NULL);
	CRYPTO_set_dynlock_destroy_callback(NULL);

	/* Clean up and destroy mutexes, if we have any */
	g_static_mutex_lock(&ssl_mutexes);
	if (mutexes) {
		for (i = 0; i < CRYPTO_num_locks(); i++) {
			if (mutexes[i]) {
				g_mutex_free(mutexes[i]);
			}
		}
		g_free(mutexes);
		mutexes = NULL;
	}
	g_static_mutex_free(&ssl_mutexes);

	return(0);			/* No errors */
}

/**
 * oh_ssl_init
 *
 * Intialize the OpenSSL library.  Note that the calls used in this routine
 * set up global data and are only to be called once for an SSL-based program.
 * To enforce this while allowing multiple callers (plugins) to initialize
 * the library, we use a static global variable to mark when we've done the
 * initialization.
 *
 * Note that the thread-safe initialization portion requires that
 * g_thread_init() has already been called, so don't call this routine
 * before then.
 *
 * Return value: 0 for success, -1 for failure
 **/
int		oh_ssl_init(void)
{
	if (! oh_ssl_init_done) {	/* Do this only once */
		oh_ssl_init_done = 1;

		/* Load error strings to provide human-readable error
		 * messages
		 */
		SSL_load_error_strings();
		ERR_load_BIO_strings();

		/* Initialize the SSL library */
		if (! SSL_library_init()) {
			CRIT("SSL_library_init() failed");
			return(-1);
		}

#ifndef NO_SSL_RAND_SEED		/* In case this isn't portable */
		/* Actions to seed PRNG */
		RAND_load_file("/dev/urandom", 1024);
#endif

		/* Set up multi-thread protection functions */
		if (thread_setup() ) {
			CRIT("SSL multi-thread protection setup call failed");
			return(-1);
		}

	}

	return(0);			/* Successful return */
}

/**
 * oh_ssl_finit
 *
 * Finalizes the OpenSSL library. The calls used in this routine releases global
 * data created/initialized by the OpenSSL library.
 *
 * Note that it is the responisbility of the caller of this function to make
 * sure that no other threads are making the OpenSSL library calls. The openhpid
 * should close all the threads and call this function from the main (single)
 * thread.
 *
 * Return value: None
 **/
void oh_ssl_finit(void)
{
        /* TODO: Check whether any other SSL library cleanup should be called */
        thread_cleanup();
        ENGINE_cleanup();
        CONF_modules_unload(0);
        ERR_free_strings();
        EVP_cleanup();
        CRYPTO_cleanup_all_ex_data();
        /* The valgrind is showing possible memory leak by
         * SSL_COMP_get_compression_methods() call.
         *
         * Call to SSL_free_comp_methods() may resolve the memory leak.
         * But not able to find this call in the openssl 0.9.8e
         * TODO: Find whether its a real problem or not
         */

}

/**
 * oh_ssl_ctx_init
 *
 * Create a new SSL_CTX object as a framework for TLS/SSL enabled functions.
 * In particular:
 * - Creates a new CTX object with default option values
 * - Sets common compatibility options
 * - Sets the default locations for trusted CA certificates.
 *   SSL_CTX_set_default_verify_paths() is used to add system-wide default
 *   certificate paths to the verify CApath without having to specify a
 *   default location.  The intent is that the distribution's configured
 *   location will be used.
 *
 * Return value: pointer to SSL_CTX or NULL for failure
 **/
SSL_CTX         *oh_ssl_ctx_init()
{
        SSL_CTX         *ctx;

        ctx = SSL_CTX_new(SSLv23_client_method());
        if (ctx == NULL) {
                CRIT("SSL_CTX_new() failed");
                return(NULL);
        }

        SSL_CTX_set_options(ctx, SSL_OP_TLS_ROLLBACK_BUG | SSL_OP_ALL);

        if (! SSL_CTX_set_default_verify_paths(ctx)) {
                CRIT("SSL_CTX_set_default_verify_paths() failed");
                return(NULL);
        }

        return(ctx);
}


/**
 * oh_ssl_ctx_free
 * @ctx:        pointer to SSL_CTX as returned by oh_ssl_ctx_init()
 *
 * Free an SSL_CTX object
 *
 * Return value: 0 for success, -1 for failure
 **/
int             oh_ssl_ctx_free(SSL_CTX *ctx)
{
        if (ctx == NULL) {
                CRIT("unexpected NULL ctx pointer");
                return(-1);
        }

        SSL_CTX_free(ctx);

        return(0);
}


/**
 * oh_ssl_connect
 * @hostname:   Name of target host.  Format:
 *                  "hostname:port" or "IPaddress:port"
 * @ctx:        pointer to SSL_CTX as returned by oh_ssl_ctx_init()
 * @timeout:    maximum number of seconds to wait for a connection to
 *              hostname, or zero to wait forever
 *
 * Create and open a new ssl conection to the specified host.
 *
 * Return value: pointer to BIO, or NULL for failure
 **/
BIO             *oh_ssl_connect(char *hostname, SSL_CTX *ctx, long timeout)
{
        BIO             *bio;
        SSL             *ssl;
        int             err;
        int len, retval = 0;
        int RetVal, socket_desc = 0;
        char *Server = NULL;
        char *Port = NULL;
        struct addrinfo Hints, *AddrInfo = NULL, *ai = NULL;

        memset(&Hints, 0, sizeof(Hints));
        Hints.ai_family = AF_UNSPEC;
        Hints.ai_socktype = SOCK_STREAM;
        len = strlen(hostname);

        if (hostname == NULL) {
                CRIT("NULL hostname in oh_ssl_connect()");
                return(NULL);
        }
        if (ctx == NULL) {
                CRIT("NULL ctx in oh_ssl_connect()");
                return(NULL);
        }
        if (timeout < 0) {
                CRIT("inappropriate timeout in oh_ssl_connect()");
                return(NULL);
        }

        /* Allocate memory to a char pointer "Server" */
        Server = (char *) g_malloc0(sizeof(char) * len);
        if (Server == NULL){
                CRIT("out of memory");
                return NULL;
        }
        memset(Server, 0, len);
        /* hostname contains "Port" along with "IP Address". As, only
         * "IP Address" is needed for some of the below operations, so copy
         * "IP Address" from hostname to "Server".
         */
        strncpy(Server, hostname, (len - 4));

        /* Allocate memory to a char pointer "Port" */
        Port = (char *) g_malloc0(sizeof(char) * 4);
        if (Port == NULL){
                CRIT("out of memory");
                g_free(Server);
                return NULL;
        }
        /* As Port number is needed separately for some of the below
         * operations, so copy port number from hostname to "Port".
         */
        strncpy(Port, hostname + (len - 3), 3);
        
        /* Create socket address structure to prepare client socket */
        RetVal = getaddrinfo(Server, Port, &Hints, &AddrInfo);
        if (RetVal != 0) {
                CRIT("Cannot resolve address [%s] and port [%s],"
                     " error %d: %s",
                       Server, Port, RetVal, gai_strerror(RetVal));
                g_free(Server);
                g_free(Port);
                return NULL;
        }
        
        ai = AddrInfo;
        /* Create a socket point */
        socket_desc = socket(ai->ai_family, ai->ai_socktype,
                                            ai->ai_protocol);
        if (socket_desc == -1) {
                CRIT("Socket failed with error: %s", 
                      strerror(errno));
                g_free(Server);
                g_free(Port);
                freeaddrinfo(AddrInfo);	
                return NULL;
        }

        /* Now connect to target IP Address */
        retval = connect(socket_desc, ai->ai_addr, ai->ai_addrlen);
        if (retval != 0) {
                CRIT("Socket connect failed with error: %s",
                      strerror(errno));
                g_free(Server);
                g_free(Port);
                freeaddrinfo(AddrInfo);	
                close(socket_desc);
                return NULL;
        }

        /* Create new SSL structure for connection */
        ssl = SSL_new(ctx);

        /* Connect ssl object with a socket descriptor */
        SSL_set_fd(ssl, socket_desc);

        /* Initiate SSL connection */
        err = SSL_connect(ssl);
        if (err != 1) {
                CRIT("SSL connection failed");
                g_free(Server);
                g_free(Port);
                freeaddrinfo(AddrInfo);	
                close(socket_desc);
                return (NULL);
        }

        bio = BIO_new(BIO_f_ssl());             /* create an ssl BIO */
        BIO_set_ssl(bio, ssl, BIO_CLOSE);       /* assign the ssl BIO to SSL */

        /* TODO: Do I need to set the client or server mode here?  I don't
         * think so.
         */

        g_free(Server);
        g_free(Port);
        freeaddrinfo(AddrInfo);	
        return(bio);
}


/**
 * oh_ssl_disconnect
 * @bio:        pointer to a BIO as returned by oh_ssl_connect()
 * @shutdown:   Selects a uni-directional or bi-directional SSL shutdown.
 *              See the SSL_shutdown() man page.
 *
 * Close the SSL connection and free the memory associated with it.
 *
 * Return value: 0 for success, -1 for failure
 **/
int             oh_ssl_disconnect(BIO *bio, enum OH_SSL_SHUTDOWN_TYPE shutdown)
{
        SSL             *ssl;
        int             ret, fd;

        if (bio == NULL) {
                CRIT("NULL bio in oh_ssl_disconnect()");
                return(-1);
        }

        /* Shut down the SSL connection.  This may involve a handshake with
         * the server.
         */
        BIO_get_ssl(bio, &ssl);
        if (ssl == NULL) {
                CRIT("BIO_get_ssl() failed");
                return(-1);
        }
        ret = SSL_shutdown(ssl);
        if (ret == -1) {
                CRIT("SSL_shutdown() failed");
                /* Continuing on to free BIO memory */
        }
        else if ((ret == 0) && (shutdown == OH_SSL_BI)) {
                /* Still need stage 2 shutdown (see SSL_shutdown() man page) */
                ret = SSL_shutdown(ssl);
                if (ret == -1) {
                        CRIT("SSL_shutdown() failed");
                        /* Continuing on to free BIO memory */
                }
                else if (ret == 0) {
                        CRIT("stage 2 of SSL_shutdown() failed");
                        /* Continuing on to free BIO memory */
                }
        }
        /* Close the socket */
        fd = SSL_get_fd(ssl);
        if (fd == -1) {
                CRIT("SSL_get_fd() failed");
                return(-1);
        }
        close(fd);

        /* Free the connection */
        BIO_free_all(bio);

        return(0);
}


/**
 * oh_ssl_read
 * @bio:        pointer to a BIO as returned by oh_ssl_connect()
 * @buf:        buffer for the data which is read from the connection
 * @size:       maximum number of bytes to be read into buf
 * @timeout:    maximum number of seconds to wait for input to be available,
 *              or zero to wait forever
 *
 * Read from an existing SSL connection.  The data and number of bytes read
 * are returned.
 *
 * Note that oh_ssl_read() and oh_ssl_write() have some subtle differences
 * in behavior.  While oh_ssl_write() will try to write all the bytes,
 * oh_ssl_read() will return as soon as it has read some data.
 *
 * Return value: (as follows)
 *   >0:        number of bytes read
 *    0:        nothing more to read; remote host closed the connection
 *   -1:        SSL or other error
 *   -2:        Timeout
 **/
int             oh_ssl_read(BIO *bio, char *buf, int size, long timeout)
{
        SSL             *ssl;
        int             bytes = 0;
        fd_set          readfds;
        fd_set          writefds;
        struct          timeval tv;
        int             read_wait;
        int             done;
        int             err;
        int             fd;

        if (bio == NULL) {
                CRIT("NULL bio in oh_ssl_read()");
                return(-1);
        }
        if (buf == NULL) {
                CRIT("NULL buf in oh_ssl_read()");
                return(-1);
        }
        if (size <= 0) {
                CRIT("inappropriate size in oh_ssl_read()");
                return(-1);
        }
        if (timeout < 0) {
                CRIT("inappropriate timeout in oh_ssl_read()");
                return(-1);
        }

        /* Get underlying file descriptor, needed for select call */
        fd = BIO_get_fd(bio, NULL);
        if (fd == -1) {
                CRIT("BIO doesn't seem to be initialized in oh_ssl_read()");
                return(-1);
        }

        /* We also need the SSL connection pointer */
        BIO_get_ssl(bio, &ssl);
        if (ssl == NULL) {
                CRIT("BIO_get_ssl() failed");
                return(-1);
        }

        /* Because of SSL renegotiations, we may have to wait on a socket
         * write even though we're trying to do a read.  The initial value
         * of read_wait indicates that we're trying to read, but it can be
         * set to 0 if we end up waiting for a socket write.
         */
        read_wait = 1;
        done = 0;

        /* We have to loop on the read call, until we get something we
         * can return to the user.
         */
        while (! done) {
                /* First, we need to wait until something happens on the
                 * underlying socket.  We are either waiting for a read
                 * or a write (but not both).
                 */
                FD_ZERO(&readfds);
                FD_ZERO(&writefds);
                if (read_wait) {
                        FD_SET(fd, &readfds);
                }
                else {
                        FD_SET(fd, &writefds);
                }
                if (timeout) {
                        tv.tv_sec = timeout;
                        tv.tv_usec = 0;
                        err = select(fd + 1, &readfds, &writefds, NULL, &tv);
                }
                else {                  /* No timeout */
                        err = select(fd + 1, &readfds, &writefds, NULL, NULL);
                }

                /* Evaluate select() return code */
                if (err < 0) {
                        CRIT("error during select()");
                        return(-1);
                }
                if (err == 0) {
                        return(-2);     /* Timeout */
                }

                /* The socket has something.  Ready to try (or re-try)
                 * the read call.
                 */
                bytes = SSL_read(ssl, buf, size);
                switch (SSL_get_error(ssl, bytes)) {
                        case SSL_ERROR_NONE:
                                /* No error */
                                if (bytes) {
                                        done = 1;
                                }
                                break;
                        case SSL_ERROR_ZERO_RETURN:
                                /* Connection was closed.  For this case,
                                 * since it's normal for the remote host
                                 * to close when it's done, we'll not signal
                                 * any error, but will return zero bytes.
                                 */
                                return(0);
                        case SSL_ERROR_WANT_READ:
                                read_wait = 1;
                                break;
                        case SSL_ERROR_WANT_WRITE:
                                read_wait = 0;
                                break;
                        default:
                                /* Some other sort of error */
                                CRIT("SSL_read reported error%d",SSL_get_error(ssl, bytes));
                                return(-1);
                }
        }

        return(bytes);
}


/**
 * oh_ssl_write
 * @bio:        pointer to a BIO as returned by oh_ssl_connect()
 * @buf:        buffer to write to the connection
 * @size:       number of bytes to be written
 * @timeout:    maximum number of seconds to wait for the remote host to
 *              accept the data, or zero to wait forever
 *
 * Write data to an existing SSL connection.
 *
 * Note that oh_ssl_read() and oh_ssl_write() have some subtle differences
 * in behavior.  While oh_ssl_read() returns as soon as it has data for the
 * caller, oh_ssl_write() does not return until all the bytes have been
 * written to the remote host.
 *
 * Return value: (as follows)
 *    0:        success
 *   -1:        error
 *   -2:        timeout
 **/
int             oh_ssl_write(BIO *bio, char *buf, int size, long timeout)
{
        SSL             *ssl;
        int             bytes;
        fd_set          readfds;
        fd_set          writefds;
        struct          timeval tv;
        int             write_wait;
        int             done;
        int             err;
        int             fd;
        int             sent;

        if (bio == NULL) {
                CRIT("NULL bio in oh_ssl_write()");
                return(-1);
        }
        if (buf == NULL) {
                CRIT("NULL buf in oh_ssl_write()");
                return(-1);
        }
        if (size <= 0) {
                CRIT("inappropriate size in oh_ssl_write()");
                return(-1);
        }
        if (timeout < 0) {
                CRIT("inappropriate timeout in oh_ssl_write()");
                return(-1);
        }

        /* Get underlying file descriptor, needed for select call */
        fd = BIO_get_fd(bio, NULL);
        if (fd == -1) {
                CRIT("BIO doesn't seem to be initialized in oh_ssl_write()");
                return(-1);
        }

        /* We also need the SSL connection pointer */
        BIO_get_ssl(bio, &ssl);
        if (ssl == NULL) {
                CRIT("BIO_get_ssl() failed");
                return(-1);
        }

        /* Because of SSL renegotiations, we may have to wait on a socket
         * read even though we're trying to do a write.  The initial value
         * of write_wait indicates that we're trying to write, but it can
         * be set to 0 if we end up waiting for a socket read.
         */
        write_wait = 1;
        done = 0;
        sent = 0;

        /* We have to loop on the write call, until everything gets written */
        while (! done) {
                /* First, we need to wait until something happens on the
                 * underlying socket.  We are either waiting for a read
                 * or a write (but not both).
                 */
                FD_ZERO(&readfds);
                FD_ZERO(&writefds);
                if (write_wait) {
                        FD_SET(fd, &writefds);
                }
                else {
                        FD_SET(fd, &readfds);
                }
                if (timeout) {
                        tv.tv_sec = timeout;
                        tv.tv_usec = 0;
                        err = select(fd + 1, &readfds, &writefds, NULL, &tv);
                }
                else {                  /* No timeout */
                        err = select(fd + 1, &readfds, &writefds, NULL, NULL);
                }

                /* Evaluate select() return code */
                if (err < 0) {
                        CRIT("error during select()");
                        return(-1);
                }
                if (err == 0) {
                        return(-2);     /* Timeout */
                }

                /* The socket is ready.  Ready to try (or re-try) the write
                 * call.
                 */
                bytes = SSL_write(ssl, buf + sent, size - sent);
                switch (SSL_get_error(ssl, bytes)) {
                        case SSL_ERROR_NONE:
                                /* No error */
                                sent += bytes;
                                if (sent == size) {
                                        done = 1;
                                }
                                break;
                        case SSL_ERROR_ZERO_RETURN:
                                /* Connection was closed.  Since we're trying
                                 * to write, this is an error condition.
                                 */
                                CRIT("remote host unexpectedly closed"
                                    " the connection");
                                return(-1);
                        case SSL_ERROR_WANT_READ:
                                write_wait = 0;
                                break;
                        case SSL_ERROR_WANT_WRITE:
                                write_wait = 1;
                                break;
                        default:
                                /* Some other sort of error */
                                CRIT("error %d from SSL_write", bytes);
                                return(-1);
                }
        }

        return(0);
}
