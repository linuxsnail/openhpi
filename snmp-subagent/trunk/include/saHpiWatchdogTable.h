/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.array-user.conf,v 5.18.2.2 2004/02/09 18:21:47 rstory Exp $
 *
 * $Id$
 *
 * Yes, there is lots of code here that you might not use. But it is much
 * easier to remove code than to add it!
 */
#ifndef SAHPIWATCHDOGTABLE_H
#define SAHPIWATCHDOGTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>
#include "SaHpi.h"

        /** Index saHpiDomainId is external */
        /** Index saHpiResourceId is external */
        /** Index saHpiWatchdogNum is internal */

typedef struct saHpiWatchdogTable_context_s {
    netsnmp_index index; /** THIS MUST BE FIRST!!! */

    /*************************************************************
     * You can store data internally in this structure.
     *
     * TODO: You will probably have to fix a few types here...
     */
    /** TODO: add storage for external index(s)! */
        /** SaHpiInstrumentId = ASN_UNSIGNED */
            unsigned long saHpiWatchdogNum;

        /** TruthValue = ASN_INTEGER */
            long saHpiWatchdogLog;

        /** TruthValue = ASN_INTEGER */
            long saHpiWatchdogRunning;

        /** SaHpiWatchdogTimerUse = ASN_INTEGER */
            long saHpiWatchdogTimerUse;

        /** INTEGER = ASN_INTEGER */
            long saHpiWatchdogTimerAction;

        /** SaHpiWatchdogPreTimerAction = ASN_INTEGER */
            long saHpiWatchdogPretimerInterrupt;

        /** UNSIGNED32 = ASN_UNSIGNED */
            unsigned long saHpiWatchdogPreTimeoutInterval;

        /** OCTETSTR = ASN_OCTET_STR */
            unsigned char saHpiWatchdogTimerUseExpFlags[SAHPI_MAX_TEXT_BUFFER_LENGTH];
            long saHpiWatchdogTimerUseExpFlags_len;

        /** UNSIGNED32 = ASN_UNSIGNED */
            unsigned long saHpiWatchdogTimerInitialCount;

        /** UNSIGNED32 = ASN_UNSIGNED */
            unsigned long saHpiWatchdogTimerPresentCount;

        /** TruthValue = ASN_INTEGER */
            long saHpiWatchdogTimerReset;

        /** UNSIGNED32 = ASN_UNSIGNED */
            unsigned long saHpiWatchdogOem;

        /** RowPointer = ASN_OBJECT_ID */
            oid saHpiWatchdogRDR[MAX_OID_LEN];
            long saHpiWatchdogRDR_len;


    /*
     * OR
     *
     * Keep a pointer to your data
     */
    void * data;

    /*
     *add anything else you want here
     */

} saHpiWatchdogTable_context;

/*************************************************************
 * function declarations
 */
void init_saHpiWatchdogTable(void);
void initialize_table_saHpiWatchdogTable(void);
const saHpiWatchdogTable_context * saHpiWatchdogTable_get_by_idx(netsnmp_index *);
const saHpiWatchdogTable_context * saHpiWatchdogTable_get_by_idx_rs(netsnmp_index *,
                                        int row_status);
int saHpiWatchdogTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);


/*************************************************************
 * oid declarations
 */
extern oid saHpiWatchdogTable_oid[];
extern size_t saHpiWatchdogTable_oid_len;

#define saHpiWatchdogTable_TABLE_OID 1,3,6,1,4,1,18568,1,1,1,6,3,10
    
/*************************************************************
 * column number definitions for table saHpiWatchdogTable
 */
#define COLUMN_SAHPIWATCHDOGNUM 1
#define COLUMN_SAHPIWATCHDOGLOG 2
#define COLUMN_SAHPIWATCHDOGRUNNING 3
#define COLUMN_SAHPIWATCHDOGTIMERUSE 4
#define COLUMN_SAHPIWATCHDOGTIMERACTION 5
#define COLUMN_SAHPIWATCHDOGPRETIMERINTERRUPT 6
#define COLUMN_SAHPIWATCHDOGPRETIMEOUTINTERVAL 7
#define COLUMN_SAHPIWATCHDOGTIMERUSEEXPFLAGS 8
#define COLUMN_SAHPIWATCHDOGTIMERINITIALCOUNT 9
#define COLUMN_SAHPIWATCHDOGTIMERPRESENTCOUNT 10
#define COLUMN_SAHPIWATCHDOGTIMERRESET 11
#define COLUMN_SAHPIWATCHDOGOEM 12
#define COLUMN_SAHPIWATCHDOGRDR 13
#define saHpiWatchdogTable_COL_MIN 1
#define saHpiWatchdogTable_COL_MAX 13

/* comment out the following line if you don't handle SET-REQUEST for saHpiWatchdogTable */
#define saHpiWatchdogTable_SET_HANDLING

/* comment out the following line if you can't create new rows */
#define saHpiWatchdogTable_ROW_CREATION

/* comment out the following line if you don't want the secondary index */
#define saHpiWatchdogTable_IDX2

/* uncommend the following line if you allow modifications to an
 * active row */
/** define saHpiWatchdogTable_CAN_MODIFY_ACTIVE_ROW */

#ifdef saHpiWatchdogTable_SET_HANDLING

int saHpiWatchdogTable_extract_index( saHpiWatchdogTable_context * ctx, netsnmp_index * hdr );

void saHpiWatchdogTable_set_reserve1( netsnmp_request_group * );
void saHpiWatchdogTable_set_reserve2( netsnmp_request_group * );
void saHpiWatchdogTable_set_action( netsnmp_request_group * );
void saHpiWatchdogTable_set_commit( netsnmp_request_group * );
void saHpiWatchdogTable_set_free( netsnmp_request_group * );
void saHpiWatchdogTable_set_undo( netsnmp_request_group * );

saHpiWatchdogTable_context * saHpiWatchdogTable_duplicate_row( saHpiWatchdogTable_context* );
netsnmp_index * saHpiWatchdogTable_delete_row( saHpiWatchdogTable_context* );

int saHpiWatchdogTable_can_activate(saHpiWatchdogTable_context *undo_ctx,
                      saHpiWatchdogTable_context *row_ctx,
                      netsnmp_request_group * rg);
int saHpiWatchdogTable_can_deactivate(saHpiWatchdogTable_context *undo_ctx,
                        saHpiWatchdogTable_context *row_ctx,
                        netsnmp_request_group * rg);
int saHpiWatchdogTable_can_delete(saHpiWatchdogTable_context *undo_ctx,
                    saHpiWatchdogTable_context *row_ctx,
                    netsnmp_request_group * rg);
    
    
#ifdef saHpiWatchdogTable_ROW_CREATION
saHpiWatchdogTable_context * saHpiWatchdogTable_create_row( netsnmp_index* );
#endif
#endif

#ifdef saHpiWatchdogTable_IDX2
saHpiWatchdogTable_context * saHpiWatchdogTable_get( const char *name, int len );
#endif

#ifdef __cplusplus
};
#endif

#endif /** SAHPIWATCHDOGTABLE_H */
