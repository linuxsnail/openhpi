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
 *        : mib2c.notify.conf,v 5.2.2.1 2004/04/15 12:29:06 dts12 Exp $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "hpiB0101.h"

static oid snmptrap_oid[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};

int
send_saHpiSensorNotification_trap( void )
{
    netsnmp_variable_list  *var_list = NULL;
    oid saHpiSensorNotification_oid[] = { 1,3,6,1,4,1,18568,2,1,1,5,1 };
    oid saHpiDomainId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,2,1,1, /* insert index here */ };
    oid saHpiResourceId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,8,1,1, /* insert index here */ };
    oid saHpiEventSeverity_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,3,1,3, /* insert index here */ };
    oid saHpiSensorEventType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,12,1,3, /* insert index here */ };
    oid saHpiSensorEventCategory_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,12,1,4, /* insert index here */ };
    oid saHpiSensorEventState_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,12,1,6, /* insert index here */ };
    oid saHpiSensorEventTriggerReadingType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,12,1,8, /* insert index here */ };
    oid saHpiSensorEventTriggerReading_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,12,1,9, /* insert index here */ };
    oid saHpiSensorEventTriggerThresholdType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,12,1,10, /* insert index here */ };
    oid saHpiSensorEventTriggerThreshold_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,12,1,11, /* insert index here */ };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, OID_LENGTH(snmptrap_oid),
        ASN_OBJECT_ID,
        saHpiSensorNotification_oid, sizeof(saHpiSensorNotification_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        saHpiDomainId_oid, OID_LENGTH(saHpiDomainId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiDomainId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceId_oid, OID_LENGTH(saHpiResourceId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiResourceId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiEventSeverity_oid, OID_LENGTH(saHpiEventSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiEventSeverity */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEventType_oid, OID_LENGTH(saHpiSensorEventType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiSensorEventType */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEventCategory_oid, OID_LENGTH(saHpiSensorEventCategory_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiSensorEventCategory */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEventState_oid, OID_LENGTH(saHpiSensorEventState_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for saHpiSensorEventState */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEventTriggerReadingType_oid, OID_LENGTH(saHpiSensorEventTriggerReadingType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiSensorEventTriggerReadingType */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEventTriggerReading_oid, OID_LENGTH(saHpiSensorEventTriggerReading_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for saHpiSensorEventTriggerReading */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEventTriggerThresholdType_oid, OID_LENGTH(saHpiSensorEventTriggerThresholdType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiSensorEventTriggerThresholdType */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEventTriggerThreshold_oid, OID_LENGTH(saHpiSensorEventTriggerThreshold_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for saHpiSensorEventTriggerThreshold */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_saHpiWatchdogNotification_trap( void )
{
    netsnmp_variable_list  *var_list = NULL;
    oid saHpiWatchdogNotification_oid[] = { 1,3,6,1,4,1,18568,2,1,1,5,5 };
    oid saHpiDomainId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,2,1,1, /* insert index here */ };
    oid saHpiResourceId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,8,1,1, /* insert index here */ };
    oid saHpiEventSeverity_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,3,1,3, /* insert index here */ };
    oid saHpiWatchdogEventAction_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,21,1,3, /* insert index here */ };
    oid saHpiWatchdogEventPreTimerAction_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,21,1,4, /* insert index here */ };
    oid saHpiWatchdogEventUse_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,21,1,5, /* insert index here */ };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, OID_LENGTH(snmptrap_oid),
        ASN_OBJECT_ID,
        saHpiWatchdogNotification_oid, sizeof(saHpiWatchdogNotification_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        saHpiDomainId_oid, OID_LENGTH(saHpiDomainId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiDomainId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceId_oid, OID_LENGTH(saHpiResourceId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiResourceId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiEventSeverity_oid, OID_LENGTH(saHpiEventSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiEventSeverity */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiWatchdogEventAction_oid, OID_LENGTH(saHpiWatchdogEventAction_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiWatchdogEventAction */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiWatchdogEventPreTimerAction_oid, OID_LENGTH(saHpiWatchdogEventPreTimerAction_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiWatchdogEventPreTimerAction */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiWatchdogEventUse_oid, OID_LENGTH(saHpiWatchdogEventUse_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiWatchdogEventUse */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_saHpiSensorEnableChangeNotification_trap( void )
{
    netsnmp_variable_list  *var_list = NULL;
    oid saHpiSensorEnableChangeNotification_oid[] = { 1,3,6,1,4,1,18568,2,1,1,5,2 };
    oid saHpiDomainId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,2,1,1, /* insert index here */ };
    oid saHpiResourceId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,8,1,1, /* insert index here */ };
    oid saHpiEventSeverity_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,3,1,3, /* insert index here */ };
    oid saHpiSensorEnableChangeEventType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,15,1,3, /* insert index here */ };
    oid saHpiSensorEnableChangeEventCategory_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,15,1,4, /* insert index here */ };
    oid saHpiSensorEnableChangeEventState_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,15,1,10, /* insert index here */ };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, OID_LENGTH(snmptrap_oid),
        ASN_OBJECT_ID,
        saHpiSensorEnableChangeNotification_oid, sizeof(saHpiSensorEnableChangeNotification_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        saHpiDomainId_oid, OID_LENGTH(saHpiDomainId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiDomainId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceId_oid, OID_LENGTH(saHpiResourceId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiResourceId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiEventSeverity_oid, OID_LENGTH(saHpiEventSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiEventSeverity */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEnableChangeEventType_oid, OID_LENGTH(saHpiSensorEnableChangeEventType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiSensorEnableChangeEventType */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEnableChangeEventCategory_oid, OID_LENGTH(saHpiSensorEnableChangeEventCategory_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiSensorEnableChangeEventCategory */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSensorEnableChangeEventState_oid, OID_LENGTH(saHpiSensorEnableChangeEventState_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for saHpiSensorEnableChangeEventState */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_saHpiUserNotifications_trap( void )
{
    netsnmp_variable_list  *var_list = NULL;
    oid saHpiUserNotifications_oid[] = { 1,3,6,1,4,1,18568,2,1,1,5,9 };
    oid saHpiDomainId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,2,1,1, /* insert index here */ };
    oid saHpiResourceId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,8,1,1, /* insert index here */ };
    oid saHpiEventSeverity_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,3,1,3, /* insert index here */ };
    oid saHpiUserEventTextType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,30,1,3, /* insert index here */ };
    oid saHpiUserEventTextLanguage_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,30,1,4, /* insert index here */ };
    oid saHpiUserEventText_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,30,1,5, /* insert index here */ };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, OID_LENGTH(snmptrap_oid),
        ASN_OBJECT_ID,
        saHpiUserNotifications_oid, sizeof(saHpiUserNotifications_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        saHpiDomainId_oid, OID_LENGTH(saHpiDomainId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiDomainId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceId_oid, OID_LENGTH(saHpiResourceId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiResourceId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiEventSeverity_oid, OID_LENGTH(saHpiEventSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiEventSeverity */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiUserEventTextType_oid, OID_LENGTH(saHpiUserEventTextType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiUserEventTextType */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiUserEventTextLanguage_oid, OID_LENGTH(saHpiUserEventTextLanguage_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiUserEventTextLanguage */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiUserEventText_oid, OID_LENGTH(saHpiUserEventText_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for saHpiUserEventText */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_saHpiOemNotifications_trap( void )
{
    netsnmp_variable_list  *var_list = NULL;
    oid saHpiOemNotifications_oid[] = { 1,3,6,1,4,1,18568,2,1,1,5,8 };
    oid saHpiDomainId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,2,1,1, /* insert index here */ };
    oid saHpiResourceId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,8,1,1, /* insert index here */ };
    oid saHpiEventSeverity_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,3,1,3, /* insert index here */ };
    oid saHpiOemEventTextType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,27,1,4, /* insert index here */ };
    oid saHpiOemEventTextLanguage_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,27,1,5, /* insert index here */ };
    oid saHpiOemEventText_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,27,1,6, /* insert index here */ };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, OID_LENGTH(snmptrap_oid),
        ASN_OBJECT_ID,
        saHpiOemNotifications_oid, sizeof(saHpiOemNotifications_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        saHpiDomainId_oid, OID_LENGTH(saHpiDomainId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiDomainId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceId_oid, OID_LENGTH(saHpiResourceId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiResourceId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiEventSeverity_oid, OID_LENGTH(saHpiEventSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiEventSeverity */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiOemEventTextType_oid, OID_LENGTH(saHpiOemEventTextType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiOemEventTextType */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiOemEventTextLanguage_oid, OID_LENGTH(saHpiOemEventTextLanguage_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiOemEventTextLanguage */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiOemEventText_oid, OID_LENGTH(saHpiOemEventText_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for saHpiOemEventText */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_saHpiSoftwareNotifications_trap( void )
{
    netsnmp_variable_list  *var_list = NULL;
    oid saHpiSoftwareNotifications_oid[] = { 1,3,6,1,4,1,18568,2,1,1,5,7 };
    oid saHpiDomainId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,2,1,1, /* insert index here */ };
    oid saHpiResourceId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,8,1,1, /* insert index here */ };
    oid saHpiEventSeverity_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,3,1,3, /* insert index here */ };
    oid saHpiSoftwareEventType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,24,1,4, /* insert index here */ };
    oid saHpiSoftwareEventTextType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,24,1,5, /* insert index here */ };
    oid saHpiSoftwareEventTextLanguage_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,24,1,6, /* insert index here */ };
    oid saHpiSoftwareEventText_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,24,1,7, /* insert index here */ };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, OID_LENGTH(snmptrap_oid),
        ASN_OBJECT_ID,
        saHpiSoftwareNotifications_oid, sizeof(saHpiSoftwareNotifications_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        saHpiDomainId_oid, OID_LENGTH(saHpiDomainId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiDomainId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceId_oid, OID_LENGTH(saHpiResourceId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiResourceId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiEventSeverity_oid, OID_LENGTH(saHpiEventSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiEventSeverity */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSoftwareEventType_oid, OID_LENGTH(saHpiSoftwareEventType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiSoftwareEventType */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSoftwareEventTextType_oid, OID_LENGTH(saHpiSoftwareEventTextType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiSoftwareEventTextType */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSoftwareEventTextLanguage_oid, OID_LENGTH(saHpiSoftwareEventTextLanguage_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiSoftwareEventTextLanguage */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiSoftwareEventText_oid, OID_LENGTH(saHpiSoftwareEventText_oid),
        ASN_OCTET_STR,
        /* Set an appropriate value for saHpiSoftwareEventText */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_saHpiResourceNotifications_trap( void )
{
    netsnmp_variable_list  *var_list = NULL;
    oid saHpiResourceNotifications_oid[] = { 1,3,6,1,4,1,18568,2,1,1,5,3 };
    oid saHpiDomainId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,2,1,1, /* insert index here */ };
    oid saHpiResourceId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,8,1,1, /* insert index here */ };
    oid saHpiEventSeverity_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,3,1,3, /* insert index here */ };
    oid saHpiResourceEventType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,6,1,3, /* insert index here */ };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, OID_LENGTH(snmptrap_oid),
        ASN_OBJECT_ID,
        saHpiResourceNotifications_oid, sizeof(saHpiResourceNotifications_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        saHpiDomainId_oid, OID_LENGTH(saHpiDomainId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiDomainId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceId_oid, OID_LENGTH(saHpiResourceId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiResourceId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiEventSeverity_oid, OID_LENGTH(saHpiEventSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiEventSeverity */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceEventType_oid, OID_LENGTH(saHpiResourceEventType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiResourceEventType */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_saHpiHotSwapNotification_trap( void )
{
    netsnmp_variable_list  *var_list = NULL;
    oid saHpiHotSwapNotification_oid[] = { 1,3,6,1,4,1,18568,2,1,1,5,6 };
    oid saHpiDomainId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,2,1,1, /* insert index here */ };
    oid saHpiResourceId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,8,1,1, /* insert index here */ };
    oid saHpiEventSeverity_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,3,1,3, /* insert index here */ };
    oid saHpiHotSwapEventState_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,18,1,3, /* insert index here */ };
    oid saHpiHotSwapEventPreviousState_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,18,1,4, /* insert index here */ };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, OID_LENGTH(snmptrap_oid),
        ASN_OBJECT_ID,
        saHpiHotSwapNotification_oid, sizeof(saHpiHotSwapNotification_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        saHpiDomainId_oid, OID_LENGTH(saHpiDomainId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiDomainId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceId_oid, OID_LENGTH(saHpiResourceId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiResourceId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiEventSeverity_oid, OID_LENGTH(saHpiEventSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiEventSeverity */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiHotSwapEventState_oid, OID_LENGTH(saHpiHotSwapEventState_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiHotSwapEventState */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiHotSwapEventPreviousState_oid, OID_LENGTH(saHpiHotSwapEventPreviousState_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiHotSwapEventPreviousState */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
int
send_saHpiDomainNotifications_trap( void )
{
    netsnmp_variable_list  *var_list = NULL;
    oid saHpiDomainNotifications_oid[] = { 1,3,6,1,4,1,18568,2,1,1,5,4 };
    oid saHpiDomainId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,2,1,1, /* insert index here */ };
    oid saHpiResourceId_oid[] = { 1,3,6,1,4,1,18568,2,1,1,2,8,1,1, /* insert index here */ };
    oid saHpiEventSeverity_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,3,1,3, /* insert index here */ };
    oid saHpiDomainEventType_oid[] = { 1,3,6,1,4,1,18568,2,1,1,3,1,9,1,3, /* insert index here */ };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
        snmptrap_oid, OID_LENGTH(snmptrap_oid),
        ASN_OBJECT_ID,
        saHpiDomainNotifications_oid, sizeof(saHpiDomainNotifications_oid));
    
    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
        saHpiDomainId_oid, OID_LENGTH(saHpiDomainId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiDomainId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiResourceId_oid, OID_LENGTH(saHpiResourceId_oid),
        ASN_UNSIGNED,
        /* Set an appropriate value for saHpiResourceId */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiEventSeverity_oid, OID_LENGTH(saHpiEventSeverity_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiEventSeverity */
        NULL, 0);
    snmp_varlist_add_variable(&var_list,
        saHpiDomainEventType_oid, OID_LENGTH(saHpiDomainEventType_oid),
        ASN_INTEGER,
        /* Set an appropriate value for saHpiDomainEventType */
        NULL, 0);

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap( var_list );
    snmp_free_varbind( var_list );

    return SNMP_ERR_NOERROR;
}
