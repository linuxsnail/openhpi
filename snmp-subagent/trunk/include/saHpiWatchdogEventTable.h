/*
 * (C) Copyright IBM Corp. 2005
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *   David Judkovics  <djudkovi@us.ibm.com>
 *   Daniel de Araujo <ddearauj@us.ibm.com>
 *
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.array-user.conf,v 5.18.2.2 2004/02/09 18:21:47 rstory Exp $
 *
 * $Id$
 *
 * Yes, there is lots of code here that you might not use. But it is much
 * easier to remove code than to add it!
 */
#ifndef SAHPIWATCHDOGEVENTTABLE_H
#define SAHPIWATCHDOGEVENTTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>

        /** Index saHpiDomainId is external */
        /** Index saHpiResourceId is external */
        /** Index saHpiWatchdogNum is external */
        /** Index saHpiEventSeverity is external */
        /** Index saHpiWatchdogEventEntryId is internal */

typedef struct saHpiWatchdogEventTable_context_s {
    netsnmp_index index; /** THIS MUST BE FIRST!!! */

    /*************************************************************
     * You can store data internally in this structure.
     *
     * TODO: You will probably have to fix a few types here...
     */
    /** TODO: add storage for external index(s)! */
        /** SaHpiEntryId = ASN_UNSIGNED */
            unsigned long saHpiWatchdogEventEntryId;

        /** SaHpiTime = ASN_COUNTER64 */
    /** TODO: Is this type correct? */
            struct counter64 saHpiWatchdogEventTimestamp;

        /** INTEGER = ASN_INTEGER */
            long saHpiWatchdogEventAction;

        /** SaHpiWatchdogPreTimerAction = ASN_INTEGER */
            long saHpiWatchdogEventPreTimerAction;

        /** SaHpiWatchdogTimerUse = ASN_INTEGER */
            long saHpiWatchdogEventUse;


    /*
     * OR
     *
     * Keep a pointer to your data
     */
    void * data;

    /*
     *add anything else you want here
     */

} saHpiWatchdogEventTable_context;

/*************************************************************
 * function declarations
 */
void init_saHpiWatchdogEventTable(void);
void initialize_table_saHpiWatchdogEventTable(void);
const saHpiWatchdogEventTable_context * saHpiWatchdogEventTable_get_by_idx(netsnmp_index *);
const saHpiWatchdogEventTable_context * saHpiWatchdogEventTable_get_by_idx_rs(netsnmp_index *,
                                        int row_status);
int saHpiWatchdogEventTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

/*************************************************************
 * function declarations: OpenHpi
 */
SaErrorT populate_saHpiWatchdogEventTable(SaHpiSessionIdT sessionid,
                                        SaHpiEventT *event,
                                        oid * event_oid, 
                                        size_t *event_oid_len);
SaErrorT async_watchdog_event_add(SaHpiSessionIdT sessionid, 
                                  SaHpiEventT *event,
				  SaHpiRdrT *rdr,
				  SaHpiRptEntryT *rpt_entry,
                                  oid * this_child_oid, 
                                  size_t *this_child_oid_len);



/*************************************************************
 * oid declarations
 */
extern oid saHpiWatchdogEventTable_oid[];
extern size_t saHpiWatchdogEventTable_oid_len;

#define saHpiWatchdogEventTable_TABLE_OID 1,3,6,1,4,1,18568,2,1,1,3,1,21
    
/*************************************************************
 * column number definitions for table saHpiWatchdogEventTable
 */
 
#define WATCHDOG_EVENT_INDEX_NR 5
#define COLUMN_SAHPIWATCHDOGEVENTENTRYID 1
#define COLUMN_SAHPIWATCHDOGEVENTTIMESTAMP 2
#define COLUMN_SAHPIWATCHDOGEVENTACTION 3
#define COLUMN_SAHPIWATCHDOGEVENTPRETIMERACTION 4
#define COLUMN_SAHPIWATCHDOGEVENTUSE 5
#define saHpiWatchdogEventTable_COL_MIN 2
#define saHpiWatchdogEventTable_COL_MAX 5

/* comment out the following line if you don't handle SET-REQUEST for saHpiWatchdogEventTable */
#define saHpiWatchdogEventTable_SET_HANDLING

/* comment out the following line if you can't create new rows */
#define saHpiWatchdogEventTable_ROW_CREATION

/* comment out the following line if you don't want the secondary index */
#define saHpiWatchdogEventTable_IDX2

/* uncommend the following line if you allow modifications to an
 * active row */
/** define saHpiWatchdogEventTable_CAN_MODIFY_ACTIVE_ROW */

#ifdef saHpiWatchdogEventTable_SET_HANDLING

int saHpiWatchdogEventTable_extract_index( saHpiWatchdogEventTable_context * ctx, netsnmp_index * hdr );

void saHpiWatchdogEventTable_set_reserve1( netsnmp_request_group * );
void saHpiWatchdogEventTable_set_reserve2( netsnmp_request_group * );
void saHpiWatchdogEventTable_set_action( netsnmp_request_group * );
void saHpiWatchdogEventTable_set_commit( netsnmp_request_group * );
void saHpiWatchdogEventTable_set_free( netsnmp_request_group * );
void saHpiWatchdogEventTable_set_undo( netsnmp_request_group * );

saHpiWatchdogEventTable_context * saHpiWatchdogEventTable_duplicate_row( saHpiWatchdogEventTable_context* );
netsnmp_index * saHpiWatchdogEventTable_delete_row( saHpiWatchdogEventTable_context* );

int saHpiWatchdogEventTable_can_activate(saHpiWatchdogEventTable_context *undo_ctx,
                      saHpiWatchdogEventTable_context *row_ctx,
                      netsnmp_request_group * rg);
int saHpiWatchdogEventTable_can_deactivate(saHpiWatchdogEventTable_context *undo_ctx,
                        saHpiWatchdogEventTable_context *row_ctx,
                        netsnmp_request_group * rg);
int saHpiWatchdogEventTable_can_delete(saHpiWatchdogEventTable_context *undo_ctx,
                    saHpiWatchdogEventTable_context *row_ctx,
                    netsnmp_request_group * rg);
    
    
#ifdef saHpiWatchdogEventTable_ROW_CREATION
saHpiWatchdogEventTable_context * saHpiWatchdogEventTable_create_row( netsnmp_index* );
#endif
#endif

#ifdef saHpiWatchdogEventTable_IDX2
saHpiWatchdogEventTable_context * saHpiWatchdogEventTable_get( const char *name, int len );
#endif

#ifdef __cplusplus
};
#endif

#endif /** SAHPIWATCHDOGEVENTTABLE_H */
