/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.array-user.conf,v 5.18.2.2 2004/02/09 18:21:47 rstory Exp $
 *
 * $Id$
 *
 * Yes, there is lots of code here that you might not use. But it is much
 * easier to remove code than to add it!
 */
#ifndef SAHPIHOTSWAPEVENTLOGTABLE_H
#define SAHPIHOTSWAPEVENTLOGTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>

        /** Index saHpiDomainId is external */
        /** Index saHpiResourceId is external */
        /** Index saHpiEventSeverity is external */
        /** Index saHpiHotSwapEventEntryId is external */

typedef struct saHpiHotSwapEventLogTable_context_s {
    netsnmp_index index; /** THIS MUST BE FIRST!!! */

    /*************************************************************
     * You can store data internally in this structure.
     *
     * TODO: You will probably have to fix a few types here...
     */
    /** TODO: add storage for external index(s)! */
        /** SaHpiTime = ASN_COUNTER64 */
    /** TODO: Is this type correct? */
            long saHpiHotSwapEventLogTimestamp;

        /** SaHpiHotSwapState = ASN_INTEGER */
            long saHpiHotSwapEventLogState;

        /** SaHpiHotSwapState = ASN_INTEGER */
            long saHpiHotSwapEventLogPreviousState;


    /*
     * OR
     *
     * Keep a pointer to your data
     */
    void * data;

    /*
     *add anything else you want here
     */

} saHpiHotSwapEventLogTable_context;

/*************************************************************
 * function declarations
 */
void init_saHpiHotSwapEventLogTable(void);
void initialize_table_saHpiHotSwapEventLogTable(void);
const saHpiHotSwapEventLogTable_context * saHpiHotSwapEventLogTable_get_by_idx(netsnmp_index *);
const saHpiHotSwapEventLogTable_context * saHpiHotSwapEventLogTable_get_by_idx_rs(netsnmp_index *,
                                        int row_status);
int saHpiHotSwapEventLogTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);


/*************************************************************
 * oid declarations
 */
extern oid saHpiHotSwapEventLogTable_oid[];
extern size_t saHpiHotSwapEventLogTable_oid_len;

#define saHpiHotSwapEventLogTable_TABLE_OID 1,3,6,1,4,1,18568,2,1,1,3,2,17
    
/*************************************************************
 * column number definitions for table saHpiHotSwapEventLogTable
 */
#define COLUMN_SAHPIHOTSWAPEVENTLOGTIMESTAMP 1
#define COLUMN_SAHPIHOTSWAPEVENTLOGSTATE 2
#define COLUMN_SAHPIHOTSWAPEVENTLOGPREVIOUSSTATE 3
#define saHpiHotSwapEventLogTable_COL_MIN 1
#define saHpiHotSwapEventLogTable_COL_MAX 3

/* comment out the following line if you don't handle SET-REQUEST for saHpiHotSwapEventLogTable */
#define saHpiHotSwapEventLogTable_SET_HANDLING

/* comment out the following line if you can't create new rows */
#define saHpiHotSwapEventLogTable_ROW_CREATION

/* comment out the following line if you don't want the secondary index */
#define saHpiHotSwapEventLogTable_IDX2

/* uncommend the following line if you allow modifications to an
 * active row */
/** define saHpiHotSwapEventLogTable_CAN_MODIFY_ACTIVE_ROW */

#ifdef saHpiHotSwapEventLogTable_SET_HANDLING

int saHpiHotSwapEventLogTable_extract_index( saHpiHotSwapEventLogTable_context * ctx, netsnmp_index * hdr );

void saHpiHotSwapEventLogTable_set_reserve1( netsnmp_request_group * );
void saHpiHotSwapEventLogTable_set_reserve2( netsnmp_request_group * );
void saHpiHotSwapEventLogTable_set_action( netsnmp_request_group * );
void saHpiHotSwapEventLogTable_set_commit( netsnmp_request_group * );
void saHpiHotSwapEventLogTable_set_free( netsnmp_request_group * );
void saHpiHotSwapEventLogTable_set_undo( netsnmp_request_group * );

saHpiHotSwapEventLogTable_context * saHpiHotSwapEventLogTable_duplicate_row( saHpiHotSwapEventLogTable_context* );
netsnmp_index * saHpiHotSwapEventLogTable_delete_row( saHpiHotSwapEventLogTable_context* );

int saHpiHotSwapEventLogTable_can_activate(saHpiHotSwapEventLogTable_context *undo_ctx,
                      saHpiHotSwapEventLogTable_context *row_ctx,
                      netsnmp_request_group * rg);
int saHpiHotSwapEventLogTable_can_deactivate(saHpiHotSwapEventLogTable_context *undo_ctx,
                        saHpiHotSwapEventLogTable_context *row_ctx,
                        netsnmp_request_group * rg);
int saHpiHotSwapEventLogTable_can_delete(saHpiHotSwapEventLogTable_context *undo_ctx,
                    saHpiHotSwapEventLogTable_context *row_ctx,
                    netsnmp_request_group * rg);
    
    
#ifdef saHpiHotSwapEventLogTable_ROW_CREATION
saHpiHotSwapEventLogTable_context * saHpiHotSwapEventLogTable_create_row( netsnmp_index* );
#endif
#endif

#ifdef saHpiHotSwapEventLogTable_IDX2
saHpiHotSwapEventLogTable_context * saHpiHotSwapEventLogTable_get( const char *name, int len );
#endif

#ifdef __cplusplus
};
#endif

#endif /** SAHPIHOTSWAPEVENTLOGTABLE_H */
