/*
 * (C) Copyright IBM Corp. 2003
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *   Konrad Rzeszutek <konradr@us.ibm.com>
 *
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.array-user.conf,v 5.15.2.1 2003/02/27 05:59:41 rstory Exp $
 *
 * $Id$
 *
 */
#ifndef SAHPISENSORTABLE_H
#define SAHPISENSORTABLE_H

#ifdef __cplusplus
extern "C"
{
#endif


#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>
#include <SaHpi.h>

  /*
   * Number of index values in this table
   * Consult the HPI-MIB
   *
   * If this number changes, look in the src code for this 
   * define, and make sure to add/remove the new index value(s).
   */
#define SENSOR_INDEX_NR 3
  /* 
   * MAX values from the MIB.
   */
#define SAHPISENSORFACTORS_MAX 10
#define SENSOR_EVENTS_SUPPORTED_MAX 255

  typedef struct saHpiSensorTable_context_s
  {
    netsnmp_index index;

	/** UNSIGNED32 = ASN_UNSIGNED */
    unsigned long saHpiSensorIndex;

	/** INTEGER = ASN_INTEGER */
    long saHpiSensorType;

	/** INTEGER = ASN_INTEGER */
    long saHpiSensorCategory;

	/** INTEGER = ASN_INTEGER */
    long saHpiSensorEventsCategoryControl;

    /** OCTETSTR = ASN_OCTET_STR */
    unsigned char   saHpiSensorEventsSupported[SENSOR_EVENTS_SUPPORTED_MAX];
    size_t            saHpiSensorEventsSupported_len;

	/** UNSIGNED32 = ASN_UNSIGNED */
    unsigned long saHpiSensorStatus;


    /** OCTETSTR = ASN_OCTET_STR */
    unsigned char   saHpiSensorAssertEvents[SENSOR_EVENTS_SUPPORTED_MAX];
    size_t            saHpiSensorAssertEvents_len;

    /** OCTETSTR = ASN_OCTET_STR */
    unsigned char   saHpiSensorDeassertEvents[SENSOR_EVENTS_SUPPORTED_MAX];
    size_t            saHpiSensorDeassertEvents_len;

	/** TruthValue = ASN_INTEGER */
    long saHpiSensorIgnore;

	/** UNSIGNED32 = ASN_UNSIGNED */
    unsigned long saHpiSensorReadingFormats;

	/** TruthValue = ASN_INTEGER */
    long saHpiSensorIsNumeric;

	/** INTEGER = ASN_INTEGER */
    long saHpiSensorSignFormat;

	/** INTEGER = ASN_INTEGER */
    long saHpiSensorBaseUnits;

	/** INTEGER = ASN_INTEGER */
    long saHpiSensorModifierUnits;

	/** INTEGER = ASN_INTEGER */
    long saHpiSensorModifierUse;

	/** TruthValue = ASN_INTEGER */
    long saHpiSensorFactorsStatic;

	/** OCTETSTR = ASN_OCTET_STR */
    unsigned char saHpiSensorFactors[SAHPISENSORFACTORS_MAX];
    long saHpiSensorFactors_len;

	/** INTEGER = ASN_INTEGER */
    long saHpiSensorFactorsLinearization;

	 /** TruthValue = ASN_INTEGER */
    long saHpiSensorPercentage;


	/** UNSIGNED32 = ASN_UNSIGNED */
    unsigned long saHpiSensorOEM;

      /** RowPointer = ASN_OBJECT_ID */
    oid saHpiSensorRDR[MAX_OID_LEN];
    long saHpiSensorRDR_len;

    long rdr_id;
    long resource_id;
    long domain_id;
    long hash;
    int flags;
    SaHpiBoolT is_threshold;

  } saHpiSensorTable_context;

  /*
   * A mapping structure for reading values.
   */
  typedef struct sensor_threshold_to_mib_s
  {
    SaHpiSensorReadingT *reading;
    SaHpiUint8T bit;
    int pos;
  } sensor_threshold_to_mib;

  typedef struct sensor_reading_to_mib_s
  {
    SaHpiSensorReadingT *reading;
    SaHpiSensorRangeFlagsT flag;
    int pos;
  } sensor_reading_to_mib;


/*************************************************************
 * function declarations
 */

  void initialize_table_saHpiSensorTable (void);

  int saHpiSensorTable_get_value (netsnmp_request_info *,
				  netsnmp_index *,
				  netsnmp_table_request_info *);


  int populate_sensor (SaHpiEntryIdT rdr_id,
		       SaHpiSensorRecT * sensor,
		       SaHpiRptEntryT * rpt_entry,
		       oid * rdr_oid, size_t rdr_oid_len,
		       oid * sensor_oid, size_t * sensor_oid_len);

  int
    delete_sensor_row (SaHpiDomainIdT domain_id,
		       SaHpiResourceIdT resource_id, SaHpiSensorNumT num);


  int set_sensor_event (saHpiSensorTable_context * ctx);


/*************************************************************
 * oid declarations
 */
  //   extern oid      saHpiSensorTable_oid[];
  //  extern size_t   saHpiSensorTable_oid_len;

#define saHpiSensorTable_TABLE_OID hpiResources_OID,6

/*************************************************************
 * column number definitions for table saHpiSensorTable
 */
#define COLUMN_SAHPISENSORINDEX 1
#define COLUMN_SAHPISENSORTYPE 2
#define COLUMN_SAHPISENSORCATEGORY 3
#define COLUMN_SAHPISENSOREVENTSCATEGORYCONTROL 4
#define COLUMN_SAHPISENSOREVENTSSUPPORTED 5
#define COLUMN_SAHPISENSORSTATUS 6
#define COLUMN_SAHPISENSORASSERTEVENTS 7
#define COLUMN_SAHPISENSORDEASSERTEVENTS 8
#define COLUMN_SAHPISENSORIGNORE 9
#define COLUMN_SAHPISENSORREADINGFORMATS 10
#define COLUMN_SAHPISENSORISNUMERIC 11
#define COLUMN_SAHPISENSORSIGNFORMAT 12
#define COLUMN_SAHPISENSORBASEUNITS 13
#define COLUMN_SAHPISENSORMODIFIERUNITS 14
#define COLUMN_SAHPISENSORMODIFIERUSE 15
#define COLUMN_SAHPISENSORFACTORSSTATIC 16
#define COLUMN_SAHPISENSORFACTORS 17
#define COLUMN_SAHPISENSORFACTORSLINEARIZATION 18
#define COLUMN_SAHPISENSORPERCENTAGE 19
#define COLUMN_SAHPISENSOROEM 20
#define COLUMN_SAHPISENSORRDR 21
#define saHpiSensorTable_COL_MIN 1
#define saHpiSensorTable_COL_MAX 21



  int
    saHpiSensorTable_extract_index (saHpiSensorTable_context * ctx,
				    netsnmp_index * hdr);

  void saHpiSensorTable_set_reserve1 (netsnmp_request_group *);
  void saHpiSensorTable_set_reserve2 (netsnmp_request_group *);
  void saHpiSensorTable_set_action (netsnmp_request_group *);
  void saHpiSensorTable_set_commit (netsnmp_request_group *);
  void saHpiSensorTable_set_free (netsnmp_request_group *);
  void saHpiSensorTable_set_undo (netsnmp_request_group *);

    saHpiSensorTable_context
    * saHpiSensorTable_duplicate_row (saHpiSensorTable_context *);
  netsnmp_index *saHpiSensorTable_delete_row (saHpiSensorTable_context *);

  int saHpiSensorTable_can_delete (saHpiSensorTable_context
				   * undo_ctx,
				   saHpiSensorTable_context
				   * row_ctx, netsnmp_request_group * rg);



  saHpiSensorTable_context *saHpiSensorTable_create_row (netsnmp_index *);


#ifdef __cplusplus
};
#endif

#endif /** SAHPISENSORTABLE_H */
