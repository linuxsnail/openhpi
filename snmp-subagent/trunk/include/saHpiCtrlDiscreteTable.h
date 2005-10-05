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
#ifndef SAHPICTRLDISCRETETABLE_H
#define SAHPICTRLDISCRETETABLE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>

#include <SaHpi.h>
#include <oh_utils.h>


        /** Index saHpiDomainId is external */
        /** Index saHpiResourceId is external */
        /** Index saHpiResourceIsHistorical is external */
        /** Index saHpiCtrlDiscreteEntryId is internal */

typedef struct saHpiCtrlDiscreteTable_context_s {
    netsnmp_index index; /** THIS MUST BE FIRST!!! */

    /*************************************************************
     * You can store data internally in this structure.
     *
     * TODO: You will probably have to fix a few types here...
     */
    /** TODO: add storage for external index(s)! */
        /** SaHpiEntryId = ASN_UNSIGNED */
            unsigned long saHpiCtrlDiscreteEntryId;

        /** SaHpiInstrumentId = ASN_UNSIGNED */
            unsigned long saHpiCtrlDiscreteNum;

        /** SaHpiCtrlOutputType = ASN_INTEGER */
            long saHpiCtrlDiscreteOutputType;

        /** SaHpiCtrlMode = ASN_INTEGER */
            long saHpiCtrlDiscreteDefaultMode;

        /** SaHpiCtrlMode = ASN_INTEGER */
            long saHpiCtrlDiscreteMode;

        /** TruthValue = ASN_INTEGER */
            long saHpiCtrlDiscreteIsReadOnly;

        /** TruthValue = ASN_INTEGER */
            long saHpiCtrlDiscreteIsWriteOnly;

        /** UNSIGNED32 = ASN_UNSIGNED */
            unsigned long saHpiCtrlDiscreteDefaultState;

        /** UNSIGNED32 = ASN_UNSIGNED */
            unsigned long saHpiCtrlDiscreteState;

        /** UNSIGNED32 = ASN_UNSIGNED */
            unsigned long saHpiCtrlDiscreteOem;

        /** RowPointer = ASN_OBJECT_ID */
            oid saHpiCtrlDiscreteRDR[MAX_OID_LEN];
            long saHpiCtrlDiscreteRDR_len;


    /*
     * OR
     *
     * Keep a pointer to your data
     */
    void * data;

    /*
     *add anything else you want here
     */

} saHpiCtrlDiscreteTable_context;


/*************************************************************
 * set funtions
 */
int set_table_ctrl_discrete_mode (saHpiCtrlDiscreteTable_context *row_ctx);
int set_table_ctrl_discrete_state (saHpiCtrlDiscreteTable_context *row_ctx);


/*************************************************************
 * function declarations
 */
SaErrorT populate_ctrl_discrete(SaHpiSessionIdT sessionid, 
			        SaHpiRdrT *rdr_entry,
			        SaHpiRptEntryT *rpt_entry,
			        oid *full_oid, size_t full_oid_len,
			        oid *child_oid, size_t *child_oid_len);
SaErrorT clear_ctrl_discrete(SaHpiDomainIdT domainId, 
                             SaHpiResourceIdT resourceId);


/*************************************************************
 * function declarations
 */
void init_saHpiCtrlDiscreteTable(void);
void initialize_table_saHpiCtrlDiscreteTable(void);
const saHpiCtrlDiscreteTable_context * saHpiCtrlDiscreteTable_get_by_idx(netsnmp_index *);
const saHpiCtrlDiscreteTable_context * saHpiCtrlDiscreteTable_get_by_idx_rs(netsnmp_index *,
                                        int row_status);
int saHpiCtrlDiscreteTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);


/*************************************************************
 * oid declarations
 */
extern oid saHpiCtrlDiscreteTable_oid[];
extern size_t saHpiCtrlDiscreteTable_oid_len;

#define saHpiCtrlDiscreteTable_TABLE_OID 1,3,6,1,4,1,18568,2,1,1,4,7,4

/* Number of table Indexes */
#define CTRL_DISCRETE_INDEX_NR 4 
#define saHpiCtrlDiscreteDomainId_INDEX 0
#define saHpiCtrlDiscreteResourceEntryId_INDEX 1
#define saHpiCtrlDiscreteResourceIsHistorical_INDEX 2
#define saHpiCtrlDiscreteEntryId_INDEX 3
    
/*************************************************************
 * column number definitions for table saHpiCtrlDiscreteTable
 */
#define COLUMN_SAHPICTRLDISCRETEENTRYID 1
#define COLUMN_SAHPICTRLDISCRETENUM 2
#define COLUMN_SAHPICTRLDISCRETEOUTPUTTYPE 3
#define COLUMN_SAHPICTRLDISCRETEDEFAULTMODE 4
#define COLUMN_SAHPICTRLDISCRETEMODE 5
#define COLUMN_SAHPICTRLDISCRETEISREADONLY 6
#define COLUMN_SAHPICTRLDISCRETEISWRITEONLY 7
#define COLUMN_SAHPICTRLDISCRETEDEFAULTSTATE 8
#define COLUMN_SAHPICTRLDISCRETESTATE 9
#define COLUMN_SAHPICTRLDISCRETEOEM 10
#define COLUMN_SAHPICTRLDISCRETERDR 11
#define saHpiCtrlDiscreteTable_COL_MIN 2
#define saHpiCtrlDiscreteTable_COL_MAX 11

/* comment out the following line if you don't handle SET-REQUEST for saHpiCtrlDiscreteTable */
#define saHpiCtrlDiscreteTable_SET_HANDLING

/* comment out the following line if you can't create new rows */
#define saHpiCtrlDiscreteTable_ROW_CREATION

/* comment out the following line if you don't want the secondary index */
#define saHpiCtrlDiscreteTable_IDX2

/* uncommend the following line if you allow modifications to an
 * active row */
/** define saHpiCtrlDiscreteTable_CAN_MODIFY_ACTIVE_ROW */

#ifdef saHpiCtrlDiscreteTable_SET_HANDLING

int saHpiCtrlDiscreteTable_extract_index( saHpiCtrlDiscreteTable_context * ctx, netsnmp_index * hdr );

void saHpiCtrlDiscreteTable_set_reserve1( netsnmp_request_group * );
void saHpiCtrlDiscreteTable_set_reserve2( netsnmp_request_group * );
void saHpiCtrlDiscreteTable_set_action( netsnmp_request_group * );
void saHpiCtrlDiscreteTable_set_commit( netsnmp_request_group * );
void saHpiCtrlDiscreteTable_set_free( netsnmp_request_group * );
void saHpiCtrlDiscreteTable_set_undo( netsnmp_request_group * );

saHpiCtrlDiscreteTable_context * saHpiCtrlDiscreteTable_duplicate_row( saHpiCtrlDiscreteTable_context* );
netsnmp_index * saHpiCtrlDiscreteTable_delete_row( saHpiCtrlDiscreteTable_context* );

int saHpiCtrlDiscreteTable_can_activate(saHpiCtrlDiscreteTable_context *undo_ctx,
                      saHpiCtrlDiscreteTable_context *row_ctx,
                      netsnmp_request_group * rg);
int saHpiCtrlDiscreteTable_can_deactivate(saHpiCtrlDiscreteTable_context *undo_ctx,
                        saHpiCtrlDiscreteTable_context *row_ctx,
                        netsnmp_request_group * rg);
int saHpiCtrlDiscreteTable_can_delete(saHpiCtrlDiscreteTable_context *undo_ctx,
                    saHpiCtrlDiscreteTable_context *row_ctx,
                    netsnmp_request_group * rg);
    
    
#ifdef saHpiCtrlDiscreteTable_ROW_CREATION
saHpiCtrlDiscreteTable_context * saHpiCtrlDiscreteTable_create_row( netsnmp_index* );
#endif
#endif

#ifdef saHpiCtrlDiscreteTable_IDX2
saHpiCtrlDiscreteTable_context * saHpiCtrlDiscreteTable_get( const char *name, int len );
#endif

#ifdef __cplusplus
};
#endif

#endif /** SAHPICTRLDISCRETETABLE_H */
