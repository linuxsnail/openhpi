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
extern          "C" {
#endif


#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>
#include <SaHpi.h>

  /*
   * Hardcoded values in macros.
   */

  

typedef  struct sensor_threshold_to_mib_s {
    SaHpiSensorReadingT* reading;
    SaHpiUint8T bit;
    int pos;
} sensor_threshold_to_mib;

  typedef struct sensor_reading_to_mib_s {
    SaHpiSensorReadingT* reading;
    SaHpiSensorRangeFlagsT flag;
    int pos;
  } sensor_reading_to_mib;
  /* 
   * MAX values from the MIB.
   */
#define SAHPISENSORFACTORS_MAX 10
#define SAHPI_RANGE_VALUES_MAX 5
#define SAHPI_RANGE_RAW_MAX  20
#define SAHPI_RANGE_INTERPRETED_MAX 165
#define SAHPI_RANGE_EVENT_SENSOR_MAX  15
#define THRESHOLD_RAW_MAX  32
#define THRESHOLD_INTERPRETED_MAX 264

#define POS_MAX 0
#define POS_MIN 1
#define POS_NOMINAL 2
#define POS_MAX_NOM 3
#define POS_MIN_NOM 4

#define POS_LOW_CRITICAL 0
#define POS_LOW_MAJOR 1
#define POS_LOW_MINOR 2
#define POS_UP_CRITICAL 3
#define POS_UP_MAJOR 4
#define POS_UP_MINOR 5
#define POS_POS_THD_HYSTERESIS 6
#define POS_NEG_THD_HYSTERESIS 7


    typedef struct saHpiSensorTable_context_s {
        netsnmp_index   index;
                  
        /** UNSIGNED32 = ASN_UNSIGNED */
        unsigned long   saHpiSensorIndex;

        /** INTEGER = ASN_INTEGER */
        long            saHpiSensorType;

        /** INTEGER = ASN_INTEGER */
        long            saHpiSensorCategory;

        /** INTEGER = ASN_INTEGER */
        long            saHpiSensorEventsCategoryControl;

        /** UNSIGNED32 = ASN_UNSIGNED */
        unsigned long   saHpiSensorEventsState;


      long   saHpiSensorIgnore;


        /** UNSIGNED32 = ASN_UNSIGNED */
        unsigned long   saHpiSensorReadingFormats;


        long   saHpiSensorIsNumeric;


        /** INTEGER = ASN_INTEGER */
        long            saHpiSensorSignFormat;

        /** INTEGER = ASN_INTEGER */
        long            saHpiSensorBaseUnits;

        /** INTEGER = ASN_INTEGER */
        long            saHpiSensorModifierUnits;

        /** INTEGER = ASN_INTEGER */
        long            saHpiSensorModifierUse;

        long   saHpiSensorFactorsStatic;


        /** OCTETSTR = ASN_OCTET_STR */
        unsigned char   saHpiSensorFactors[SAHPISENSORFACTORS_MAX];
        long            saHpiSensorFactors_len;

        /** INTEGER = ASN_INTEGER */
        long            saHpiSensorFactorsLinearization;

        long   saHpiSensorPercentage;

        /** UNSIGNED32 = ASN_UNSIGNED */
        unsigned long   saHpiSensorRangeFlags;

        /** OCTETSTR = ASN_OCTET_STR */
        unsigned char   saHpiSensorRangeReadingValuesPresent[SAHPI_RANGE_VALUES_MAX];
        long            saHpiSensorRangeReadingValuesPresent_len;

        /** OCTETSTR = ASN_OCTET_STR */
        unsigned char   saHpiSensorRangeReadingRaw[SAHPI_RANGE_RAW_MAX];
        long            saHpiSensorRangeReadingRaw_len;

        /** OCTETSTR = ASN_OCTET_STR */
        unsigned char   saHpiSensorRangeReadingInterpreted[SAHPI_RANGE_INTERPRETED_MAX];
        long            saHpiSensorRangeReadingInterpreted_len;

        /** OCTETSTR = ASN_OCTET_STR */
        unsigned char   saHpiSensorRangeReadingEventSensor[SAHPI_RANGE_EVENT_SENSOR_MAX];
        long            saHpiSensorRangeReadingEventSensor_len;


         long   saHpiSensorThresholdDefnIsThreshold;



        long   saHpiSensorThresholdDefnTholdCapabilities;


        /** UNSIGNED32 = ASN_UNSIGNED */
        unsigned long   saHpiSensorThresholdDefnReadThold;

        /** UNSIGNED32 = ASN_UNSIGNED */
        unsigned long   saHpiSensorThresholdDefnWriteThold;

        /** UNSIGNED32 = ASN_UNSIGNED */
        unsigned long   saHpiSensorThresholdDefnFixedThold;

        /** OCTETSTR = ASN_OCTET_STR */
        unsigned char   saHpiSensorThresholdRaw[THRESHOLD_RAW_MAX];
        long            saHpiSensorThresholdRaw_len;

        /** OCTETSTR = ASN_OCTET_STR */
        unsigned char   saHpiSensorThresholdInterpreted[THRESHOLD_INTERPRETED_MAX];
        long            saHpiSensorThresholdInterpreted_len;

        /** UNSIGNED32 = ASN_UNSIGNED */
        unsigned long   saHpiSensorOEM;

      /** RowPointer = ASN_OBJECT_ID */
          oid             saHpiSensorRDR[MAX_OID_LEN];
      long            saHpiSensorRDR_len;
      long resource_id;
      long hash;
      
    } saHpiSensorTable_context;



/*************************************************************
 * function declarations
 */
 
    void            initialize_table_saHpiSensorTable(void);
  /*
    const saHpiSensorTable_context
        *saHpiSensorTable_get_by_idx(netsnmp_index *);
    const saHpiSensorTable_context
        *saHpiSensorTable_get_by_idx_rs(netsnmp_index *, int row_status);
	*/
    int             saHpiSensorTable_get_value(netsnmp_request_info *,
                                                netsnmp_index *,
                                                netsnmp_table_request_info
                                                *);
						

  int  populate_sensor(
		       SaHpiSensorRecT *sensor,SaHpiSessionIdT resource_id,
		       oid *rdr_oid, size_t rdr_oid_len,
		       oid *sensor_oid, 
		       size_t *sensor_oid_len);

  int set_sensor(saHpiSensorTable_context *ctx);
int  
saHpiSensorTable_modify_context(
			     SaHpiSensorRecT *entry, 
			     SaHpiSensorThresholdsT *sensor_threshold,
			     SaHpiResourceIdT resource_id,
			     oid *, size_t,
			     saHpiSensorTable_context *ctx);


 
  int send_saHpiSensorTable_notification(saHpiSensorTable_context *ctx);

void 
 fill_sensor_threshold_info(saHpiSensorTable_context *ctx, 
			    SaHpiSensorThresholdsT *sensor_threshold);
void
make_SaHpiSensorTable_trap_msg(netsnmp_variable_list *list, 
	      netsnmp_index *index,
	      int column, 
	      u_char type,
	      const u_char *value, 
	      const size_t value_len);
  /*
void
update_char_with_raw_data(SaHpiUint32T value,
			  const int pos,
			  const char *);
  */

/*************************************************************
 * oid declarations
 */
    extern oid      saHpiSensorTable_oid[];
    extern size_t   saHpiSensorTable_oid_len;
//1,3,6,1,3,90,3,6
#define saHpiSensorTable_TABLE_OID hpiResources_OID,6

/*************************************************************
 * column number definitions for table saHpiSensorTable
 */
#define COLUMN_SAHPISENSORINDEX 1
#define COLUMN_SAHPISENSORTYPE 2
#define COLUMN_SAHPISENSORCATEGORY 3
#define COLUMN_SAHPISENSOREVENTSCATEGORYCONTROL 4
#define COLUMN_SAHPISENSOREVENTSSTATE 5
#define COLUMN_SAHPISENSORIGNORE 6
#define COLUMN_SAHPISENSORREADINGFORMATS 7
#define COLUMN_SAHPISENSORISNUMERIC 8
#define COLUMN_SAHPISENSORSIGNFORMAT 9
#define COLUMN_SAHPISENSORBASEUNITS 10
#define COLUMN_SAHPISENSORMODIFIERUNITS 11
#define COLUMN_SAHPISENSORMODIFIERUSE 12
#define COLUMN_SAHPISENSORFACTORSSTATIC 13
#define COLUMN_SAHPISENSORFACTORS 14
#define COLUMN_SAHPISENSORFACTORSLINEARIZATION 15
#define COLUMN_SAHPISENSORPERCENTAGE 16
#define COLUMN_SAHPISENSORRANGEFLAGS 17
#define COLUMN_SAHPISENSORRANGEREADINGVALUESPRESENT 18
#define COLUMN_SAHPISENSORRANGEREADINGRAW 19
#define COLUMN_SAHPISENSORRANGEREADINGINTERPRETED 20
#define COLUMN_SAHPISENSORRANGEREADINGEVENTSENSOR 21
#define COLUMN_SAHPISENSORTHRESHOLDDEFNISTHRESHOLD 22
#define COLUMN_SAHPISENSORTHRESHOLDDEFNTHOLDCAPABILITIES 23
#define COLUMN_SAHPISENSORTHRESHOLDDEFNREADTHOLD 24
#define COLUMN_SAHPISENSORTHRESHOLDDEFNWRITETHOLD 25
#define COLUMN_SAHPISENSORTHRESHOLDDEFNFIXEDTHOLD 26
#define COLUMN_SAHPISENSORTHRESHOLDRAW 27
#define COLUMN_SAHPISENSORTHRESHOLDINTERPRETED 28
#define COLUMN_SAHPISENSOROEM 29
#define COLUMN_SAHPISENSORRDR 30
#define saHpiSensorTable_COL_MIN 1
#define saHpiSensorTable_COL_MAX 30

 
    int            
        saHpiSensorTable_extract_index(saHpiSensorTable_context * ctx,
                                        netsnmp_index * hdr);
  
    void            saHpiSensorTable_set_reserve1(netsnmp_request_group
                                                   *);
    void            saHpiSensorTable_set_reserve2(netsnmp_request_group
                                                   *);
    void            saHpiSensorTable_set_action(netsnmp_request_group *);
    void            saHpiSensorTable_set_commit(netsnmp_request_group *);
    void            saHpiSensorTable_set_free(netsnmp_request_group *);
    void            saHpiSensorTable_set_undo(netsnmp_request_group *);
  
    saHpiSensorTable_context
        *saHpiSensorTable_duplicate_row(saHpiSensorTable_context *);
    netsnmp_index  *saHpiSensorTable_delete_row(saHpiSensorTable_context
                                                 *);

    int             saHpiSensorTable_can_delete(saHpiSensorTable_context
                                                 * undo_ctx,
                                                 saHpiSensorTable_context
                                                 * row_ctx,
                                                 netsnmp_request_group *
                                                 rg);



    saHpiSensorTable_context *saHpiSensorTable_create_row(netsnmp_index
                                                            *);

#ifdef saHpiSensorTable_IDX2
    saHpiSensorTable_context *saHpiSensorTable_get(const char *name,
                                                     int len);
#endif

#ifdef __cplusplus
};
#endif

#endif /** SAHPISENSORTABLE_H */
