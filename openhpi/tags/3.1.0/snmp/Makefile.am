# (C) Copyright IBM Corp 2003, 2004
# All rights reserved.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
# file and program are licensed under a BSD style license.  See
# the Copying file included with the OpenHPI distribution for
# full licensing terms.

MAINTAINERCLEANFILES 	= Makefile.in

AM_CPPFLAGS = -DG_LOG_DOMAIN=\"snmp\"

INCLUDES		= @OPENHPI_INCLUDES@

AM_CFLAGS		= @SNMPFLAGS@

lib_LTLIBRARIES		= libopenhpi_snmp.la

libopenhpi_snmp_la_SOURCES = snmp_utils.c snmp_utils.h

libopenhpi_snmp_la_LDFLAGS = -version-info @HPI_LIB_VERSION@
libopenhpi_snmp_la_LIBADD  = -luuid @SNMPLIBS@
