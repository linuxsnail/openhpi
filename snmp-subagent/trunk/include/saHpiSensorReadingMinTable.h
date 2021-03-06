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
#ifndef SAHPISENSORREADINGMINTABLE_H
#define SAHPISENSORREADINGMINTABLE_H

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

typedef struct saHpiSensorReadingMinTable_context_s {
	netsnmp_index index; /** THIS MUST BE FIRST!!! */

	/*************************************************************
	 * You can store data internally in this structure.
	 *
	 * TODO: You will probably have to fix a few types here...
	 */
	/** TODO: add storage for external index(s)! */
	/** TruthValue = ASN_INTEGER */
	long saHpiSensorReadingMinIsSupported;

	/** SaHpiSensorReadingType = ASN_INTEGER */
	long saHpiSensorReadingMinType;

	/** SaHpiSensorReadingValue = ASN_OCTET_STR */
	unsigned char saHpiSensorReadingMinValue[SAHPI_SENSOR_BUFFER_LENGTH];
	long saHpiSensorReadingMinValue_len;


	/*
	 * OR
	 *
	 * Keep a pointer to your data
	 */
	void * data;

	/*
	 *add anything else you want here
	 */

} saHpiSensorReadingMinTable_context;

/*
 * SaErrorT populate_ctrl_text()
 */
SaErrorT populate_sensor_min(SaHpiSessionIdT sessionid, 
				     SaHpiRdrT *rdr_entry,
				     SaHpiRptEntryT *rpt_entry);
SaErrorT clear_sensor_min(SaHpiDomainIdT domainId, 
                          SaHpiResourceIdT resourceId);

/*************************************************************
 * function declarations
 */
void init_saHpiSensorReadingMinTable(void);
void initialize_table_saHpiSensorReadingMinTable(void);
const saHpiSensorReadingMinTable_context * saHpiSensorReadingMinTable_get_by_idx(netsnmp_index *);
const saHpiSensorReadingMinTable_context * saHpiSensorReadingMinTable_get_by_idx_rs(netsnmp_index *,
										    int row_status);
int saHpiSensorReadingMinTable_get_value(netsnmp_request_info *, netsnmp_index *, netsnmp_table_request_info *);


/*************************************************************
 * oid declarations
 */
	extern oid saHpiSensorReadingMinTable_oid[];
	extern size_t saHpiSensorReadingMinTable_oid_len;

/* Number of table Indexes */
#define SENSOR_READING_MIN_INDEX_NR 4 
#define saHpiSenMinDomainId_INDEX 0
#define saHpiSenMinResourceId_INDEX 1
#define saHpiSenMinResourceIsHistorical_INDEX 2
#define saHpiSensorMinNum_INDEX 3


#define saHpiSensorReadingMinTable_TABLE_OID 1,3,6,1,4,1,18568,2,1,1,4,9,5

/*************************************************************
 * column number definitions for table saHpiSensorReadingMinTable
 */
#define COLUMN_SAHPISENSORREADINGMINISSUPPORTED 1
#define COLUMN_SAHPISENSORREADINGMINTYPE 2
#define COLUMN_SAHPISENSORREADINGMINVALUE 3
#define saHpiSensorReadingMinTable_COL_MIN 1
#define saHpiSensorReadingMinTable_COL_MAX 3

int saHpiSensorReadingMinTable_extract_index( saHpiSensorReadingMinTable_context * ctx, netsnmp_index * hdr );

void saHpiSensorReadingMinTable_set_reserve1( netsnmp_request_group * );
void saHpiSensorReadingMinTable_set_reserve2( netsnmp_request_group * );
void saHpiSensorReadingMinTable_set_action( netsnmp_request_group * );
void saHpiSensorReadingMinTable_set_commit( netsnmp_request_group * );
void saHpiSensorReadingMinTable_set_free( netsnmp_request_group * );
void saHpiSensorReadingMinTable_set_undo( netsnmp_request_group * );

saHpiSensorReadingMinTable_context * saHpiSensorReadingMinTable_duplicate_row( saHpiSensorReadingMinTable_context* );
netsnmp_index * saHpiSensorReadingMinTable_delete_row( saHpiSensorReadingMinTable_context* );

int saHpiSensorReadingMinTable_can_activate(saHpiSensorReadingMinTable_context *undo_ctx,
					    saHpiSensorReadingMinTable_context *row_ctx,
					    netsnmp_request_group * rg);
int saHpiSensorReadingMinTable_can_deactivate(saHpiSensorReadingMinTable_context *undo_ctx,
					      saHpiSensorReadingMinTable_context *row_ctx,
					      netsnmp_request_group * rg);
int saHpiSensorReadingMinTable_can_delete(saHpiSensorReadingMinTable_context *undo_ctx,
					  saHpiSensorReadingMinTable_context *row_ctx,
					  netsnmp_request_group * rg);


saHpiSensorReadingMinTable_context * saHpiSensorReadingMinTable_create_row( netsnmp_index* );

saHpiSensorReadingMinTable_context * saHpiSensorReadingMinTable_get( const char *name, int len );

#ifdef __cplusplus
};
#endif

#endif /** SAHPISENSORREADINGMINTABLE_H */
