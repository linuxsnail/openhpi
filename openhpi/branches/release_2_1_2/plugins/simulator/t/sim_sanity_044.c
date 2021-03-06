/*      -*- linux-c -*-
 *
 *(C) Copyright IBM Corp. 2005
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 *	Authors:
 *     	Sean Dague <http://dague.net/sean>
*/

#include <stdlib.h>
#include <SaHpi.h>
#include <oh_utils.h>
#include <sahpi_struct_utils.h>
#include <oh_error.h>


/**
 * Run a series of sanity tests on the simulator
 * Return 0 on success, otherwise return -1
 **/

int main(int argc, char **argv)
{
	SaHpiSessionIdT sid = 0;
        SaHpiEntryIdT newid;
        SaHpiIdrFieldT field;
	SaErrorT rc = SA_OK;

        rc = saHpiSessionOpen(SAHPI_UNSPECIFIED_DOMAIN_ID, &sid, NULL);
	if (rc != SA_OK) {
		dbg("Failed to open session");
                return -1;
	}
	rc = saHpiDiscover(sid);
	if (rc != SA_OK) {
		dbg("Failed to run discover");
                return -1;
	}

        rc = saHpiIdrAreaAdd(sid, 1, 1, SAHPI_IDR_AREATYPE_PRODUCT_INFO,
                             &newid);
        if (rc != SA_OK) {
		dbg("Couldn't add new area");
		dbg("Error %s",oh_lookup_error(rc));
                return -1;
	}

        field.AreaId = newid;
        field.Type = SAHPI_IDR_FIELDTYPE_PART_NUMBER;
        field.Field.DataType = SAHPI_TL_TYPE_TEXT;
        field.Field.Language = SAHPI_LANG_ENGLISH;
        field.Field.DataLength = 6;
        field.Field.Data[0] = '1';
        field.Field.Data[1] = '2';
        field.Field.Data[2] = '3';
        field.Field.Data[3] = '4';
        field.Field.Data[4] = '5';
        field.Field.Data[5] = '6';
        field.Field.Data[6] = '\0';
        rc = saHpiIdrFieldAdd(sid, 1, 1, &field);
        if (rc != SA_OK) {
		dbg("Couldn't add field");
		dbg("Error %s",oh_lookup_error(rc));
                return -1;
	}

        rc = saHpiIdrFieldDelete(sid, 1, 1, newid, field.FieldId);
        if (rc != SA_OK) {
		dbg("Couldn't delete field");
		dbg("Error %s",oh_lookup_error(rc));
                return -1;
	}

        field.AreaId = 1;
        field.FieldId = 1;
        rc = saHpiIdrFieldDelete(sid, 1, 1, 1, 1);
        if (rc == SA_OK) {
		dbg("Able to delete read only field");
                return -1;
	}

	return 0;
}

