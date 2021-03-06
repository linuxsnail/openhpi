/*      -*- linux-c -*-
 *
 * (C) Copyright IBM Corp. 2004
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *     Renier Morales <renierm@users.sf.net>
 */
 
#include <stdlib.h>
#include <string.h>
#include <SaHpi.h>
#include <oHpi.h>

/**
 * Load 'libsimulator', create three handlers, iterate through them
 * making sure they match with the expected values.
 * Destroy handlers, unload plugin.
 * Pass on success, otherwise failure.
 **/
 
int main(int argc, char **argv)
{
        SaHpiSessionIdT sid = 0;
        GHashTable *h0 = g_hash_table_new(g_str_hash, g_str_equal),
                   *h1 = g_hash_table_new(g_str_hash, g_str_equal),
                   *h2 = g_hash_table_new(g_str_hash, g_str_equal);
        oHpiHandlerIdT hid0 = 0, hid1 = 0, hid2 = 0, next_id = 0;
        
        setenv("OPENHPI_CONF","./noconfig", 1);
        
        if (saHpiSessionOpen(SAHPI_UNSPECIFIED_DOMAIN_ID, &sid, NULL))
                return -1;
                
        if (oHpiPluginLoad("libsimulator"))
                return -1;
                
        if (oHpiPluginLoad("libwatchdog"))
                return -1;
                
        /* Set configuration. */
        g_hash_table_insert(h0, "plugin", "libsimulator");
        g_hash_table_insert(h0, "entity_root", "{SYSTEM_CHASSIS,1}");
        g_hash_table_insert(h0, "name", "test");
        g_hash_table_insert(h0, "addr", "0");
        
        g_hash_table_insert(h1, "plugin", "libwatchdog");
        g_hash_table_insert(h1, "entity_root", "{SYSTEM_CHASSIS,2}");
        g_hash_table_insert(h1, "addr", "0");
        
        g_hash_table_insert(h2, "plugin", "libsimulator");
        g_hash_table_insert(h2, "entity_root", "{SYSTEM_CHASSIS,3}");
        g_hash_table_insert(h2, "name", "test");
        g_hash_table_insert(h2, "addr", "0");
        
        if (oHpiHandlerCreate(h0, &hid0) ||
            oHpiHandlerCreate(h1, &hid1) ||
            oHpiHandlerCreate(h2, &hid2))
                return -1;
                
        if (oHpiHandlerGetNext(0, &next_id) || next_id != 1)
                return -1;
                
        if (oHpiHandlerGetNext(1, &next_id) || next_id != 2)
                return -1;
                
        if (oHpiHandlerGetNext(2, &next_id) || next_id != 3)
                return -1;
                
        if (!oHpiHandlerGetNext(3, &next_id))
                return -1;
                
        if (oHpiHandlerDestroy(hid0) ||
            oHpiHandlerDestroy(hid1) ||
            oHpiHandlerDestroy(hid2))
                return -1;
                
        if (oHpiPluginUnload("libwatchdog"))
                return -1;
                
        
        return oHpiPluginUnload("libsimulator");
}
