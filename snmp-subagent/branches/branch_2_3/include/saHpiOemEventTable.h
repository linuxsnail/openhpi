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
#ifndef SAHPIOemEVENTTABLE_H
#define SAHPIOemEVENTTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>

#include <hpiSubagent.h>
        /** Index saHpiDomainId is external */
        /** Index saHpiResourceId is external */
        /** Index saHpiEventSeverity is external */
        /** Index saHpiOemEventEntryId is internal */

typedef struct saHpiOemEventTable_context_s {
    netsnmp_index index; /** THIS MUST BE FIRST!!! */

    /*************************************************************
     * You can store data internally in this structure.
     *
     * TODO: You will probably have to fix a few types here...
     */
    /** TODO: add storage for external index(s)! */
        /** SaHpiEntryId = ASN_UNSIGNED */
            unsigned long saHpiOemEventEntryId;

        /** SaHpiTime = ASN_OCTET_STR */
            unsigned char saHpiOemEventTimestamp[SAF_UNSIGNED_64_LEN];
            long saHpiOemEventTimestamp_len;

        /** SaHpiManufacturerId = ASN_UNSIGNED */
            unsigned long saHpiOemEventManufacturerIdT;

        /** SaHpiTextType = ASN_INTEGER */
            long saHpiOemEventTextType;

        /** SaHpiTextLanguage = ASN_INTEGER */
            long saHpiOemEventTextLanguage;

        /** SaHpiText = ASN_OCTET_STR */
            unsigned char saHpiOemEventText[SAHPI_MAX_TEXT_BUFFER_LENGTH];
            long saHpiOemEventText_len;


    /*
     * OR
     *
     * Keep a pointer to your data
     */
    void * data;

    /*
     *add anything else you want here
     */

} saHpiOemEventTable_context;

/*************************************************************
 * function declarations
 */
void init_saHpiOemEventTable(void);
void initialize_table_saHpiOemEventTable(void);
const saHpiOemEventTable_context * saHpiOemEventTable_get_by_idx(netsnmp_index *);
const saHpiOemEventTable_context * saHpiOemEventTable_get_by_idx_rs(netsnmp_index *,
                                        int row_status);
int saHpiOemEventTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);

/*************************************************************
 * function declarations: OpenHpi
 */
SaErrorT populate_saHpiOemEventTable(SaHpiSessionIdT sessionid,
                                        SaHpiEventT *event,
                                        oid * event_oid, 
                                        size_t *event_oid_len);
SaErrorT async_oem_event_add(SaHpiSessionIdT sessionid, 
                       SaHpiEventT *event,
		       SaHpiRdrT *rdr,
		       SaHpiRptEntryT *rpt_entry,
                       oid * this_child_oid, 
                       size_t *this_child_oid_len);


/*************************************************************
 * oid declarations
 */
extern oid saHpiOemEventTable_oid[];
extern size_t saHpiOemEventTable_oid_len;

#define saHpiOemEventTable_TABLE_OID 1,3,6,1,4,1,18568,2,1,1,3,1,27
    
/*************************************************************
 * column number definitions for table saHpiOemEventTable
 */
 
#define Oem_EVENT_INDEX_NR 4
#define COLUMN_SAHPIOemEVENTENTRYID 1
#define COLUMN_SAHPIOemEVENTTIMESTAMP 2
#define COLUMN_SAHPIOemEVENTMANUFACTURERIDT 3
#define COLUMN_SAHPIOemEVENTTEXTTYPE 4
#define COLUMN_SAHPIOemEVENTTEXTLANGUAGE 5
#define COLUMN_SAHPIOemEVENTTEXT 6
#define saHpiOemEventTable_COL_MIN 2
#define saHpiOemEventTable_COL_MAX 6

/* uncommend the following line if you allow modifications to an
 * active row */
/** define saHpiOemEventTable_CAN_MODIFY_ACTIVE_ROW */

int saHpiOemEventTable_extract_index( saHpiOemEventTable_context * ctx, netsnmp_index * hdr );

void saHpiOemEventTable_set_reserve1( netsnmp_request_group * );
void saHpiOemEventTable_set_reserve2( netsnmp_request_group * );
void saHpiOemEventTable_set_action( netsnmp_request_group * );
void saHpiOemEventTable_set_commit( netsnmp_request_group * );
void saHpiOemEventTable_set_free( netsnmp_request_group * );
void saHpiOemEventTable_set_undo( netsnmp_request_group * );

saHpiOemEventTable_context * saHpiOemEventTable_duplicate_row( saHpiOemEventTable_context* );
netsnmp_index * saHpiOemEventTable_delete_row( saHpiOemEventTable_context* );

int saHpiOemEventTable_can_activate(saHpiOemEventTable_context *undo_ctx,
                      saHpiOemEventTable_context *row_ctx,
                      netsnmp_request_group * rg);
int saHpiOemEventTable_can_deactivate(saHpiOemEventTable_context *undo_ctx,
                        saHpiOemEventTable_context *row_ctx,
                        netsnmp_request_group * rg);
int saHpiOemEventTable_can_delete(saHpiOemEventTable_context *undo_ctx,
                    saHpiOemEventTable_context *row_ctx,
                    netsnmp_request_group * rg);
    
    
saHpiOemEventTable_context * saHpiOemEventTable_create_row( netsnmp_index* );

saHpiOemEventTable_context * saHpiOemEventTable_get( const char *name, int len );

#ifdef __cplusplus
};
#endif

#endif /** SAHPIOemEVENTTABLE_H */
