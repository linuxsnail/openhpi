/* -*- linux-c -*-
 * 
 * (C) Copyright IBM Corp. 2004
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Author(s):
 *     Steve Sherman <stevees@us.ibm.com>
 */

#include <glib.h>
#include <SaHpi.h>

#include <oh_plugin.h>
#include <rpt_utils.h>
#include <snmp_util.h>
#include <snmp_bc.h>
#include <bc_resources.h>
#include <snmp_bc_utils.h>
#include <snmp_bc_hotswap.h>

#include <tstubs_res.h>
#include <tstubs_snmp.h>
#include <thotswap.h>

int main(int argc, char **argv) 
{

	struct snmp_bc_hnd snmp_handle;
	struct oh_handler_state hnd = {
		.rptcache = (RPTable *)&test_rpt,
		.eventq = NULL,
		.config = NULL,
		.data = (void *)&snmp_handle,
	};

#if 0
	/* Fill in RPT Entry */
	test_rpt.rpt.ResourceTag.DataType = SAHPI_TL_TYPE_LANGUAGE;
	test_rpt.rpt.ResourceTag.Language = SAHPI_LANG_ENGLISH;
	test_rpt.rpt.ResourceTag.DataLength = strlen(test_rpt.comment);
	strcpy(test_rpt.rpt.ResourceTag.Data, test_rpt.comment);
#endif

	SaHpiResourceIdT  id = 1;
	SaHpiResetActionT act;
	SaErrorT          err;
	/* SaHpiResetActionT expected_value; */

	/*********************************
	 * Set SAHPI_RESET_ASSERT TestCase
         *********************************/
	act = SAHPI_RESET_ASSERT;
	err = snmp_bc_set_reset_state((void *)&hnd, id, act);
	if (err != SA_ERR_HPI_INVALID_CMD) { 
		printf("Error! Set SAHPI_RESET_ASSERT TestCase\n");
		printf("Error! snmp_bc_set_reset_state returned err=%d\n", err);
		return -1; 
	}

	/***********************************
	 * Set SAHPI_RESET_DEASSERT TestCase
         ***********************************/
	act = SAHPI_RESET_DEASSERT;
	err = snmp_bc_set_reset_state((void *)&hnd, id, act);
	if (err != SA_ERR_HPI_INVALID_CMD) { 
		printf("Error! Set SAHPI_RESET_DEASSERT TestCase\n");
		printf("Error! snmp_bc_set_reset_state returned err=%d\n", err);
		return -1; 
	}

	/***********************************
	 * Set SAHPI_COLD_RESET TestCase
         ***********************************/
	act = SAHPI_COLD_RESET;
	err = snmp_bc_set_reset_state((void *)&hnd, id, act);
	if (err) { 
		printf("Error! Set SAHPI_COLD_RESET TestCase\n");
		printf("Error! snmp_bc_set_reset_state returned err=%d\n", err);
		return -1; 
	}

	/***********************************
	 * Set SAHPI_WARM_RESET TestCase
         ***********************************/
	act = SAHPI_WARM_RESET;
	err = snmp_bc_set_reset_state((void *)&hnd, id, act);
	if (err) { 
		printf("Error! Set SAHPI_WARM_RESET TestCase\n");
		printf("Error! snmp_bc_set_reset_state returned err=%d\n", err);
		return -1; 
	}

	return 0;
}

/****************
 * Stub Functions
 ****************/
#include <tstubs_res.c>
#include <tstubs_snmp.c>
