/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.array-user.conf,v 5.18.2.2 2004/02/09 18:21:47 rstory Exp $
 *
 * $Id$
 *
 * Yes, there is lots of code here that you might not use. But it is much
 * easier to remove code than to add it!
 */
#ifndef SAHPISENSORTHDNEGHYSTERESISTABLE_H
#define SAHPISENSORTHDNEGHYSTERESISTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>

        /** Index saHpiDomainId is external */
        /** Index saHpiResourceId is external */
        /** Index saHpiResourceIsHistorical is external */
        /** Index saHpiSensorNum is external */

typedef struct saHpiSensorThdNegHysteresisTable_context_s {
    netsnmp_index index; /** THIS MUST BE FIRST!!! */

    /*************************************************************
     * You can store data internally in this structure.
     *
     * TODO: You will probably have to fix a few types here...
     */
    /** TODO: add storage for external index(s)! */
        /** TruthValue = ASN_INTEGER */
            long saHpiSensorThdNegHysteresisIsReadable;

        /** TruthValue = ASN_INTEGER */
            long saHpiSensorThdNegHysteresisIsWritable;

        /** SaHpiSensorReadingType = ASN_INTEGER */
            long saHpiSensorThdNegHysteresisType;

        /** SaHpiSensorReadingValue = ASN_OCTET_STR */
            unsigned char saHpiSensorThdNegHysteresisValue[65535];
            long saHpiSensorThdNegHysteresisValue_len;

        /** TruthValue = ASN_INTEGER */
            long saHpiSensorThdNegHysteresisNonLinear;


    /*
     * OR
     *
     * Keep a pointer to your data
     */
    void * data;

    /*
     *add anything else you want here
     */

} saHpiSensorThdNegHysteresisTable_context;

/*************************************************************
 * function declarations
 */
void init_saHpiSensorThdNegHysteresisTable(void);
void initialize_table_saHpiSensorThdNegHysteresisTable(void);
const saHpiSensorThdNegHysteresisTable_context * saHpiSensorThdNegHysteresisTable_get_by_idx(netsnmp_index *);
const saHpiSensorThdNegHysteresisTable_context * saHpiSensorThdNegHysteresisTable_get_by_idx_rs(netsnmp_index *,
                                        int row_status);
int saHpiSensorThdNegHysteresisTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);


/*************************************************************
 * oid declarations
 */
extern oid saHpiSensorThdNegHysteresisTable_oid[];
extern size_t saHpiSensorThdNegHysteresisTable_oid_len;

#define saHpiSensorThdNegHysteresisTable_TABLE_OID 1,3,6,1,4,1,18568,2,1,1,4,9,16
    
/*************************************************************
 * column number definitions for table saHpiSensorThdNegHysteresisTable
 */
#define COLUMN_SAHPISENSORTHDNEGHYSTERESISISREADABLE 1
#define COLUMN_SAHPISENSORTHDNEGHYSTERESISISWRITABLE 2
#define COLUMN_SAHPISENSORTHDNEGHYSTERESISTYPE 3
#define COLUMN_SAHPISENSORTHDNEGHYSTERESISVALUE 4
#define COLUMN_SAHPISENSORTHDNEGHYSTERESISNONLINEAR 5
#define saHpiSensorThdNegHysteresisTable_COL_MIN 1
#define saHpiSensorThdNegHysteresisTable_COL_MAX 5

/* comment out the following line if you don't handle SET-REQUEST for saHpiSensorThdNegHysteresisTable */
#define saHpiSensorThdNegHysteresisTable_SET_HANDLING

/* comment out the following line if you can't create new rows */
#define saHpiSensorThdNegHysteresisTable_ROW_CREATION

/* comment out the following line if you don't want the secondary index */
#define saHpiSensorThdNegHysteresisTable_IDX2

/* uncommend the following line if you allow modifications to an
 * active row */
/** define saHpiSensorThdNegHysteresisTable_CAN_MODIFY_ACTIVE_ROW */

#ifdef saHpiSensorThdNegHysteresisTable_SET_HANDLING

int saHpiSensorThdNegHysteresisTable_extract_index( saHpiSensorThdNegHysteresisTable_context * ctx, netsnmp_index * hdr );

void saHpiSensorThdNegHysteresisTable_set_reserve1( netsnmp_request_group * );
void saHpiSensorThdNegHysteresisTable_set_reserve2( netsnmp_request_group * );
void saHpiSensorThdNegHysteresisTable_set_action( netsnmp_request_group * );
void saHpiSensorThdNegHysteresisTable_set_commit( netsnmp_request_group * );
void saHpiSensorThdNegHysteresisTable_set_free( netsnmp_request_group * );
void saHpiSensorThdNegHysteresisTable_set_undo( netsnmp_request_group * );

saHpiSensorThdNegHysteresisTable_context * saHpiSensorThdNegHysteresisTable_duplicate_row( saHpiSensorThdNegHysteresisTable_context* );
netsnmp_index * saHpiSensorThdNegHysteresisTable_delete_row( saHpiSensorThdNegHysteresisTable_context* );

int saHpiSensorThdNegHysteresisTable_can_activate(saHpiSensorThdNegHysteresisTable_context *undo_ctx,
                      saHpiSensorThdNegHysteresisTable_context *row_ctx,
                      netsnmp_request_group * rg);
int saHpiSensorThdNegHysteresisTable_can_deactivate(saHpiSensorThdNegHysteresisTable_context *undo_ctx,
                        saHpiSensorThdNegHysteresisTable_context *row_ctx,
                        netsnmp_request_group * rg);
int saHpiSensorThdNegHysteresisTable_can_delete(saHpiSensorThdNegHysteresisTable_context *undo_ctx,
                    saHpiSensorThdNegHysteresisTable_context *row_ctx,
                    netsnmp_request_group * rg);
    
    
#ifdef saHpiSensorThdNegHysteresisTable_ROW_CREATION
saHpiSensorThdNegHysteresisTable_context * saHpiSensorThdNegHysteresisTable_create_row( netsnmp_index* );
#endif
#endif

#ifdef saHpiSensorThdNegHysteresisTable_IDX2
saHpiSensorThdNegHysteresisTable_context * saHpiSensorThdNegHysteresisTable_get( const char *name, int len );
#endif

#ifdef __cplusplus
};
#endif

#endif /** SAHPISENSORTHDNEGHYSTERESISTABLE_H */
