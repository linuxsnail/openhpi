/*      -*- linux-c -*-
 *
 * Copyright (c) 2003 by Intel Corp.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *     Louis Zhuang <louis.zhuang@linux.intel.com>
 *     Racing Guo <racing.guo@intel.com>
 *     Vadim Revyakin <vadim.a.revyakin@intel.com>
 */

 
#include "ipmi.h"
#include <oh_utils.h>
#include <string.h>

static int prnt_fru = 0;

#define OHOI_IDR_DEFAULT_ID         0
#define OHOI_CHECK_RPT_CAP_IDR()                                     \
do{                                                                  \
	SaHpiRptEntryT           *rpt_entry;                         \
	rpt_entry = oh_get_resource_by_id(handler->rptcache, rid);   \
	if (!rpt_entry) {                                            \
		dbg("Resource %d No rptentry", rid);                 \
		return SA_ERR_HPI_INVALID_PARAMS;                    \
	}                                                            \
	if (!(rpt_entry->ResourceCapabilities &                      \
	    SAHPI_CAPABILITY_INVENTORY_DATA)) {                      \
		dbg("Resource %d no inventory capability", rid);     \
		return SA_ERR_HPI_INVALID_PARAMS;                    \
	}                                                            \
	if (idrid != OHOI_IDR_DEFAULT_ID) {                          \
		dbg("error id");                                     \
		return SA_ERR_HPI_NOT_PRESENT;                    \
	}                                                            \
}while(0)

struct ohoi_field_data {
	SaHpiIdrFieldTypeT fieldtype;
	SaHpiLanguageT lang;
	int (*get_len)(ipmi_entity_t *, unsigned int*);
	int (*get_data)(ipmi_entity_t *, char*, unsigned int*);
	int (*get_type)(ipmi_fru_t *fru, enum ipmi_str_type_e *type);
};

static struct ohoi_field_data chassis_fields[] = {
	{
		SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER,
		SAHPI_LANG_ENGLISH, 
		ipmi_entity_get_chassis_info_serial_number_len,
		ipmi_entity_get_chassis_info_serial_number,
		ipmi_fru_get_chassis_info_serial_number_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_PART_NUMBER,
		SAHPI_LANG_UNDEF,
		ipmi_entity_get_chassis_info_part_number_len,
		ipmi_entity_get_chassis_info_part_number,
		ipmi_fru_get_chassis_info_part_number_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_CUSTOM,
		SAHPI_LANG_UNDEF,
		NULL,
		NULL,
		NULL,
	}
};

static int _ipmi_fru_get_board_info_mfg_time_len(ipmi_entity_t *entity,
						 unsigned int  *length)
{
	*length = sizeof (time_t);
	return 0;
}
static int _ipmi_fru_get_board_info_mfg_time(ipmi_entity_t *entity,
					     char          *str,
					     unsigned int  *strlen)
{
	time_t tm;
	int res;
	
	res = ipmi_entity_get_board_info_mfg_time(entity, &tm);
	if (res) {
		return res;
	}
	memcpy(str, &tm, sizeof (tm));
	*strlen = sizeof (tm);
	return 0;
}

static int _ipmi_fru_get_board_info_mfg_time_type(ipmi_fru_t *fru,
						  enum ipmi_str_type_e *type)
{
	*type = IPMI_FRU_DATA_BINARY;
	return 0;
}
	
static struct ohoi_field_data board_fields[] = {
	{
		SAHPI_IDR_FIELDTYPE_MFG_DATETIME,
		SAHPI_LANG_UNDEF,
		_ipmi_fru_get_board_info_mfg_time_len, 
		_ipmi_fru_get_board_info_mfg_time,
		_ipmi_fru_get_board_info_mfg_time_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_MANUFACTURER,
		SAHPI_LANG_UNDEF,
		ipmi_entity_get_board_info_board_manufacturer_len, 
		ipmi_entity_get_board_info_board_manufacturer,
		ipmi_fru_get_board_info_board_manufacturer_type
	},
	{
		SAHPI_IDR_FIELDTYPE_PRODUCT_NAME,
		SAHPI_LANG_UNDEF, 
		ipmi_entity_get_board_info_board_product_name_len, 
		ipmi_entity_get_board_info_board_product_name,
		ipmi_fru_get_board_info_board_product_name_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER,
		SAHPI_LANG_ENGLISH, 
		ipmi_entity_get_board_info_board_serial_number_len, 
		ipmi_entity_get_board_info_board_serial_number,
		ipmi_fru_get_board_info_board_serial_number_type
	},
	{
		SAHPI_IDR_FIELDTYPE_PART_NUMBER,
		SAHPI_LANG_UNDEF, 
		ipmi_entity_get_board_info_board_part_number_len, 
		ipmi_entity_get_board_info_board_part_number,
		ipmi_fru_get_board_info_board_part_number_type
	},
	{
		SAHPI_IDR_FIELDTYPE_FILE_ID,
		SAHPI_LANG_ENGLISH, 
		ipmi_entity_get_board_info_fru_file_id_len, 
		ipmi_entity_get_board_info_fru_file_id,
		ipmi_fru_get_board_info_fru_file_id_type
	},
	{
		SAHPI_IDR_FIELDTYPE_CUSTOM,
		SAHPI_LANG_UNDEF,
		NULL,
		NULL,
		NULL,
	}
};

static int _ipmi_entity_get_product_info_custom_len(ipmi_entity_t *entity,
				      unsigned int  *len)
{
	return ipmi_entity_get_product_info_custom_len(entity,0,len);
}

static int _ipmi_entity_get_product_info_custom(ipmi_entity_t *entity,
				      char *data, unsigned int  *max_len)
{
	return ipmi_entity_get_product_info_custom(entity,0,data,max_len);
}

static int _ipmi_fru_get_product_info_custom_type(ipmi_fru_t *fru,
				      enum ipmi_str_type_e  *type)
{
	return ipmi_fru_get_product_info_custom_type(fru, 0, type);
}

static struct ohoi_field_data product_fields[] = {
	{
		SAHPI_IDR_FIELDTYPE_MANUFACTURER,
		SAHPI_LANG_UNDEF, 
		ipmi_entity_get_product_info_manufacturer_name_len, 
		ipmi_entity_get_product_info_manufacturer_name,
		ipmi_fru_get_product_info_manufacturer_name_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_PRODUCT_NAME,
		SAHPI_LANG_UNDEF,
		ipmi_entity_get_product_info_product_name_len,
		ipmi_entity_get_product_info_product_name,
		ipmi_fru_get_product_info_product_name_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_PART_NUMBER,
		SAHPI_LANG_UNDEF,
		ipmi_entity_get_product_info_product_part_model_number_len,
		ipmi_entity_get_product_info_product_part_model_number,
		ipmi_fru_get_product_info_product_part_model_number_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_PRODUCT_VERSION,
		SAHPI_LANG_UNDEF,
		ipmi_entity_get_product_info_product_version_len,
		ipmi_entity_get_product_info_product_version,
		ipmi_fru_get_product_info_product_version_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER,
		SAHPI_LANG_ENGLISH,
		ipmi_entity_get_product_info_product_serial_number_len,
		ipmi_entity_get_product_info_product_serial_number,
		ipmi_fru_get_product_info_product_serial_number_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_FILE_ID,
		SAHPI_LANG_UNDEF,
		ipmi_entity_get_product_info_fru_file_id_len,
		ipmi_entity_get_product_info_fru_file_id,
		ipmi_fru_get_product_info_fru_file_id_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_ASSET_TAG,
		SAHPI_LANG_UNDEF,
		ipmi_entity_get_product_info_asset_tag_len,
		ipmi_entity_get_product_info_asset_tag,
		ipmi_fru_get_product_info_asset_tag_type,
	},
	{
		SAHPI_IDR_FIELDTYPE_CUSTOM,
		SAHPI_LANG_UNDEF,
		_ipmi_entity_get_product_info_custom_len,
		_ipmi_entity_get_product_info_custom,
		_ipmi_fru_get_product_info_custom_type,
	}
};

static int _ipmi_entity_get_internal_use_data(ipmi_entity_t *entity,
					      char *data,
					      unsigned int  *max_len)
{
	return ipmi_entity_get_internal_use_data(entity, data, max_len);
}
static int _ipmi_fru_get_multi_record_type(ipmi_fru_t    *fru,
						enum ipmi_str_type_e *type)
{
	return ipmi_fru_get_multi_record_type(fru, 0, (char *)type);
}

static struct ohoi_field_data internal_fields[] = {
	{
		SAHPI_IDR_FIELDTYPE_CUSTOM,
		SAHPI_LANG_UNDEF,
		ipmi_entity_get_internal_use_length,
		_ipmi_entity_get_internal_use_data,
		_ipmi_fru_get_multi_record_type
	},
	{
		SAHPI_IDR_FIELDTYPE_CUSTOM,
		SAHPI_LANG_UNDEF,
		NULL,
		NULL,
		NULL,
	}
};

static struct ohoi_area_data {
	int field_num;
	SaHpiIdrAreaTypeT areatype;
	unsigned int empty_len;
	SaHpiBoolT read_only;
	struct ohoi_field_data *fields;
} areas [] = {
	{
		sizeof(internal_fields)/sizeof(internal_fields[0]),
		SAHPI_IDR_AREATYPE_INTERNAL_USE,
		8,
		SAHPI_TRUE,
		internal_fields
	},
	{	sizeof(chassis_fields)/sizeof(chassis_fields[0]),
		SAHPI_IDR_AREATYPE_CHASSIS_INFO,
		8,
		SAHPI_FALSE,
		chassis_fields
	}, 
	{
		sizeof(board_fields)/sizeof(board_fields[0]),
		SAHPI_IDR_AREATYPE_BOARD_INFO,
		16,
		SAHPI_FALSE,
		board_fields
	},
	{
		sizeof(product_fields)/sizeof(product_fields[0]),
		SAHPI_IDR_AREATYPE_PRODUCT_INFO,
		16,
		SAHPI_FALSE,
		product_fields
	},
	{
		0,
		SAHPI_IDR_AREATYPE_OEM,
		0,
		SAHPI_TRUE,
		NULL
	}
};

#define OHOI_AREA_NUM        (int)(sizeof(areas)/sizeof(areas[0]))
#define OHOI_AREA_EMPTY_ID   0
#define OHOI_AREA_FIRST_ID   1
#define OHOI_AREA_LAST_ID    OHOI_AREA_NUM

#define OHOI_FIELD_NUM(area)         (area->field_num)
#define OHOI_FIELD_EMPTY_ID          0
#define OHOI_FIELD_FIRST_ID          1
#define OHOI_FIELD_LAST_ID(area)     (area->field_num)

	
struct ohoi_area_layout {
	unsigned int off;
	unsigned int len;
	unsigned int used_len;
};


struct ohoi_fru_layout {
	unsigned int len;
	unsigned int free_len;
	struct ohoi_area_layout areas[IPMI_FRU_FTR_NUMBER - 1];
};



static SaHpiEntryIdT get_areaid_by_type(SaHpiIdrAreaTypeT areatype)
{
	SaHpiEntryIdT id;
	
	for (id = 0; id < OHOI_AREA_NUM; id++) {
		if (areas[id].areatype == areatype) {
			return id + 1;
		}
	}
	return 0;
}
static SaHpiIdrAreaTypeT get_areatype_by_id(SaHpiEntryIdT id)
{
	if (id > OHOI_AREA_NUM || id < 1) {
		return OHOI_AREA_EMPTY_ID;
	}
	return areas[id - 1].areatype;
}


static unsigned int get_ipmi_areatype(SaHpiIdrAreaTypeT areatype)
{
	switch (areatype) {
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		return IPMI_FRU_FTR_BOARD_INFO_AREA;
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		return IPMI_FRU_FTR_PRODUCT_INFO_AREA;
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:
		return IPMI_FRU_FTR_CHASSIS_INFO_AREA;
	case SAHPI_IDR_AREATYPE_INTERNAL_USE:
		return IPMI_FRU_FTR_INTERNAL_USE_AREA;
	case SAHPI_IDR_AREATYPE_OEM:
		return IPMI_FRU_FTR_MULTI_RECORD_AREA;
	default:
		return IPMI_FRU_FTR_NUMBER;
	}
}

#if 0
static SaHpiIdrFieldTypeT get_fieldtype_by_id(SaHpiIdrAreaTypeT areatype,
                                              SaHpiEntryIdT fieldid)
{
	struct ohoi_area_data *area;
	SaHpiEntryIdT areaid;
	
	if (fieldid < 1) {
		return SAHPI_IDR_FIELDTYPE_UNSPECIFIED;
	}
	if (areatype == SAHPI_IDR_AREATYPE_OEM) {
		return SAHPI_IDR_FIELDTYPE_UNSPECIFIED;
	}
	areaid = get_areaid_by_type(areatype);
	if (areaid == 0) {
		return SAHPI_IDR_FIELDTYPE_UNSPECIFIED;
	}
	area = &areas[areaid - 1];
	if (fieldid >= OHOI_FIELD_NUM(area) + 1) {
		return SAHPI_IDR_FIELDTYPE_CUSTOM;
	}
	return area->fields[fieldid].fieldtype;
}
#endif


static SaHpiEntryIdT get_fieldid_by_type(SaHpiEntryIdT areaid,
                                         SaHpiIdrFieldTypeT type)
{
	struct ohoi_area_data *area;
	SaHpiEntryIdT i;
	
	if (areaid < 1 || areaid > OHOI_AREA_NUM) {
		dbg("Invalid areaid 0x%x", areaid);
		return 0;
	}
	area = &areas[areaid - 1];
	for (i = 0; i < OHOI_FIELD_NUM(area); i++) {
		if (area->fields[i].fieldtype == type) {
			return i + 1;
		}
	}
	dbg("No area field type %d in areatype 0x%x", type, area->areatype);
	return 0;
}

static int get_fru_layout(ipmi_fru_t *fru, struct ohoi_fru_layout *layout)
{
	unsigned int len, used_len;
	unsigned int offset;
	unsigned int common_len = 0;
	int rv;
	SaHpiEntryIdT id;

	layout->free_len = 0;
	layout->len = ipmi_fru_get_data_length(fru);

	rv = ipmi_fru_area_get_length(fru, IPMI_FRU_FTR_INTERNAL_USE_AREA,
			&len);
	rv |= ipmi_fru_area_get_offset(fru, IPMI_FRU_FTR_INTERNAL_USE_AREA,
			&offset);
	rv |= ipmi_fru_area_get_used_length(fru, IPMI_FRU_FTR_INTERNAL_USE_AREA,
			&used_len);
	if (rv != 0) {
		offset = 0;
		len = 0;
		used_len = 0;
	}
	id = get_ipmi_areatype(SAHPI_IDR_AREATYPE_INTERNAL_USE);
	layout->areas[id].off = offset;
	layout->areas[id].len = len;
	layout->areas[id].used_len = used_len;
	common_len += len;
	
	rv = ipmi_fru_area_get_length(fru, IPMI_FRU_FTR_CHASSIS_INFO_AREA,
			&len);
	rv |= ipmi_fru_area_get_offset(fru, IPMI_FRU_FTR_CHASSIS_INFO_AREA,
			&offset);
	rv |= ipmi_fru_area_get_used_length(fru, IPMI_FRU_FTR_CHASSIS_INFO_AREA,
			&used_len);
	if (rv != 0) {
		offset = 0;
		len = 0;
		used_len = 0;
	}
	id = get_ipmi_areatype(SAHPI_IDR_AREATYPE_CHASSIS_INFO);
	layout->areas[id].off = offset;
	layout->areas[id].len = len;
	layout->areas[id].used_len = used_len;
	common_len += len;
	
	rv = ipmi_fru_area_get_length(fru, IPMI_FRU_FTR_BOARD_INFO_AREA,
			&len);
	rv |= ipmi_fru_area_get_offset(fru, IPMI_FRU_FTR_BOARD_INFO_AREA,
			&offset);
	rv |= ipmi_fru_area_get_used_length(fru, IPMI_FRU_FTR_BOARD_INFO_AREA,
			&used_len);
	if (rv != 0) {
		offset = 0;
		len = 0;
		used_len = 0;
	}
	id = get_ipmi_areatype(SAHPI_IDR_AREATYPE_BOARD_INFO);
	layout->areas[id].off = offset;
	layout->areas[id].len = len;
	layout->areas[id].used_len = used_len;
	common_len += len;
	
	rv = ipmi_fru_area_get_length(fru, IPMI_FRU_FTR_PRODUCT_INFO_AREA,
			&len);
	rv |= ipmi_fru_area_get_offset(fru, IPMI_FRU_FTR_PRODUCT_INFO_AREA,
			&offset);
	rv |= ipmi_fru_area_get_used_length(fru, IPMI_FRU_FTR_PRODUCT_INFO_AREA,
			&used_len);
	if (rv != 0) {
		offset = 0;
		len = 0;
		used_len = 0;
	}
	id = get_ipmi_areatype(SAHPI_IDR_AREATYPE_PRODUCT_INFO);
	layout->areas[id].off = offset;
	layout->areas[id].len = len;
	layout->areas[id].used_len = used_len;
	common_len += len;

	rv = ipmi_fru_area_get_length(fru, IPMI_FRU_FTR_MULTI_RECORD_AREA,
			&len);
	rv |= ipmi_fru_area_get_offset(fru, IPMI_FRU_FTR_MULTI_RECORD_AREA,
			&offset);
	rv |= ipmi_fru_area_get_used_length(fru, IPMI_FRU_FTR_MULTI_RECORD_AREA,
			&used_len);
	if (rv != 0) {
		offset = 0;
		len = 0;
		used_len = 0;
	}
	id = get_ipmi_areatype(SAHPI_IDR_AREATYPE_OEM);
	layout->areas[id].off = offset;
	layout->areas[id].len = len;
	layout->areas[id].used_len = used_len;
	common_len += len;
	
	layout->free_len = layout->len - common_len;
	
	return 0;
}



static void print_fru_layout(char *head, struct ohoi_fru_layout *l)
{
	struct ohoi_area_layout *al;
	
	if (!prnt_fru) {
		return;
	}
	if (head != NULL) {
		printf("*****   %s\n", head);
	}
	printf("       FRU length = %i, free_len = %i\n", l->len, l->free_len);
	al = &l->areas[IPMI_FRU_FTR_INTERNAL_USE_AREA];
	printf("         Internal Use Area: off = %i; len = %i"
			"; used len = %i\n", al->off, al->len, al->used_len);
	al = &l->areas[IPMI_FRU_FTR_CHASSIS_INFO_AREA];
	printf("         Chassis Info Area: off = %i; len = %i"
			"; used len = %i\n", al->off, al->len, al->used_len);
	al = &l->areas[IPMI_FRU_FTR_BOARD_INFO_AREA];
	printf("         Board Info Area: off = %i; len = %i"
			"; used len = %i\n", al->off, al->len, al->used_len);
	al = &l->areas[IPMI_FRU_FTR_PRODUCT_INFO_AREA];
	printf("         Product Info Area: off = %i; len = %i"
			"; used len = %i\n", al->off, al->len, al->used_len);
	al = &l->areas[IPMI_FRU_FTR_MULTI_RECORD_AREA];
	printf("         Multi Record Area: off = %i; len = %i"
			"; used len = %i\n", al->off, al->len, al->used_len);
}






struct ohoi_fru_write {
	int done;
	SaErrorT rv;
};

static void fru_write_done_cb(ipmi_domain_t *domain,
			ipmi_fru_t    *fru,
			int           err,
			void          *cb_data)
{
	struct ohoi_fru_write *fw = cb_data;
	fw->done = 1;
	if (err) {
		dbg("err = %d", err);
		fw->rv = SA_ERR_HPI_INTERNAL_ERROR;
	}
}

static int get_first_area(SaHpiIdrAreaTypeT areatype)
{
	int i;
	for (i = 0; i < OHOI_AREA_NUM; i++)
		if (areas[i].areatype == areatype) {
			return (i + 1);
		}
	return OHOI_AREA_EMPTY_ID;
}

static int get_first_field(struct ohoi_area_data *area,
			   struct ohoi_inventory_info *fru)
{
	int i;
	unsigned int *msk;

	switch (area->areatype) {
	case SAHPI_IDR_AREATYPE_INTERNAL_USE:
		return 1;
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:
		msk = &fru->ci_fld_msk;
		break;
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		msk = &fru->bi_fld_msk;
		break;	
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		msk = &fru->pi_fld_msk;
		break;
	default:
		return OHOI_FIELD_EMPTY_ID;	
	}

	for (i = 0; i < OHOI_FIELD_NUM(area); i++) {
		if ((1 << area->fields[i].fieldtype) & *msk) {
			return (i + 1);
		}
	}
	return OHOI_FIELD_EMPTY_ID;
}

static int get_first_typed_field(struct ohoi_area_data *area,
			   SaHpiIdrFieldTypeT fieldtype,
			   struct ohoi_inventory_info *fru)
{
	int i;
	unsigned int *msk;
	
	switch (area->areatype) {
	case SAHPI_IDR_AREATYPE_INTERNAL_USE:
		return 1;
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:
		msk = &fru->ci_fld_msk;
		break;
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		msk = &fru->bi_fld_msk;
		break;	
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		msk = &fru->pi_fld_msk;
		break;
	default:
		return OHOI_FIELD_EMPTY_ID;	
	}
	if (!((1 << fieldtype) & *msk)) {
		return OHOI_FIELD_EMPTY_ID;
	}
	for (i = 0; i < OHOI_FIELD_NUM(area); i++) {
		if (area->fields[i].fieldtype ==  fieldtype) {
			return (i + 1);
		}
	}
	return OHOI_FIELD_EMPTY_ID;
}

SaHpiTextTypeT convert_to_hpi_data_type(enum ipmi_str_type_e type)
{
	switch (type) {
	case IPMI_ASCII_STR:
		return SAHPI_TL_TYPE_TEXT;
	case IPMI_UNICODE_STR:
		return SAHPI_TL_TYPE_UNICODE;
	case IPMI_BINARY_STR:
		return SAHPI_TL_TYPE_BINARY;
	}
	return SAHPI_TL_TYPE_BINARY;
}


static SaErrorT get_str_type(SaHpiTextBufferT *tb,
		      SaHpiLanguageT lang,
		      enum ipmi_str_type_e *type)
{
	switch (tb->DataType) {
	case SAHPI_TL_TYPE_BINARY:
		*type = IPMI_BINARY_STR;
		return SA_OK;
	case SAHPI_TL_TYPE_TEXT:
	case SAHPI_TL_TYPE_ASCII6:
	case SAHPI_TL_TYPE_BCDPLUS:
		*type = IPMI_ASCII_STR;
		break;
	case SAHPI_TL_TYPE_UNICODE:
		*type = IPMI_UNICODE_STR;
		break;
	default:
		return SA_ERR_HPI_INVALID_DATA;
	}
	if (lang && (tb->Language != lang)) {
		return SA_ERR_HPI_INVALID_DATA;
	}
	return SA_OK;
} 


static SaHpiLanguageT get_language(struct ohoi_inventory_info *i_info,
				   SaHpiIdrAreaTypeT areatype)
{
	switch (areatype) {
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		return i_info->bi;
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		return i_info->pi;
	default:
		return SAHPI_LANG_UNDEF;
	}
}



#if 0
static SaHpiBoolT valid_area_type(SaHpiIdrAreaTypeT areatype)
{
	switch(areatype) {
	case SAHPI_IDR_AREATYPE_INTERNAL_USE:
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
	case SAHPI_IDR_AREATYPE_OEM:
	case SAHPI_IDR_AREATYPE_UNSPECIFIED:
		return SAHPI_TRUE;
	default:
		return SAHPI_FALSE;
	}
}
#endif




static unsigned char get_area_presence(struct ohoi_inventory_info *i_info,
				   SaHpiIdrAreaTypeT areatype)
{
	switch (areatype) {
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		return i_info->bi;
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		return i_info->pi;
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:
		return i_info->ci;
	case SAHPI_IDR_AREATYPE_OEM:
		return i_info->oem;
	case SAHPI_IDR_AREATYPE_INTERNAL_USE:
		return i_info->iu;
	default:
		return (unsigned char)0;
	}
}



static void set_area_presence(struct ohoi_inventory_info *i_info,
				   SaHpiIdrAreaTypeT areatype)
{
	switch (areatype) {
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		i_info->bi = SAHPI_LANG_ENGLISH;
		i_info->bi_fld_msk = 
			(1 << SAHPI_IDR_FIELDTYPE_MANUFACTURER) |
			(1 << SAHPI_IDR_FIELDTYPE_MFG_DATETIME) |
			(1 << SAHPI_IDR_FIELDTYPE_PRODUCT_NAME) |
			(1 << SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER) |
			(1 << SAHPI_IDR_FIELDTYPE_PART_NUMBER) |
			(1 << SAHPI_IDR_FIELDTYPE_FILE_ID);
		return;
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		i_info->pi = SAHPI_LANG_ENGLISH;
		i_info->pi_fld_msk = 
			(1 << SAHPI_IDR_FIELDTYPE_MANUFACTURER) |
			(1 << SAHPI_IDR_FIELDTYPE_PRODUCT_NAME) |
			(1 << SAHPI_IDR_FIELDTYPE_PART_NUMBER) |
			(1 << SAHPI_IDR_FIELDTYPE_PRODUCT_VERSION) |
			(1 << SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER) |
			(1 << SAHPI_IDR_FIELDTYPE_ASSET_TAG) |
			(1 << SAHPI_IDR_FIELDTYPE_FILE_ID);
		return;
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:	
		i_info->ci = 1;
		i_info->ci_fld_msk =
			(1 << SAHPI_IDR_FIELDTYPE_CHASSIS_TYPE) |
			(1 << SAHPI_IDR_FIELDTYPE_PART_NUMBER) |
			(1 << SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER);
		return;
	case SAHPI_IDR_AREATYPE_OEM:
		i_info->oem = 1;
		return;
		default:
		return;
	}
}


static void unset_area_presence(struct ohoi_inventory_info *i_info,
				   SaHpiIdrAreaTypeT areatype)
{
	switch (areatype) {
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		i_info->bi = 0;
		i_info->bi_fld_msk = 0;
		i_info->bi_custom_num = 0;
		return;
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		i_info->pi = 0;
		i_info->pi_fld_msk = 0;
		i_info->pi_custom_num = 0;
		return;
	case SAHPI_IDR_AREATYPE_OEM:
		i_info->oem = 0;
		i_info->oem_fields_num = 0;
		return;
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:	
		i_info->ci = 0;
		i_info->ci_fld_msk = 0;
		i_info->ci_custom_num = 0;
		return;
	default:
		return;
	}
}









struct ohoi_get_field {
	struct ohoi_field_data *data;
	SaHpiIdrFieldT *field;
	int done;
	SaErrorT rv;
};

static void get_field(ipmi_entity_t *ent,
		      void          *cb_data)
{
	int rv;
	int len;
	enum ipmi_str_type_e type;
	ipmi_fru_t *fru;
	int (*get_len)(ipmi_entity_t *, unsigned int*);
	int (*get_data)(ipmi_entity_t *, char*, unsigned int*);
	struct ohoi_get_field *gf = cb_data;
	struct ohoi_field_data *data = gf->data;
	
	SaHpiIdrFieldT *field = gf->field;
	get_len = data->get_len;
	get_data = data->get_data;
	
	field->Type = data->fieldtype;
	field->ReadOnly = SAHPI_FALSE;

	gf->done = 1;
	fru = ipmi_entity_get_fru(ent);
	if (fru == NULL) {
		dbg("Bug: entity without fru");
		gf->rv = SA_ERR_HPI_INTERNAL_ERROR;
		return;
	}
	
	rv = data->get_type(fru, &type);
	if (rv) {
		dbg("Could not get data type = %d. set SAHPI_TL_TYPE_BINARY", rv);
		field->Field.DataType = SAHPI_TL_TYPE_BINARY;
	} else {
		field->Field.DataType = convert_to_hpi_data_type(type);
	}
	field->Field.Language = SAHPI_LANG_ENGLISH;
	field->Field.DataLength = 0;

	rv = get_len(ent, &len);
	if (rv) {
		dbg("Error on get_len: %d", rv);
		gf->rv = SA_ERR_HPI_NOT_PRESENT;
		return;
	}
	len++;  /* if ASCII string (yes), add one for NULL char. */

	if (len > SAHPI_MAX_TEXT_BUFFER_LENGTH)
		len = SAHPI_MAX_TEXT_BUFFER_LENGTH;

	rv = get_data(ent, &field->Field.Data[0], &len);
	if (!rv) {
		field->Field.DataLength = len;
	} else {
		dbg("Error on  get_data: %d", rv);
		gf->rv = SA_ERR_HPI_INTERNAL_ERROR;
	}
}




struct oem_idr_field {
	SaHpiIdrFieldT *field;
	SaErrorT rv;
	int done;
};

static void get_oem_idr_field_cb(ipmi_entity_t *ent, void *cbdata)
{
	struct oem_idr_field *oif = cbdata;
	int rv;
	unsigned int len;
	unsigned char ver, type;
	unsigned int f_id = oif->field->FieldId - 1;
	
	oif->done = 1;
	rv = ipmi_entity_get_multi_record_data_len(ent, f_id, &len);
	if (rv) {
		dbg("ipmi_entity_get_multi_record_data_len = %d", rv);
		oif->rv = SA_ERR_HPI_NOT_PRESENT;
		return;
	}
	rv = ipmi_entity_get_multi_record_type(ent, f_id, &type);
	if (rv) {
		dbg("ipmi_entity_get_multi_record_type = %d", rv);
		oif->rv = SA_ERR_HPI_NOT_PRESENT;
		return;
	}
	rv = ipmi_entity_get_multi_record_format_version(ent, f_id, &ver);
	if (rv) {
		dbg("ipmi_entity_get_multi_record_format_version = %d", rv);
		oif->rv = SA_ERR_HPI_NOT_PRESENT;
		return;
	}
	if (len > SAHPI_MAX_TEXT_BUFFER_LENGTH - 2) {
		len = SAHPI_MAX_TEXT_BUFFER_LENGTH - 2;
	}
	rv = ipmi_entity_get_multi_record_data(ent, f_id,
		&oif->field->Field.Data[2], &len);
	if (rv) {
		dbg("ipmi_entity_get_multi_record_data = %d", rv);
		oif->rv = SA_ERR_HPI_NOT_PRESENT;
		return;
	}
	oif->field->Field.Data[0] = type;
	oif->field->Field.Data[1] = ver;
	oif->field->Field.DataLength = len + 2;
	oif->field->Field.DataType = SAHPI_TL_TYPE_BINARY;
	oif->rv = SA_OK;
}


static SaErrorT get_oem_idr_field(struct oh_handler_state  *handler,
				struct ohoi_resource_info   *ohoi_res_info,
				SaHpiIdrFieldTypeT fieldtype,
				SaHpiEntryIdT fieldid,
				SaHpiEntryIdT *nextfieldid,
				SaHpiIdrFieldT *field)
{
	struct ohoi_inventory_info *i_info = ohoi_res_info->fru;
	struct oem_idr_field oif;
	int rv;
	
	if (fieldtype != SAHPI_IDR_FIELDTYPE_UNSPECIFIED) {
		return SA_ERR_HPI_INVALID_PARAMS;
	}
	if (fieldid == SAHPI_FIRST_ENTRY) {
		fieldid = 1;
	}
	if (fieldid > i_info->oem_fields_num) {
		dbg("fieldid(%d) > i_info->oem_fields_num(%d)",
			fieldid, i_info->oem_fields_num);
		return SA_ERR_HPI_NOT_PRESENT;
	}
	if (fieldid < 1) {
		dbg("fieldid(%d) < 1", fieldid);
		return SA_ERR_HPI_NOT_PRESENT;
	}
	field->FieldId = fieldid;
	field->Type = SAHPI_IDR_FIELDTYPE_UNSPECIFIED;
	oif.done = 0;
	oif.rv = SA_OK;
	oif.field = field;
	rv = ipmi_entity_pointer_cb(ohoi_res_info->u.entity_id,
		get_oem_idr_field_cb, &oif);
	if (rv) {
		dbg("ipmi_entity_pointer_cb returned %d", rv);
		oif.rv = SA_ERR_HPI_INTERNAL_ERROR;
	} else {
		oif.rv = ohoi_loop(&oif.done, handler->data);
	}
	if (oif.rv != SA_OK) {
		dbg("get_oem_idr_field. rv = %d", oif.rv);
	} else if (fieldid < i_info->oem_fields_num) {
		*nextfieldid = fieldid + 1;
	} else {
		*nextfieldid = SAHPI_LAST_ENTRY;
	}
		
	return oif.rv; 
}






struct ohoi_custom_field {
	int (*get_len)(ipmi_fru_t *, unsigned int, unsigned int*);
	int (*get_data)(ipmi_fru_t *, unsigned int, char*, unsigned int*);
	SaHpiIdrFieldT *field;
	unsigned int num;
	SaErrorT rv;
	int done;
};

static void get_custom_field_cb(ipmi_entity_t *ent, void *cbdata)
{
	struct ohoi_custom_field *cf = cbdata;
	ipmi_fru_t *fru;
	SaHpiIdrFieldT *field = cf->field;
	unsigned int len;
	int rv;
	
	cf->done = 1;
	fru = ipmi_entity_get_fru(ent);
	if (fru == NULL) {
		dbg("Bug: entity without fru");
		cf->rv = SA_ERR_HPI_INTERNAL_ERROR;
		return;
	}
	field->Field.DataType = SAHPI_TL_TYPE_BINARY;
	
	field->Field.Language = SAHPI_LANG_ENGLISH;
	field->Field.DataLength = 0;

	rv = cf->get_len(fru, cf->num, &len);
	if (rv) {
		dbg("Error on get_len: %d", rv);
		cf->rv = SA_ERR_HPI_NOT_PRESENT;
		return;
	}

	if (len > SAHPI_MAX_TEXT_BUFFER_LENGTH)
		len = SAHPI_MAX_TEXT_BUFFER_LENGTH;

	rv = cf->get_data(fru, cf->num, &field->Field.Data[0], &len);
	if (!rv) {
		field->Field.DataLength = len;
	} else {
		dbg("Error on  get_data: %d", rv);
		cf->rv = SA_ERR_HPI_INTERNAL_ERROR;
	}
}

		
static SaErrorT get_custom_field(struct oh_handler_state  *handler,
				struct ohoi_resource_info   *ohoi_res_info,
				SaHpiEntryIdT lastid,
				SaHpiEntryIdT fieldid,
				SaHpiEntryIdT *nextfieldid,
				SaHpiIdrFieldT *field)
{
	unsigned int num;
	struct ohoi_custom_field cf;
	int rv;
	
	switch(areas[field->AreaId - 1].areatype) {
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:
		cf.get_len = ipmi_fru_get_chassis_info_custom_len;
		cf.get_data = ipmi_fru_get_chassis_info_custom;
		num = ohoi_res_info->fru->ci_custom_num;
		break;
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		cf.get_len = ipmi_fru_get_board_info_custom_len;
		cf.get_data = ipmi_fru_get_board_info_custom;
		num = ohoi_res_info->fru->bi_custom_num;
		break;
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		cf.get_len =ipmi_fru_get_product_info_custom_len;
		cf.get_data =ipmi_fru_get_product_info_custom;
		//get_type = ipmi_fru_get_product_info_custom_type
		num = ohoi_res_info->fru->pi_custom_num;
		break;
	default:
		dbg("bug: areaa %d; wrong areatype %x",
			field->AreaId, areas[field->AreaId -1].areatype);
		return SA_ERR_HPI_NOT_PRESENT;
	}
	
	if (fieldid - lastid >= num) {
		dbg("fieldid(%d) - lastid(%d) >= num(%d)", fieldid, lastid, num);
		return SA_ERR_HPI_NOT_PRESENT;
	}
	
	cf.done = 0;
	cf.rv = SA_OK;
	cf.num = fieldid - lastid;
	cf.field = field;
	
	rv = ipmi_entity_pointer_cb(ohoi_res_info->u.entity_id,
		get_custom_field_cb, &cf);
	if (rv) {
		dbg("ipmi_entity_pointer_cb returned %d", rv);
		cf.rv = SA_ERR_HPI_INTERNAL_ERROR;
	} else {
		cf.rv = ohoi_loop(&cf.done, handler->data);
	}
	if (cf.rv != SA_OK) {
		dbg("error after get_custom_field_cb cf.rv =%d", cf.rv);
		return cf.rv;
	}
	field->Field.DataType = SAHPI_TL_TYPE_TEXT; // FIXME
	field->Field.Language = SAHPI_LANG_ENGLISH;   // FIXME
	if (fieldid + 1 - lastid < num) {
		*nextfieldid = fieldid + 1;
	} else {
		*nextfieldid = SAHPI_LAST_ENTRY;
	}
	return SA_OK;	 
}


SaErrorT ohoi_get_idr_info(void *hnd, SaHpiResourceIdT rid, SaHpiIdrIdT idrid,
                           SaHpiIdrInfoT *idrinfo)
{
	struct oh_handler_state  *handler = hnd;
	struct ohoi_resource_info   *ohoi_res_info;
	struct ohoi_inventory_info  *fru;
	SaHpiUint32T na = 0;

	OHOI_CHECK_RPT_CAP_IDR();
	ohoi_res_info = oh_get_resource_data(handler->rptcache, rid);
	if (ohoi_res_info->fru == NULL) {
		dbg("bug: resource without fru?");
		return SA_ERR_HPI_CAPABILITY;
	}
	if (idrid != OHOI_IDR_DEFAULT_ID) {
		return SA_ERR_HPI_NOT_PRESENT;
	}
	fru = ohoi_res_info->fru;
	g_mutex_lock(fru->mutex);
	idrinfo->IdrId = OHOI_IDR_DEFAULT_ID;
	idrinfo->UpdateCount = fru->update_count;
	idrinfo->ReadOnly = SAHPI_FALSE;

	if (fru->iu) {
		na++;
	}
	if (fru->ci) {
		na++;
	}
	if (fru->bi) {
		na++;
	}
	if (fru->pi) {
		na++;
	}
	if (fru->oem) {
		na++;
	}
	idrinfo->NumAreas = na;
	g_mutex_unlock(fru->mutex);
	return SA_OK;
}

static SaHpiUint32T get_num_fields(struct ohoi_inventory_info *fru,
			    SaHpiIdrAreaTypeT areatype)
{
	SaHpiUint32T nf = 0;
	unsigned int msk;
	unsigned int cust_num;
	int i;

	switch (areatype) {
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		msk = fru->bi_fld_msk;
		cust_num = fru->bi_custom_num;
		break;
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		msk = fru->pi_fld_msk;
		cust_num = fru->pi_custom_num;
		break;
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:
		msk = fru->ci_fld_msk;
		cust_num = fru->ci_custom_num;
		break;
	case SAHPI_IDR_AREATYPE_INTERNAL_USE:
		return 1;
	case SAHPI_IDR_AREATYPE_OEM:
		return fru->oem_fields_num;
	default:
		return (SaHpiUint32T)0;
	}

	msk &= ~SAHPI_IDR_FIELDTYPE_CUSTOM; 
	for (i = 0; i < sizeof (msk) * 8; i++) {
		if (msk & 1) {
			nf++;
		}
		msk = (msk >> 1);
	}
	nf += cust_num;

	return nf;
}




SaErrorT ohoi_get_idr_area_header(void *hnd, SaHpiResourceIdT rid,
				  SaHpiIdrIdT idrid,
                                  SaHpiIdrAreaTypeT areatype,
				  SaHpiEntryIdT areaid,
                                  SaHpiEntryIdT *nextareaid,
				  SaHpiIdrAreaHeaderT *header)
{
	struct oh_handler_state  *handler = hnd;
	struct ohoi_area_data *area;
	struct ohoi_resource_info   *ohoi_res_info;
	SaHpiEntryIdT tmp_id;
	struct ohoi_inventory_info *fru;

	OHOI_CHECK_RPT_CAP_IDR();
	ohoi_res_info = oh_get_resource_data(handler->rptcache, rid);
	if (ohoi_res_info->fru == NULL) {
		dbg("bug: resource without fru?");
		return SA_ERR_HPI_CAPABILITY;
	}
	fru = ohoi_res_info->fru;
	g_mutex_lock(fru->mutex);
	if ((areatype == SAHPI_IDR_AREATYPE_UNSPECIFIED) &&
	    (areaid == SAHPI_FIRST_ENTRY)) {
		for (tmp_id = OHOI_AREA_FIRST_ID;
				tmp_id <= OHOI_AREA_LAST_ID; tmp_id++) {
			if (get_area_presence(fru,
					areas[tmp_id - 1].areatype)) {
				break;
			}
		}
		if (tmp_id > OHOI_AREA_LAST_ID) {
			dbg("tmp_id > OHOI_AREA_LAST_ID");
			g_mutex_unlock(fru->mutex);
			return SA_ERR_HPI_NOT_PRESENT;
		}
		areaid = tmp_id;
	} else if ((areatype != SAHPI_IDR_AREATYPE_UNSPECIFIED) &&
	    (areaid == SAHPI_FIRST_ENTRY)) {
		areaid = get_first_area(areatype);
		if (areaid == OHOI_AREA_EMPTY_ID) {
			dbg("areaid == OHOI_AREA_EMPTY_ID");
			g_mutex_unlock(fru->mutex);
			return SA_ERR_HPI_NOT_PRESENT;
		}
	} else if ((areatype == SAHPI_IDR_AREATYPE_UNSPECIFIED) &&
	    (areaid != SAHPI_FIRST_ENTRY)) {
		if (areaid > OHOI_AREA_LAST_ID) {
			dbg("areaid(%d) > OHOI_AREA_LAST_ID(%d)",
					areaid, OHOI_AREA_LAST_ID);
			g_mutex_unlock(fru->mutex);
			return SA_ERR_HPI_NOT_PRESENT;
		}
	} else if ((areatype != SAHPI_IDR_AREATYPE_UNSPECIFIED) && 
	    (areaid != SAHPI_FIRST_ENTRY)) {
		if (areaid > OHOI_AREA_LAST_ID) {
			dbg("areaid(%d) > OHOI_AREA_LAST_ID(%d)",
					areaid, OHOI_AREA_LAST_ID);
			g_mutex_unlock(fru->mutex);
			return SA_ERR_HPI_NOT_PRESENT;
		}
	}
	
	if (areatype != SAHPI_IDR_AREATYPE_UNSPECIFIED) {
		if (areas[areaid - 1].areatype != areatype) {
			dbg("areas[areaid - 1].areatype(%d) != areatype(%d)",
				areas[areaid - 1].areatype, areatype);
			g_mutex_unlock(fru->mutex);
			return SA_ERR_HPI_INVALID_PARAMS;
		}
		if (!get_area_presence(fru, areatype)) {
			dbg("area %d not present", areatype);
			g_mutex_unlock(fru->mutex);
			return SA_ERR_HPI_NOT_PRESENT;
		}
		*nextareaid = SAHPI_LAST_ENTRY;
	} else {
		for (tmp_id = areaid + 1; tmp_id <= OHOI_AREA_LAST_ID; tmp_id++) {
			if (get_area_presence(ohoi_res_info->fru,
					areas[tmp_id - 1].areatype)) {
				break;
			}
		}
		if (tmp_id > OHOI_AREA_LAST_ID) {
			*nextareaid = SAHPI_LAST_ENTRY;
		} else {
			*nextareaid = tmp_id;
		}
	}

	area = &areas[areaid - 1];

	header->AreaId = areaid;
	header->Type = area->areatype;

	header->ReadOnly = (area->areatype == SAHPI_IDR_AREATYPE_INTERNAL_USE) ?
						SAHPI_TRUE : SAHPI_FALSE;
	header->NumFields = get_num_fields(ohoi_res_info->fru, area->areatype);
	g_mutex_unlock (fru->mutex);
	return SA_OK;
}

static int try_to_alloc_room_for_area(ipmi_fru_t *fru,
				      SaHpiIdrAreaTypeT areatype,
				      unsigned int *off)
{
	struct ohoi_fru_layout layout;
	unsigned int len;
	unsigned int ipmi_atype;
	unsigned int i;
	unsigned int beg = 8; // common header length
	unsigned int end;
	struct ohoi_area_layout *al;
	int r;
	
	len = areas[get_areaid_by_type(areatype) - 1].empty_len;

	if (get_fru_layout(fru, &layout)) {
		dbg("could not get fru layout");
		return 1;
	}
	print_fru_layout("Initial layout", &layout);
	if (len > layout.free_len) {
		al = &layout.areas[IPMI_FRU_FTR_MULTI_RECORD_AREA];
		if (al->off != 0 && (len <= layout.free_len + al->len -
				((al->used_len +7)&~7))) {
			dbg("Decrease len of OEM_AREA from %d to %d", al->len,
				(al->used_len +7)&~7);
			/*
			r = ipmi_fru_area_set_length(fru,
				IPMI_FRU_FTR_MULTI_RECORD_AREA,
				(al->used_len +7)&~7);
			if (r != 0) {
				dbg("ipmi_fru_area_set_length() returned %d", r);
				return 1;
			}
			*/
			layout.free_len += al->len - ((al->used_len +7)&~7);
			al->len = (al->used_len +7)&~7;
		print_fru_layout("After truncate OEM AREA layout", &layout);
		}
	}
	
	if (len > layout.free_len) {
		dbg("len(%d) < layout->free_len(%d)", len, layout.free_len);
		return 1;
	}
	ipmi_atype = get_ipmi_areatype(areatype);
	for (i = 0; i < ipmi_atype; i++) {
		al = &layout.areas[i];
		if (al->off == 0) {
			continue;
		}
		if (beg < al->off) {
			r = ipmi_fru_area_set_offset(fru, i, beg);
			if (r != 0) {
				dbg("ipmi_fru_area_set_offset for area %d = %d",
					i, r);
				return 1;
			}
			al->off = beg;
			beg += al->len;
		}
	}
	end =layout.len - 8; 
	if ((ipmi_atype + 1) == IPMI_FRU_FTR_NUMBER) {
		if (beg + len <= end) {
			*off = beg;
			print_fru_layout("After moving above areas layout", &layout);
			return 0;
		} else {
			dbg("STRANGE. There is still not enough room.");
			return 1;
		}
	}
	for (i = IPMI_FRU_FTR_NUMBER - 1; i > ipmi_atype; i--) {
		al = &layout.areas[i];
		if (al->off == 0) {
			continue;
		}
		if (al->off + al->len < end) {
		r = ipmi_fru_area_set_offset(fru, i, end - al->len);
			if (r != 0) {
				dbg("ipmi_fru_area_set_offset to %d for area %d = %d",
					end - al->len, i, r);
				return 1;
			}
		}
		al->off = end - al->len;
		end = al->off;
	}
	print_fru_layout("Result Layout", &layout);
	if (beg + len > end) {
		dbg("STRANGE. There is still not enough room.");
		return 1;
	}
	*off = beg;
	return 0;
}



                       /**************   ADD AREA ************/
		       
struct ohoi_add_area {
	struct ohoi_resource_info   *res_info;
	struct oh_handler_state  *handler;
	SaHpiIdrAreaTypeT areatype;
	int done;
	SaErrorT rv;
};


static void add_idr_area_cb(ipmi_entity_t *ent, void *cb_data)
{
	struct ohoi_add_area *ar_add = cb_data;
	ipmi_fru_t *fru;
	unsigned int off;
	struct ohoi_fru_write fw;
	int r;


	fru = ipmi_entity_get_fru(ent);
	ar_add->done = 1;
	if (try_to_alloc_room_for_area(fru, ar_add->areatype, &off)) {
		ar_add->rv = SA_ERR_HPI_OUT_OF_SPACE;
		return;
	}
	r = ipmi_fru_add_area(fru, get_ipmi_areatype(ar_add->areatype), off,
		areas[get_areaid_by_type(ar_add->areatype) - 1].empty_len);
	if (r) {
		dbg("ipmi_fru_add_area return %d", r);
		ar_add->rv = SA_ERR_HPI_INTERNAL_ERROR;
		return;
	}
	if (((struct ohoi_handler *)ar_add->handler->data)->real_write_fru) {
		fw.done = 0;
		fw.rv = SA_OK;
		r = ipmi_fru_write(ipmi_entity_get_fru(ent),
					fru_write_done_cb, &fw);
		if (r) {
			dbg("ipmi_fru_write failed. return = %d", r);
			ar_add->rv = SA_ERR_HPI_INTERNAL_ERROR;
		} else {
			ar_add->rv = ohoi_loop(&fw.done, ar_add->handler->data);
		}
	}
}



SaErrorT ohoi_add_idr_area(void *hnd, SaHpiResourceIdT rid, SaHpiIdrIdT idrid,
                           SaHpiIdrAreaTypeT areatype, SaHpiEntryIdT *areaid)
{
	struct oh_handler_state  *handler = hnd;
	struct ohoi_resource_info   *ohoi_res_info;
	struct ohoi_inventory_info *fru;
	int rv;
	SaErrorT ret;
	struct ohoi_add_area ar_add;
	
	OHOI_CHECK_RPT_CAP_IDR();
	ohoi_res_info = oh_get_resource_data(handler->rptcache, rid);
	if (ohoi_res_info->fru == NULL) {
		dbg("bug: resource without fru?");
		return SA_ERR_HPI_CAPABILITY;
	}
	fru = ohoi_res_info->fru;
	g_mutex_lock(fru->mutex);
	if (get_area_presence(fru, areatype)) {
		g_mutex_unlock(fru->mutex);
		return SA_ERR_HPI_INVALID_DATA;
	}
	ar_add.res_info = ohoi_res_info;
	ar_add.handler = handler;
	ar_add.areatype = areatype;
	ar_add.done = 0;
	ar_add.rv = SA_OK;
	rv = ipmi_entity_pointer_cb(ohoi_res_info->u.entity_id,
		add_idr_area_cb, &ar_add);
	if (rv) {
		dbg("ipmi_entity_pointer_cb returned %d", rv);
		g_mutex_unlock (fru->mutex);
		ar_add.rv = SA_ERR_HPI_INTERNAL_ERROR;
	} else {
		ret = ohoi_loop(&ar_add.done, handler->data);
		if (ret != SA_OK) {
			ar_add.rv = ret;
		}
	}
	if (ar_add.rv != SA_OK) {
		dbg("ohoi_del_idr_field failed. rv = %d", ar_add.rv);
	} else {
		set_area_presence(fru, areatype);
		ohoi_res_info->fru->update_count++;
	}
	g_mutex_unlock (fru->mutex);

	return ar_add.rv;
}



                    /**************  DELETE AREA  ******************/


struct ohoi_del_area {
	SaHpiIdrAreaTypeT areatype;
	struct oh_handler_state  *handler;
	int done;
	SaErrorT rv;
};


static void del_idr_area_cb(ipmi_entity_t *ent, void *cb_data)
{
	struct ohoi_del_area *ar_del = cb_data;
	ipmi_fru_t *fru;
	struct ohoi_fru_write fw;
	int r;


	fru = ipmi_entity_get_fru(ent);
	ar_del->done = 1;
	r = ipmi_fru_delete_area(fru, get_ipmi_areatype(ar_del->areatype));
	if (r) {
		dbg("ipmi_fru_del_area return %d", r);
		ar_del->rv = SA_ERR_HPI_INTERNAL_ERROR;
		return;
	}
	if (((struct ohoi_handler *)ar_del->handler->data)->real_write_fru) {
		fw.done = 0;
		fw.rv = SA_OK;
		r = ipmi_fru_write(ipmi_entity_get_fru(ent),
					fru_write_done_cb, &fw);
		if (r) {
			dbg("ipmi_fru_write failed. return = %d", r);
			ar_del->rv = SA_ERR_HPI_INTERNAL_ERROR;
		} else {
			ar_del->rv = ohoi_loop(&fw.done, ar_del->handler->data);
		}
	}
}


SaErrorT ohoi_del_idr_area(void *hnd, SaHpiResourceIdT rid, SaHpiIdrIdT idrid,
                           SaHpiEntryIdT areaid)
{	
	struct oh_handler_state  *handler = hnd;
	struct ohoi_resource_info   *ohoi_res_info;
	struct ohoi_inventory_info *fru;
	int rv;
	SaErrorT ret;
	struct ohoi_del_area ar_del;
	SaHpiIdrAreaTypeT areatype;
	
	
	OHOI_CHECK_RPT_CAP_IDR();
	ohoi_res_info = oh_get_resource_data(handler->rptcache, rid);
	if (ohoi_res_info->fru == NULL) {
		dbg("bug: resource without fru?");
		return SA_ERR_HPI_CAPABILITY;
	}
	areatype = get_areatype_by_id(areaid);
	if (areatype == OHOI_AREA_EMPTY_ID) {
		return SA_ERR_HPI_INVALID_PARAMS;
	}
	if (areatype == SAHPI_IDR_AREATYPE_INTERNAL_USE) {
		return SA_ERR_HPI_READ_ONLY;
	}
	fru = ohoi_res_info->fru;
	g_mutex_lock(fru->mutex);
	if (!get_area_presence(fru, areatype)) {
		g_mutex_unlock(fru->mutex);
		return SA_ERR_HPI_NOT_PRESENT;
	}
	ar_del.areatype = areatype;
	ar_del.handler = handler;
	ar_del.done = 0;
	ar_del.rv = SA_OK;
	rv = ipmi_entity_pointer_cb(ohoi_res_info->u.entity_id,
		del_idr_area_cb, &ar_del);
	if (rv) {
		dbg("ipmi_entity_pointer_cb returned %d", rv);
		g_mutex_unlock (fru->mutex);
		ar_del.rv = SA_ERR_HPI_INTERNAL_ERROR;
	} else {
		ret = ohoi_loop(&ar_del.done, handler->data);
		if (ret != SA_OK) {
			ar_del.rv = ret;
		}
	}
	if (ar_del.rv != SA_OK) {
		dbg("ohoi_del_idr_field failed. rv = %d", ar_del.rv);
	} else {
		unset_area_presence(fru, areatype);
		ohoi_res_info->fru->update_count++;
	}
	g_mutex_unlock (fru->mutex);

	return ar_del.rv;	
}




                /****************   GET FIELD ****************/


static  SaHpiEntryIdT get_nextfield(struct ohoi_inventory_info *i_info,
				    struct ohoi_area_data  *area_data,
				    SaHpiEntryIdT fieldid)
{
	unsigned int msk;
	unsigned int num;
	SaHpiEntryIdT i;

	switch (area_data->areatype) {
	case SAHPI_IDR_AREATYPE_INTERNAL_USE:
		return SAHPI_LAST_ENTRY;
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:
		msk = i_info->ci_fld_msk;
		num = i_info->ci_custom_num;
		break;
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		msk = i_info->bi_fld_msk;
		num = i_info->bi_custom_num;
		break;
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		msk = i_info->pi_fld_msk;
		num = i_info->pi_custom_num;
		break;
	case SAHPI_IDR_AREATYPE_OEM:
		msk = 0;
		num = 0;
		break;
	default:
		dbg("bug: wrong areatype %x", area_data->areatype);
		return SA_ERR_HPI_INTERNAL_ERROR;
	}
	
	dbg("area = %x; fieldid = %d; msk = %x; num = %x", area_data->areatype,
			fieldid, msk, num);

	for (i = 1; fieldid + i - 1 < OHOI_FIELD_LAST_ID(area_data); i++) {
		if (area_data->fields[fieldid + i - 1].fieldtype ==
				SAHPI_IDR_FIELDTYPE_CUSTOM) {
			break;
		}
		if ((1 << (area_data->fields[fieldid + i - 1].fieldtype)) & msk) {
			dbg("return %d for not custom field %d",
				fieldid + i,
				area_data->fields[fieldid + i - 1].fieldtype);
			return fieldid + i;
		}
	}
	if (((1 << SAHPI_IDR_FIELDTYPE_CUSTOM) & msk) && 
		(fieldid + 1 < OHOI_FIELD_LAST_ID(area_data) + num)) {
		dbg("return %d for custom field", fieldid + 1);
		return fieldid + 1;
	}
	dbg("return SAHPI_LAST_ENTRY");
	return SAHPI_LAST_ENTRY;
}
			
	

SaErrorT ohoi_get_idr_field(void *hnd, SaHpiResourceIdT rid, SaHpiIdrIdT idrid,
		            SaHpiEntryIdT areaid, SaHpiIdrFieldTypeT fieldtype,
                            SaHpiEntryIdT fieldid, SaHpiEntryIdT *nextfieldid,
                            SaHpiIdrFieldT *field)
{
	struct oh_handler_state  *handler = hnd;
	struct ohoi_resource_info   *ohoi_res_info;
	ipmi_entity_id_t         ent_id;
	struct ohoi_field_data   *field_data;
	struct ohoi_area_data  *area_data;
	struct ohoi_get_field gf;
	struct ohoi_inventory_info *fru;
	int rv;
	SaErrorT ret;

	OHOI_CHECK_RPT_CAP_IDR();

	ohoi_res_info = oh_get_resource_data(handler->rptcache, rid);
	if (ohoi_res_info->type != OHOI_RESOURCE_ENTITY) {
		dbg("Bug: try to get fru in unsupported resource");
		return SA_ERR_HPI_INVALID_CMD;
	}
	fru = ohoi_res_info->fru;
	if (fru == NULL) {
		dbg("bug: resource without fru?");
		return SA_ERR_HPI_CAPABILITY;
	}
	ent_id = ohoi_res_info->u.entity_id;
	
	if (areaid == SAHPI_FIRST_ENTRY)
		areaid = OHOI_AREA_FIRST_ID;

	if (areaid > OHOI_AREA_LAST_ID) {
		dbg("areaid > OHOI_AREA_LAST_ID");
		return SA_ERR_HPI_NOT_PRESENT;
	}

	area_data = &areas[areaid - 1];
	field->AreaId = areaid;
	
	g_mutex_lock(fru->mutex);	
	if (area_data->areatype == SAHPI_IDR_AREATYPE_OEM) {
		// oem area is handled by special alghorithm
		ret = get_oem_idr_field(handler, ohoi_res_info,
				fieldtype, fieldid, nextfieldid, field);
		g_mutex_unlock(fru->mutex);
		return ret;
	}

	if ((fieldtype == SAHPI_IDR_FIELDTYPE_UNSPECIFIED) &&
	    (fieldid == SAHPI_FIRST_ENTRY)) {
		fieldid = get_first_field(area_data, fru);	
		if (fieldid == OHOI_FIELD_EMPTY_ID) {
			dbg("fieldid == OHOI_FIELD_EMPTY_ID");
			g_mutex_unlock(fru->mutex);
			return SA_ERR_HPI_NOT_PRESENT;
		}
	} else if ((fieldtype != SAHPI_IDR_FIELDTYPE_UNSPECIFIED) &&
					(fieldid == SAHPI_FIRST_ENTRY)) {
		fieldid = get_first_typed_field(area_data, fieldtype, fru);
		if (fieldid == OHOI_FIELD_EMPTY_ID) {
			dbg("fieldid == OHOI_FIELD_EMPTY_ID");
			g_mutex_unlock(fru->mutex);
			return SA_ERR_HPI_NOT_PRESENT;
		}
	} else if ((fieldtype == SAHPI_IDR_FIELDTYPE_UNSPECIFIED) &&
					(fieldid != SAHPI_FIRST_ENTRY)) {
		if (fieldid >= OHOI_FIELD_LAST_ID(area_data)) {
			dbg("area %x; fieldid(%d) >= "
			    "OHOI_FIELD_LAST_ID(area_data)(%d)",
				area_data->areatype, fieldid,
				OHOI_FIELD_LAST_ID(area_data));
				field->FieldId = fieldid;
				field->Type = SAHPI_IDR_FIELDTYPE_CUSTOM;
				field->ReadOnly = area_data->read_only;
				ret = get_custom_field(handler, ohoi_res_info,
						OHOI_FIELD_LAST_ID(area_data),
						fieldid, nextfieldid, field);
				g_mutex_unlock(fru->mutex);
				return ret;
		}	
	} else if ((fieldtype != SAHPI_IDR_FIELDTYPE_UNSPECIFIED) && 
					(fieldid != SAHPI_FIRST_ENTRY)) {
		if (fieldid >= OHOI_FIELD_LAST_ID(area_data)) {
			if (fieldtype != SAHPI_IDR_FIELDTYPE_CUSTOM) {
				dbg("fieldtype(%d) != SAHPI_IDR_FIELDTYPE_CUSTOM",
					fieldtype);
				g_mutex_unlock(fru->mutex);
				return SA_ERR_HPI_NOT_PRESENT;
			}
			ret = get_custom_field(handler, ohoi_res_info,
				fieldid - OHOI_FIELD_LAST_ID(area_data),
				fieldid, nextfieldid, field);
			g_mutex_unlock(fru->mutex);
			return ret;
		}
		if (area_data->fields[fieldid - 1].fieldtype != fieldtype) {
			dbg("area_data->fields[fieldid - 1].fieldtype != fieldtype(%d != %d)",
				area_data->fields[fieldid - 1].fieldtype, fieldtype);
			g_mutex_unlock(fru->mutex);
			return SA_ERR_HPI_INVALID_PARAMS;
		}
	}

	field_data = &area_data->fields[fieldid - 1];

	if (area_data->areatype == SAHPI_IDR_AREATYPE_OEM) {
		if (fieldid < ohoi_res_info->fru->oem_fields_num - 1) {
			*nextfieldid = fieldid + 1;
		} else {
			*nextfieldid = SAHPI_LAST_ENTRY;
		}
	} else {
		*nextfieldid = get_nextfield(ohoi_res_info->fru,
			area_data, fieldid);
	}

	field->FieldId = fieldid;
	field->Type = field_data->fieldtype;

	gf.field = field;
	gf.data = field_data;
	gf.rv = SA_OK;
	gf.done = 0;

	rv = ipmi_entity_pointer_cb(ent_id, get_field, &gf);
	if (rv) {
		g_mutex_unlock(fru->mutex);
		return SA_ERR_HPI_INTERNAL_ERROR;
	}
	gf.rv = ohoi_loop(&gf.done, handler->data);
	if (gf.rv != SA_OK) {
		g_mutex_unlock(fru->mutex);
		return gf.rv;
	}
	field->ReadOnly = area_data->read_only;
	if (field_data->lang == SAHPI_LANG_UNDEF) {
		field->Field.Language = get_language(ohoi_res_info->fru,
						area_data->areatype);
	} else {
		field->Field.Language = field_data->lang;
	}
	g_mutex_unlock (fru->mutex);
	return SA_OK;
}



               /*************   MODIFYING (ADD, SET, DELETE)  FIELD  **************/


static SaErrorT modify_inventory(SaHpiIdrFieldT *field, ipmi_entity_t *ent,
					struct ohoi_resource_info *res_info);

struct ohoi_mod_field {
	SaHpiIdrFieldT *field;
	struct ohoi_resource_info *res_info;
	struct oh_handler_state  *hnd;
	SaErrorT rv;
	int done;
};

struct ohoi_del_field {
	SaHpiEntryIdT fieldid;
	SaHpiEntryIdT areaid;
	struct ohoi_resource_info *res_info;
	struct oh_handler_state  *hnd;
	SaErrorT rv;
	int done;
};



static void modify_inventoty_field_cb(ipmi_entity_t *ent, void *cbdata)
{
	struct ohoi_mod_field *mf = cbdata;
	int rv;
	struct ohoi_fru_write fw;
	
	mf->rv = modify_inventory(mf->field, ent, mf->res_info);
	mf->done = 1;
	if (mf->rv != SA_OK) {
		dbg("modify_inventory failed. return %d", mf->rv);
		return;
	}
	if (((struct ohoi_handler *)mf->hnd->data)->real_write_fru) {
		fw.done = 0;
		fw.rv = SA_OK;
		rv = ipmi_fru_write(ipmi_entity_get_fru(ent),
					fru_write_done_cb, &fw);
		if (rv) {
			dbg("ipmi_fru_write failed. return = %d", rv);
			mf->rv = SA_ERR_HPI_INTERNAL_ERROR;
		} else {
			mf->rv = ohoi_loop(&fw.done, mf->hnd->data);
		}
	}
}




                /*************  SET FIELD *************/


SaErrorT ohoi_set_idr_field(void *hnd, SaHpiResourceIdT rid, SaHpiIdrIdT idrid,
                            SaHpiIdrFieldT *field)
{
	struct oh_handler_state  *handler = hnd;
	struct ohoi_resource_info   *ohoi_res_info;
	struct ohoi_inventory_info *fru;
	struct ohoi_mod_field mf;
	int rv;
	SaErrorT ret;

	OHOI_CHECK_RPT_CAP_IDR();

	ohoi_res_info = oh_get_resource_data(handler->rptcache, rid);
	if (ohoi_res_info->type != OHOI_RESOURCE_ENTITY) {
		dbg("Bug: try to get fru in unsupported resource");
		return SA_ERR_HPI_INVALID_CMD;
	}
	fru = ohoi_res_info->fru;
	if (fru == NULL) {
		dbg("bug: resource without fru?");
		return SA_ERR_HPI_CAPABILITY;
	}
	
	mf.field = field;
	mf.res_info = ohoi_res_info;
	mf.hnd = hnd;
	mf.rv = SA_OK;
	mf.done = 0;

	g_mutex_lock(fru->mutex);
	rv = ipmi_entity_pointer_cb(ohoi_res_info->u.entity_id,
					modify_inventoty_field_cb, &mf);
	if (rv) {
		dbg("ipmi_entity_pointer_cb returned %d", rv);
		g_mutex_unlock (fru->mutex);
		mf.rv = SA_ERR_HPI_INTERNAL_ERROR;
	} else {
		ret = ohoi_loop(&mf.done, handler->data);
		if (ret != SA_OK) {
			mf.rv = ret;
		}
	}
	if (mf.rv != SA_OK) {
		dbg("ohoi_set_idr_field failed. rv = %d", mf.rv);
	} else {
		ohoi_res_info->fru->update_count++;
	}
	g_mutex_unlock (fru->mutex);
	return mf.rv; 

}


                        /******    ADD FIELD   ******/

SaErrorT ohoi_add_idr_field(void *hnd, SaHpiResourceIdT rid, SaHpiIdrIdT idrid,
                            SaHpiIdrFieldT *field)
{
	SaHpiEntryIdT fid;

	fid = get_fieldid_by_type(field->AreaId, field->Type);
	if (fid == 0) {
		dbg("invalid field type %d", field->Type);
		return SA_ERR_HPI_INVALID_PARAMS;
	}
	field->FieldId = fid;
	return ohoi_set_idr_field(hnd, rid, idrid, field);
}


                /************   DELETE FIELD  ************/

static void delete_inventoty_field_cb(ipmi_entity_t *ent, void *cbdata)
{
	struct ohoi_del_field *df = cbdata;
	int rv;
	struct ohoi_fru_write fw;
	SaHpiIdrFieldT field;
	
	field.FieldId = df->fieldid;
	field.AreaId = df->areaid;
	field.Type = areas[df->areaid - 1].fields[df->fieldid - 1].fieldtype;
	field.Field.DataLength = 0;
	field.Field.DataType = SAHPI_TL_TYPE_BINARY;
	
	df->rv = modify_inventory(&field, ent, df->res_info);
	df->done = 1;
	if (df->rv != SA_OK) {
		dbg("modify_inventory failed. return %d", df->rv);
		return;
	}
	if (((struct ohoi_handler *)df->hnd->data)->real_write_fru) {
		fw.done = 0;
		fw.rv = SA_OK;
		rv = ipmi_fru_write(ipmi_entity_get_fru(ent),
					fru_write_done_cb, &fw);
		if (rv) {
			dbg("ipmi_fru_write failed. return = %d", rv);
			df->rv = SA_ERR_HPI_INTERNAL_ERROR;
		} else {
			df->rv = ohoi_loop(&fw.done, df->hnd->data);
		}
	}
}

SaErrorT ohoi_del_idr_field(void *hnd, SaHpiResourceIdT rid, SaHpiIdrIdT idrid,
                            SaHpiEntryIdT areaid, SaHpiEntryIdT fieldid)
{
	struct oh_handler_state  *handler = hnd;
	struct ohoi_resource_info   *ohoi_res_info;
	struct ohoi_inventory_info *fru;
	struct ohoi_del_field df;
	int rv;
	SaErrorT ret;

	OHOI_CHECK_RPT_CAP_IDR();

	ohoi_res_info = oh_get_resource_data(handler->rptcache, rid);
	if (ohoi_res_info->type != OHOI_RESOURCE_ENTITY) {
		dbg("Bug: try to get fru in unsupported resource");
		return SA_ERR_HPI_INVALID_CMD;
	}
	fru = ohoi_res_info->fru;
	if (fru == NULL) {
		dbg("bug: resource without fru?");
		return SA_ERR_HPI_CAPABILITY;
	}
	if (areaid > OHOI_AREA_LAST_ID || areaid < 1) {
		dbg("areaid(%d) > OHOI_AREA_LAST_ID(%d) || areaid < 1",
			areaid, OHOI_AREA_LAST_ID);
		return SA_ERR_HPI_NOT_PRESENT;
	}
	if (!get_area_presence(fru, areas[areaid - 1].areatype)) {
		return SA_ERR_HPI_NOT_PRESENT;
	}
	df.fieldid = fieldid;
	df.areaid = areaid;
	df.res_info = ohoi_res_info;
	df.hnd = hnd;
	df.rv = SA_OK;
	df.done = 0;

	g_mutex_lock(fru->mutex);
	rv = ipmi_entity_pointer_cb(ohoi_res_info->u.entity_id,
					delete_inventoty_field_cb, &df);
	if (rv) {
		dbg("ipmi_entity_pointer_cb returned %d", rv);
		g_mutex_unlock (fru->mutex);
		df.rv = SA_ERR_HPI_INTERNAL_ERROR;
	} else {
		ret = ohoi_loop(&df.done, handler->data);
		if (ret != SA_OK) {
			df.rv = ret;
		}
	}
	if (df.rv != SA_OK) {
		dbg("ohoi_del_idr_field failed. rv = %d", df.rv);
	} else {
		ohoi_res_info->fru->update_count++;
	}
	g_mutex_unlock (fru->mutex);
	return df.rv; 

}
 
  

static int try_to_change_area_size(ipmi_fru_t *fru, unsigned int delta,
				   unsigned int my_a_type)
{
	unsigned int fru_len;
	unsigned int sum_len = 0;
	unsigned int len;
	unsigned int my_len = 0;
	unsigned int i;
	unsigned int off;
	int rv;
	
	dbg("enter: delta = %u, area = %u",
			delta, my_a_type);
	// calculate sum of all area sizes
	for (i = 0; i < IPMI_FRU_FTR_NUMBER; i++) {
		rv = ipmi_fru_area_get_length(fru, i, &len);
		if (rv == 0) {
			sum_len += len;
			if (i == my_a_type) {
				my_len = len;
			}
		}
	}
	fru_len = ipmi_fru_get_data_length(fru);

	// must be multiple 8
	delta = ((delta + 7) >> 3) << 3;
	dbg("fru_len = %u; sum_len = %u; new_delta = %u", fru_len, sum_len, delta);
	if (sum_len + delta > fru_len) {
		dbg("not enough space. Do nothing");
		return 1;
	}
	for (i = IPMI_FRU_FTR_NUMBER - 1; i != my_a_type; i--) {
		// move all below areas down to free space
		rv = ipmi_fru_area_get_offset(fru, i, &off);
		if (rv) {
			dbg("no area %u in fru. coontinue", i);
			continue;
		}
		dbg("offset of area %u is changing from %u to %u",
						i, off, off + delta);
		rv = ipmi_fru_area_set_offset(fru, i, off + delta);
		if (rv) {
			dbg("could not chang offset");
			return 1;
		}
	}
	rv = ipmi_fru_area_set_length(fru, my_a_type, my_len + delta);
	dbg("change size of area %d from %u to %u. res = %d",
		my_a_type, my_len, my_len + delta, rv);
	return rv;
}
	
	  
static SaErrorT modify_inventory(SaHpiIdrFieldT *field,
				ipmi_entity_t *ent,
				struct ohoi_resource_info   *ohoi_res_info)
{
	struct ohoi_inventory_info *i_info = ohoi_res_info->fru;
	SaHpiEntryIdT areaid = field->AreaId;
	SaHpiEntryIdT fieldid = field->FieldId;
	SaHpiTextBufferT *tb = &field->Field;
	SaHpiIdrFieldTypeT f_type;
	SaHpiIdrAreaTypeT  a_type;
	struct ohoi_area_data  *area_data;
	enum ipmi_str_type_e type;
	unsigned char lang;
	unsigned int ipmi_a_type = IPMI_FRU_FTR_NUMBER;
	SaErrorT ret = SA_OK;
	ipmi_fru_t *fru;
	int rv = 0;
	int (* set_func)(ipmi_fru_t   *fru,
			enum ipmi_str_type_e type,
			char         *str,
			unsigned int len) = NULL;
	int (* len_func)(ipmi_fru_t *fru, unsigned int *length) = NULL;


	if (areaid > OHOI_AREA_LAST_ID || areaid < 1) {
		dbg("areaid(%d) > OHOI_AREA_LAST_ID(%d) && areaid < 1",
			areaid, OHOI_AREA_LAST_ID);
		return SA_ERR_HPI_NOT_PRESENT;
	}
	area_data = &areas[areaid - 1];
	a_type = area_data->areatype;
	if (fieldid > OHOI_FIELD_LAST_ID(area_data)) {
		dbg("fieldid(%d) >= OHOI_FIELD_LAST_ID(area_data)(%d)",
			fieldid, OHOI_FIELD_LAST_ID(area_data));
		return SA_ERR_HPI_NOT_PRESENT;
	}
		
	f_type = area_data->fields[fieldid - 1].fieldtype;
	dbg("modify_inventory: area = 0x%x; field = %i", a_type, f_type);
	if (i_info == NULL) {
		dbg("Bug: ohoi_res_info->fru == NULL");
		return SA_ERR_HPI_CAPABILITY;
	}
	fru = ipmi_entity_get_fru(ent);
	if (fru == NULL) {
		dbg("Bug: resource without fru");
		return SA_ERR_HPI_INTERNAL_ERROR;
	}
	switch (a_type) {
	case SAHPI_IDR_AREATYPE_INTERNAL_USE:
		return SA_ERR_HPI_READ_ONLY;
	case SAHPI_IDR_AREATYPE_CHASSIS_INFO:
		if (i_info->ci == 0) {
			dbg("CHASSIS_INFO area not present");
			ret = SA_ERR_HPI_NOT_PRESENT;
			goto out;
		}
		ipmi_a_type = IPMI_FRU_FTR_CHASSIS_INFO_AREA;
		switch (f_type) {
		case SAHPI_IDR_FIELDTYPE_CHASSIS_TYPE:
			if ((tb->DataType != SAHPI_TL_TYPE_BINARY) ||
			    ((tb->DataLength != 1) && (tb->DataLength != 0))) {
				dbg("CHASSIS_TYPE: DataType = %d; "
					"DataLength = %d",
					tb->DataType, tb->DataLength);
				return SA_ERR_HPI_INVALID_DATA;
			}
			rv = ipmi_fru_set_chassis_info_type(fru,
				*(unsigned char *)tb->Data);
			break;
		case SAHPI_IDR_FIELDTYPE_PART_NUMBER:
			lang = 0;
			set_func = ipmi_fru_set_chassis_info_part_number;
			len_func = ipmi_fru_get_chassis_info_part_number_len;
			break;
		case SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER:
			lang = SAHPI_LANG_ENGLISH;
			set_func = ipmi_fru_set_chassis_info_serial_number;
			len_func = ipmi_fru_get_chassis_info_serial_number_len;
			break;
		default:
			dbg("CHASSIS_INFO: field %d not present", fieldid);
			ret = SA_ERR_HPI_NOT_PRESENT;
			goto out;
		}
		break;
	case SAHPI_IDR_AREATYPE_BOARD_INFO:
		if (i_info->bi == 0) {
			dbg("BOARD_INFO area not present");
			return SA_ERR_HPI_NOT_PRESENT;
		}
		ipmi_a_type = IPMI_FRU_FTR_BOARD_INFO_AREA;
		lang = i_info->bi;
		switch (f_type) {
		case SAHPI_IDR_FIELDTYPE_MFG_DATETIME:
			if ((tb->DataType != SAHPI_TL_TYPE_BINARY) ||
					((tb->DataLength != sizeof(time_t)) &&
					(tb->DataLength != 0))) {
				dbg("BOARD_INFO/MFG_DATETIME: DataType = %d; "
					"DataLength = %d",
					tb->DataType, tb->DataLength);	
				return SA_ERR_HPI_INVALID_DATA;
			}
			if (tb->DataLength == 0) {
				rv = ipmi_fru_set_board_info_mfg_time(fru, 0);
			} else {
				rv = ipmi_fru_set_board_info_mfg_time(fru,
					*(time_t *)tb->Data);
			}
			break;
		case SAHPI_IDR_FIELDTYPE_MANUFACTURER:
			set_func = ipmi_fru_set_board_info_board_manufacturer;
			len_func = ipmi_fru_get_board_info_board_manufacturer_len;
			break;
		case SAHPI_IDR_FIELDTYPE_PRODUCT_NAME:
			set_func = ipmi_fru_set_board_info_board_product_name;
			len_func = ipmi_fru_get_board_info_board_product_name_len;
			break;
		case SAHPI_IDR_FIELDTYPE_PART_NUMBER:
			set_func = ipmi_fru_set_board_info_board_part_number;
			len_func = ipmi_fru_get_board_info_board_part_number_len;
			break;
		case SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER:
			lang = SAHPI_LANG_ENGLISH;
			set_func = ipmi_fru_set_board_info_board_serial_number;
			len_func = ipmi_fru_get_board_info_board_serial_number_len;
			break;
		case SAHPI_IDR_FIELDTYPE_FILE_ID:
			lang = SAHPI_LANG_ENGLISH;
			set_func = ipmi_fru_set_board_info_fru_file_id;
			len_func = ipmi_fru_get_board_info_fru_file_id_len;
			break;
		default:
			dbg("BOARD_INFO: field %d not present", fieldid);
			ret = SA_ERR_HPI_NOT_PRESENT;
			goto out;
		}
		break;
	case SAHPI_IDR_AREATYPE_PRODUCT_INFO:
		if (i_info->pi == 0) {
			dbg("PRODUCT_INFO area not present");
			return SA_ERR_HPI_NOT_PRESENT;
		}
		ipmi_a_type = IPMI_FRU_FTR_PRODUCT_INFO_AREA;
		lang = i_info->pi;
		switch (f_type) {
		case SAHPI_IDR_FIELDTYPE_MANUFACTURER:
			set_func = ipmi_fru_set_product_info_manufacturer_name;
			len_func = ipmi_fru_get_product_info_manufacturer_name_len;
			break;
		case SAHPI_IDR_FIELDTYPE_PRODUCT_NAME:
			set_func = ipmi_fru_set_product_info_product_name;
			len_func = ipmi_fru_get_product_info_product_name_len;
			break;
		case SAHPI_IDR_FIELDTYPE_PART_NUMBER:
			set_func = ipmi_fru_set_product_info_product_part_model_number;
			len_func = ipmi_fru_get_product_info_product_part_model_number_len;
			break;
		case SAHPI_IDR_FIELDTYPE_PRODUCT_VERSION:
			set_func = ipmi_fru_set_product_info_product_version;
			len_func = ipmi_fru_get_product_info_product_version_len;
			break;
		case SAHPI_IDR_FIELDTYPE_SERIAL_NUMBER:
			lang = SAHPI_LANG_ENGLISH;
			set_func = ipmi_fru_set_product_info_product_serial_number;
			len_func = ipmi_fru_get_product_info_product_serial_number_len;
			break;
		case SAHPI_IDR_FIELDTYPE_ASSET_TAG:
			set_func = ipmi_fru_set_product_info_asset_tag;
			len_func = ipmi_fru_get_product_info_asset_tag_len;
			break;
		case SAHPI_IDR_FIELDTYPE_FILE_ID:
			set_func = ipmi_fru_set_product_info_fru_file_id;
			len_func = ipmi_fru_get_product_info_fru_file_id_len;
			break;
		default:
			dbg("PRODUCT_INFO: field %d not present", fieldid);
			ret = SA_ERR_HPI_NOT_PRESENT;
			goto out;
		}
		break;
	case SAHPI_IDR_AREATYPE_OEM :
		return SA_ERR_HPI_UNSUPPORTED_API;
	default:
		dbg("Unknown area type = 0x%x", areas[areaid].areatype);
		ret = SA_ERR_HPI_INVALID_PARAMS;
		goto out;
	}

	if (set_func != NULL) {
		ret = get_str_type(tb, lang, &type);
		if (ret != SA_OK) {
			goto out;
		}
		rv = set_func(fru, type, (char *)tb->Data,
					(unsigned int)tb->DataLength);
		if (rv == ENOSPC) {
			// try to increase the room for area moving other areas
			int r;
			unsigned int f_len;
			//print_fru_layout(fru);
			r = len_func(fru, &f_len);
			dbg("rv = %d; f_len = %u; new_len = %u", rv, f_len, tb->DataLength);
			if ((r == 0) && (f_len < (unsigned int)tb->DataLength)
					&& (try_to_change_area_size(fru,
					(unsigned int)tb->DataLength - f_len,
					ipmi_a_type) == 0)) {
				//print_fru_layout(fru);
				rv = set_func(fru, type, (char *)tb->Data,
					(unsigned int)tb->DataLength);
			}
		}
		if (rv) {
			dbg("Could not set FRU field %d of area %d. rv = %d\n",
				fieldid, areaid, rv);
			if (rv == ENOSPC) {
				ret = SA_ERR_HPI_OUT_OF_SPACE;
			} else {
				dbg("set_func for %x/%d returned error = %d",
					a_type, f_type, rv);
				ret = SA_ERR_HPI_INVALID_DATA;
			}
		} 
	}
out:
	return ret;
}

