/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.array-user.conf,v 5.18.2.2 2004/02/09 18:21:47 rstory Exp $
 *
 * $Id$
 *
 * Yes, there is lots of code here that you might not use. But it is much
 * easier to remove code than to add it!
 */
#ifndef SAHPIAUTOINSERTTIMEOUTTABLE_H
#define SAHPIAUTOINSERTTIMEOUTTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>

#include <hpiSubagent.h>

        /** Index saHpiDomainId is external */

typedef struct saHpiAutoInsertTimeoutTable_context_s {
    netsnmp_index index; /** THIS MUST BE FIRST!!! */

    /*************************************************************
     * You can store data internally in this structure.
     *
     * TODO: You will probably have to fix a few types here...
     */
    /** TODO: add storage for external index(s)! */
        /** SafUnsigned64 = ASN_OPAQUE */
            unsigned char saHpiAutoInsertTimeoutForInsert[SAF_UNSIGNED_64_LEN];
            long saHpiAutoInsertTimeoutForInsert_len;


    /*
     * OR
     *
     * Keep a pointer to your data
     */
    void * data;

    /*
     *add anything else you want here
     */

} saHpiAutoInsertTimeoutTable_context;

/*************************************************************
 * function declarations
 */
void init_saHpiAutoInsertTimeoutTable(void);
void initialize_table_saHpiAutoInsertTimeoutTable(void);
const saHpiAutoInsertTimeoutTable_context * saHpiAutoInsertTimeoutTable_get_by_idx(netsnmp_index *);
const saHpiAutoInsertTimeoutTable_context * saHpiAutoInsertTimeoutTable_get_by_idx_rs(netsnmp_index *,
                                        int row_status);
int saHpiAutoInsertTimeoutTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

/*************************************************************
 * function declarations: OpenHpi
 */
SaErrorT populate_saHpiAutoInsertTimeoutTable(SaHpiSessionIdT sessionid);

/*************************************************************
 * oid declarations
 */ 
extern oid saHpiAutoInsertTimeoutTable_oid[];
extern size_t saHpiAutoInsertTimeoutTable_oid_len;

#define saHpiAutoInsertTimeoutTable_TABLE_OID 1,3,6,1,4,1,18568,2,1,1,2,11,1
    
/*************************************************************
 * column number definitions for table saHpiAutoInsertTimeoutTable
 */
#define AUTO_INSERT_INDEX_NR 1 
#define COLUMN_SAHPIAUTOINSERTTIMEOUTFORINSERT 1
#define saHpiAutoInsertTimeoutTable_COL_MIN 1
#define saHpiAutoInsertTimeoutTable_COL_MAX 1

/* comment out the following line if you don't handle SET-REQUEST for saHpiAutoInsertTimeoutTable */
#define saHpiAutoInsertTimeoutTable_SET_HANDLING

/* comment out the following line if you can't create new rows */
#define saHpiAutoInsertTimeoutTable_ROW_CREATION

/* comment out the following line if you don't want the secondary index */
#define saHpiAutoInsertTimeoutTable_IDX2

/* uncommend the following line if you allow modifications to an
 * active row */
/** define saHpiAutoInsertTimeoutTable_CAN_MODIFY_ACTIVE_ROW */

#ifdef saHpiAutoInsertTimeoutTable_SET_HANDLING

int saHpiAutoInsertTimeoutTable_extract_index( saHpiAutoInsertTimeoutTable_context * ctx, netsnmp_index * hdr );

void saHpiAutoInsertTimeoutTable_set_reserve1( netsnmp_request_group * );
void saHpiAutoInsertTimeoutTable_set_reserve2( netsnmp_request_group * );
void saHpiAutoInsertTimeoutTable_set_action( netsnmp_request_group * );
void saHpiAutoInsertTimeoutTable_set_commit( netsnmp_request_group * );
void saHpiAutoInsertTimeoutTable_set_free( netsnmp_request_group * );
void saHpiAutoInsertTimeoutTable_set_undo( netsnmp_request_group * );

saHpiAutoInsertTimeoutTable_context * saHpiAutoInsertTimeoutTable_duplicate_row( saHpiAutoInsertTimeoutTable_context* );
netsnmp_index * saHpiAutoInsertTimeoutTable_delete_row( saHpiAutoInsertTimeoutTable_context* );

int saHpiAutoInsertTimeoutTable_can_activate(saHpiAutoInsertTimeoutTable_context *undo_ctx,
                      saHpiAutoInsertTimeoutTable_context *row_ctx,
                      netsnmp_request_group * rg);
int saHpiAutoInsertTimeoutTable_can_deactivate(saHpiAutoInsertTimeoutTable_context *undo_ctx,
                        saHpiAutoInsertTimeoutTable_context *row_ctx,
                        netsnmp_request_group * rg);
int saHpiAutoInsertTimeoutTable_can_delete(saHpiAutoInsertTimeoutTable_context *undo_ctx,
                    saHpiAutoInsertTimeoutTable_context *row_ctx,
                    netsnmp_request_group * rg);
    
    
#ifdef saHpiAutoInsertTimeoutTable_ROW_CREATION
saHpiAutoInsertTimeoutTable_context * saHpiAutoInsertTimeoutTable_create_row( netsnmp_index* );
#endif
#endif

#ifdef saHpiAutoInsertTimeoutTable_IDX2
saHpiAutoInsertTimeoutTable_context * saHpiAutoInsertTimeoutTable_get( const char *name, int len );
#endif

#ifdef __cplusplus
};
#endif

#endif /** SAHPIAUTOINSERTTIMEOUTTABLE_H */
