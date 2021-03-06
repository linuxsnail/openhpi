/*
 * marshaling/demarshaling of hpi data types
 *
 * Copyright (c) 2004 by FORCE Computers.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  This
 * file and program are licensed under a BSD style license.  See
 * the Copying file included with the OpenHPI distribution for
 * full licensing terms.
 *
 * Authors:
 *     Thomas Kanngieser <thomas.kanngieser@fci.com>
 *     W. David Ashley <dashley@us.ibm.com>
 *     Renier Morales <renier@openhpi.org>
 *     Anton Pak <anton.pak@pigeonpoint.com>
 */

#include "marshal_hpi_types.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include <oHpi.h>


// SaHpiTextBuffer
static cMarshalType SaHpiTextBufferDataArray = dArray( SAHPI_MAX_TEXT_BUFFER_LENGTH, SaHpiUint8T, SaHpiUint8Type );

static cMarshalType SaHpiTextBufferElements[] =
{
  dStructElement( SaHpiTextBufferT, DataType, SaHpiTextTypeType ),
  dStructElement( SaHpiTextBufferT, Language, SaHpiLanguageType ),
  dStructElement( SaHpiTextBufferT, DataLength, SaHpiUint8Type ),
  dStructElement( SaHpiTextBufferT, Data, SaHpiTextBufferDataArray ),
  dStructElementEnd()
};

cMarshalType SaHpiTextBufferType = dStruct( SaHpiTextBufferElements );

// oHpi
static cMarshalType oHpiHandlerConfigParamTypeNameArray = dArray( SAHPI_MAX_TEXT_BUFFER_LENGTH, SaHpiUint8T, SaHpiUint8Type );
static cMarshalType oHpiHandlerConfigParamTypeValueArray = dArray( SAHPI_MAX_TEXT_BUFFER_LENGTH, SaHpiUint8T, SaHpiUint8Type );
static cMarshalType oHpiHandlerConfigParamTypeElements[] =
{
	dStructElement( oHpiHandlerConfigParamT, Name, oHpiHandlerConfigParamTypeNameArray ),
	dStructElement( oHpiHandlerConfigParamT, Value, oHpiHandlerConfigParamTypeValueArray ),
	dStructElementEnd()
};
cMarshalType oHpiHandlerConfigParamType = dStruct( oHpiHandlerConfigParamTypeElements );

static cMarshalType HandlerConfigParamsArray = dVarArray( 0, oHpiHandlerConfigParamT, oHpiHandlerConfigParamType );
static cMarshalType oHpiHandlerConfigTypeElements[] =
{
	dStructElement( oHpiHandlerConfigT, NumberOfParams, SaHpiUint8Type ),
	dStructElement( oHpiHandlerConfigT, Params, HandlerConfigParamsArray ),
	dStructElementEnd()
};
cMarshalType oHpiHandlerConfigType = dStruct( oHpiHandlerConfigTypeElements );

// entity
static cMarshalType SaHpiEntityElements[] =
{
  dStructElement( SaHpiEntityT, EntityType, SaHpiEntityTypeType ),
  dStructElement( SaHpiEntityT, EntityLocation, SaHpiEntityLocationType ),
  dStructElementEnd()
};

cMarshalType SaHpiEntityType = dStruct( SaHpiEntityElements );

// entity path
static cMarshalType SaHpiEntityPathEntryArray = dArray( SAHPI_MAX_ENTITY_PATH, SaHpiEntityT, SaHpiEntityType );


static cMarshalType SaHpiEntityPathElements[] =
{
  dStructElement( SaHpiEntityPathT, Entry, SaHpiEntityPathEntryArray ),
  dStructElementEnd()
};

cMarshalType SaHpiEntityPathType = dStruct( SaHpiEntityPathElements );

// sensors
static cMarshalType SaHpiSensorInterpretedUnionBufferArray = dArray( SAHPI_SENSOR_BUFFER_LENGTH, SaHpiUint8T, SaHpiUint8Type );

static cMarshalType SaHpiSensorReadingUnionElements[] =
{
  dUnionElement( SAHPI_SENSOR_READING_TYPE_INT64,   SaHpiInt64Type ),
  dUnionElement( SAHPI_SENSOR_READING_TYPE_UINT64,  SaHpiUint64Type ),
  dUnionElement( SAHPI_SENSOR_READING_TYPE_FLOAT64, SaHpiFloat64Type ),
  dUnionElement( SAHPI_SENSOR_READING_TYPE_BUFFER , SaHpiSensorInterpretedUnionBufferArray ),
  dUnionElementEnd()
};

static cMarshalType SaHpiSensorReadingUnionType = dUnion( 1, SaHpiSensorReadingUnionElements );

// sensor reading
static cMarshalType SaHpiSensorReadingTElements[] =
{
  dStructElement( SaHpiSensorReadingT, IsSupported, SaHpiBoolType ),
  dStructElement( SaHpiSensorReadingT, Type, SaHpiSensorReadingTypeType ),
  dStructElement( SaHpiSensorReadingT, Value, SaHpiSensorReadingUnionType ),
  dStructElementEnd()
};

cMarshalType SaHpiSensorReadingType = dStruct( SaHpiSensorReadingTElements );


// sensor threshold values

static cMarshalType SaHpiSensorThresholdsElements[] =
{
  dStructElement( SaHpiSensorThresholdsT, LowCritical     , SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorThresholdsT, LowMajor        , SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorThresholdsT, LowMinor        , SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorThresholdsT, UpCritical      , SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorThresholdsT, UpMajor         , SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorThresholdsT, UpMinor         , SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorThresholdsT, PosThdHysteresis, SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorThresholdsT, NegThdHysteresis, SaHpiSensorReadingType ),
  dStructElementEnd()
};

cMarshalType SaHpiSensorThresholdsType = dStruct( SaHpiSensorThresholdsElements );


// sensor range

static cMarshalType SaHpiSensorRangeElements[] =
{
  dStructElement( SaHpiSensorRangeT, Flags    , SaHpiSensorRangeFlagsType ),
  dStructElement( SaHpiSensorRangeT, Max      , SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorRangeT, Min      , SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorRangeT, Nominal  , SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorRangeT, NormalMax, SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorRangeT, NormalMin, SaHpiSensorReadingType ),
  dStructElementEnd()
};


cMarshalType SaHpiSensorRangeType = dStruct( SaHpiSensorRangeElements );


// sensor units

static cMarshalType SaHpiSensorDataFormatElements[] =
{
	dStructElement( SaHpiSensorDataFormatT, IsSupported, SaHpiBoolType ),
	dStructElement( SaHpiSensorDataFormatT, ReadingType, SaHpiSensorReadingTypeType ),
   	dStructElement( SaHpiSensorDataFormatT, BaseUnits, SaHpiSensorUnitsType ),
	dStructElement( SaHpiSensorDataFormatT, ModifierUnits, SaHpiSensorUnitsType ),
	dStructElement( SaHpiSensorDataFormatT, ModifierUse, SaHpiSensorModUnitUseType ),
	dStructElement( SaHpiSensorDataFormatT, Percentage, SaHpiBoolType ),
        dStructElement( SaHpiSensorDataFormatT, Range, SaHpiSensorRangeType ),
        dStructElement( SaHpiSensorDataFormatT, AccuracyFactor,SaHpiFloat64Type ),
	dStructElementEnd()
};

cMarshalType SaHpiSensorDataFormatType = dStruct( SaHpiSensorDataFormatElements );


// threshold support

static cMarshalType SaHpiSensorThdDefnElements[] =
{
  dStructElement( SaHpiSensorThdDefnT, IsAccessible, SaHpiBoolType ),
  dStructElement( SaHpiSensorThdDefnT, ReadThold, SaHpiSensorThdMaskType ),
  dStructElement( SaHpiSensorThdDefnT, WriteThold, SaHpiSensorThdMaskType ),
  dStructElement( SaHpiSensorThdDefnT, Nonlinear, SaHpiBoolType ),
  dStructElementEnd()
};

cMarshalType SaHpiSensorThdDefnType = dStruct( SaHpiSensorThdDefnElements );


// sensor record

static cMarshalType SaHpiSensorRecElements[] =
{
  dStructElement( SaHpiSensorRecT, Num, SaHpiSensorNumType ),
  dStructElement( SaHpiSensorRecT, Type, SaHpiSensorTypeType ),
  dStructElement( SaHpiSensorRecT, Category, SaHpiEventCategoryType ),
  dStructElement( SaHpiSensorRecT, EnableCtrl,SaHpiBoolType ),
  dStructElement( SaHpiSensorRecT, EventCtrl, SaHpiSensorEventCtrlType ),
  dStructElement( SaHpiSensorRecT, Events, SaHpiEventStateType ),
  dStructElement( SaHpiSensorRecT, DataFormat, SaHpiSensorDataFormatType ),
  dStructElement( SaHpiSensorRecT, ThresholdDefn, SaHpiSensorThdDefnType ),
  dStructElement( SaHpiSensorRecT, Oem, SaHpiUint32Type ),
  dStructElementEnd()
};

cMarshalType SaHpiSensorRecType = dStruct( SaHpiSensorRecElements );


// stream control state

static cMarshalType SaHpiCtrlStateStreamArray = dArray( SAHPI_CTRL_MAX_STREAM_LENGTH, SaHpiUint8T, SaHpiUint8Type );


static cMarshalType SaHpiCtrlStateStreamElements[] =
{
  dStructElement( SaHpiCtrlStateStreamT, Repeat, SaHpiBoolType ),
  dStructElement( SaHpiCtrlStateStreamT, StreamLength, SaHpiUint32Type ),
  dStructElement( SaHpiCtrlStateStreamT, Stream, SaHpiCtrlStateStreamArray ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlStateStreamType = dStruct( SaHpiCtrlStateStreamElements );


// text control state

static cMarshalType SaHpiCtrlStateTextElements[] =
{
  dStructElement( SaHpiCtrlStateTextT, Line, SaHpiTxtLineNumType ),
  dStructElement( SaHpiCtrlStateTextT, Text, SaHpiTextBufferType ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlStateTextType = dStruct( SaHpiCtrlStateTextElements );


// OEM control state

static cMarshalType SaHpiCtrlStateOemBodyArray = dArray( SAHPI_CTRL_MAX_OEM_BODY_LENGTH, SaHpiUint8T, SaHpiUint8Type );

static cMarshalType SaHpiCtrlStateOemElements[] =
{
  dStructElement( SaHpiCtrlStateOemT, MId, SaHpiManufacturerIdType ),
  dStructElement( SaHpiCtrlStateOemT, BodyLength, SaHpiUint8Type ),
  dStructElement( SaHpiCtrlStateOemT, Body, SaHpiCtrlStateOemBodyArray ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlStateOemType = dStruct( SaHpiCtrlStateOemElements );


static cMarshalType SaHpiCtrlStateUnionElements[] =
{
  dUnionElement( SAHPI_CTRL_TYPE_DIGITAL, SaHpiCtrlStateDigitalType ),
  dUnionElement( SAHPI_CTRL_TYPE_DISCRETE, SaHpiCtrlStateDiscreteType ),
  dUnionElement( SAHPI_CTRL_TYPE_ANALOG, SaHpiCtrlStateAnalogType ),
  dUnionElement( SAHPI_CTRL_TYPE_STREAM, SaHpiCtrlStateStreamType ),
  dUnionElement( SAHPI_CTRL_TYPE_TEXT, SaHpiCtrlStateTextType ),
  dUnionElement( SAHPI_CTRL_TYPE_OEM, SaHpiCtrlStateOemType ),
  dUnionElementEnd()
};


static cMarshalType SaHpiCtrlStateUnionType = dUnion( 0, SaHpiCtrlStateUnionElements );

static cMarshalType SaHpiCtrlStateElements[] =
{
  dStructElement( SaHpiCtrlStateT, Type, SaHpiCtrlTypeType ),
  dStructElement( SaHpiCtrlStateT, StateUnion, SaHpiCtrlStateUnionType ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlStateType = dStruct( SaHpiCtrlStateElements );


// control rdr record types

static cMarshalType SaHpiCtrlRecDigitalElements[] =
{
  dStructElement( SaHpiCtrlRecDigitalT, Default, SaHpiCtrlStateDigitalType ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlRecDigitalType = dStruct( SaHpiCtrlRecDigitalElements );


static cMarshalType SaHpiCtrlRecDiscreteElements[] =
{
  dStructElement( SaHpiCtrlRecDiscreteT, Default, SaHpiCtrlStateDiscreteType ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlRecDiscreteType = dStruct( SaHpiCtrlRecDiscreteElements );


static cMarshalType SaHpiCtrlRecAnalogElements[] =
{
  dStructElement( SaHpiCtrlRecAnalogT, Min, SaHpiCtrlStateAnalogType ),
  dStructElement( SaHpiCtrlRecAnalogT, Max, SaHpiCtrlStateAnalogType ),
  dStructElement( SaHpiCtrlRecAnalogT, Default, SaHpiCtrlStateAnalogType ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlRecAnalogType = dStruct( SaHpiCtrlRecAnalogElements );


static cMarshalType SaHpiCtrlRecStreamElements[] =
{
  dStructElement( SaHpiCtrlRecStreamT, Default, SaHpiCtrlStateStreamType ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlRecStreamType = dStruct( SaHpiCtrlRecStreamElements );


static cMarshalType SaHpiCtrlRecTextElements[] =
{
  dStructElement( SaHpiCtrlRecTextT, MaxChars, SaHpiUint8Type ),
  dStructElement( SaHpiCtrlRecTextT, MaxLines, SaHpiUint8Type ),
  dStructElement( SaHpiCtrlRecTextT, Language, SaHpiLanguageType ),
  dStructElement( SaHpiCtrlRecTextT, DataType, SaHpiTextTypeType ),
  dStructElement( SaHpiCtrlRecTextT, Default, SaHpiCtrlStateTextType ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlRecTextType = dStruct( SaHpiCtrlRecTextElements );


static cMarshalType SaHpiCtrlRecOemConfigDataArray = dArray( SAHPI_CTRL_OEM_CONFIG_LENGTH, SaHpiUint8T, SaHpiUint8Type );

static cMarshalType SaHpiCtrlRecOemElements[] =
{
  dStructElement( SaHpiCtrlRecOemT, MId, SaHpiManufacturerIdType ),
  dStructElement( SaHpiCtrlRecOemT, ConfigData, SaHpiCtrlRecOemConfigDataArray ),
  dStructElement( SaHpiCtrlRecOemT, Default, SaHpiCtrlStateOemType ),
  dStructElementEnd()
};

cMarshalType SaHpiCtrlRecOemType = dStruct( SaHpiCtrlRecOemElements );


static cMarshalType SaHpiCtrlRecUnionElements[] =
{
	dUnionElement( SAHPI_CTRL_TYPE_DIGITAL, SaHpiCtrlRecDigitalType ),
	dUnionElement( SAHPI_CTRL_TYPE_DISCRETE, SaHpiCtrlRecDiscreteType ),
	dUnionElement( SAHPI_CTRL_TYPE_ANALOG, SaHpiCtrlRecAnalogType ),
	dUnionElement( SAHPI_CTRL_TYPE_STREAM, SaHpiCtrlRecStreamType ),
	dUnionElement( SAHPI_CTRL_TYPE_TEXT, SaHpiCtrlRecTextType ),
	dUnionElement( SAHPI_CTRL_TYPE_OEM, SaHpiCtrlRecOemType ),
	dUnionElementEnd()
};

static cMarshalType SaHpiCtrlRecUnionType = dUnion( 2, SaHpiCtrlRecUnionElements );


// control rdr mode

static cMarshalType SaHpiCtrlDefaultModeElements[] =
{
	dStructElement( SaHpiCtrlDefaultModeT, Mode, SaHpiCtrlModeType ),
	dStructElement( SaHpiCtrlDefaultModeT, ReadOnly, SaHpiBoolType),
	dStructElementEnd()
};

cMarshalType SaHpiCtrlDefaultModeType = dStruct( SaHpiCtrlDefaultModeElements );


static cMarshalType SaHpiCtrlRecElements[] =
{
	dStructElement( SaHpiCtrlRecT, Num, SaHpiCtrlNumType ),

	dStructElement( SaHpiCtrlRecT, OutputType, SaHpiCtrlOutputTypeType ),
	dStructElement( SaHpiCtrlRecT, Type, SaHpiCtrlTypeType ),
	dStructElement( SaHpiCtrlRecT, TypeUnion, SaHpiCtrlRecUnionType ),
	dStructElement( SaHpiCtrlRecT, DefaultMode, SaHpiCtrlDefaultModeType ),
	dStructElement( SaHpiCtrlRecT, WriteOnly, SaHpiBoolType ),
	dStructElement( SaHpiCtrlRecT, Oem, SaHpiUint32Type),
	dStructElementEnd()
};

cMarshalType SaHpiCtrlRecType = dStruct( SaHpiCtrlRecElements );


// entity inventory data

static cMarshalType SaHpiIdrFieldTElements[] =
{
  dStructElement( SaHpiIdrFieldT, AreaId, SaHpiEntryIdType ),
  dStructElement( SaHpiIdrFieldT, FieldId, SaHpiEntryIdType ),
  dStructElement( SaHpiIdrFieldT, Type, SaHpiIdrFieldTypeType ),
  dStructElement( SaHpiIdrFieldT, ReadOnly, SaHpiBoolType ),
  dStructElement( SaHpiIdrFieldT, Field, SaHpiTextBufferType ),
  dStructElementEnd()
};

cMarshalType SaHpiIdrFieldType = dStruct( SaHpiIdrFieldTElements );

static cMarshalType SaHpiIdrAreaHeaderTElements[] =
{
  dStructElement( SaHpiIdrAreaHeaderT, AreaId, SaHpiEntryIdType ),
  dStructElement( SaHpiIdrAreaHeaderT, Type, SaHpiIdrAreaTypeType ),
  dStructElement( SaHpiIdrAreaHeaderT, ReadOnly, SaHpiBoolType ),
  dStructElement( SaHpiIdrAreaHeaderT, NumFields, SaHpiUint32Type ),
  dStructElementEnd()
};

cMarshalType SaHpiIdrAreaHeaderType = dStruct( SaHpiIdrAreaHeaderTElements );

static cMarshalType SaHpiIdrInfoTElements[] =
{
  dStructElement( SaHpiIdrInfoT, IdrId, SaHpiIdrIdType ),
  dStructElement( SaHpiIdrInfoT, UpdateCount, SaHpiUint32Type ),
  dStructElement( SaHpiIdrInfoT, ReadOnly, SaHpiBoolType ),
  dStructElement( SaHpiIdrInfoT, NumAreas, SaHpiUint32Type ),
  dStructElementEnd()
};

cMarshalType SaHpiIdrInfoType = dStruct( SaHpiIdrInfoTElements );


// inventory resource data records

static cMarshalType SaHpiInventoryRecElements[] =
{
  dStructElement( SaHpiInventoryRecT, IdrId, SaHpiUint32Type ),
  dStructElement( SaHpiInventoryRecT, Persistent, SaHpiBoolType ),
  dStructElement( SaHpiInventoryRecT, Oem, SaHpiUint32Type ),
  dStructElementEnd()
};

cMarshalType SaHpiInventoryRecType = dStruct( SaHpiInventoryRecElements );


// watchdogs

static cMarshalType SaHpiWatchdogElements[] =
{
  dStructElement( SaHpiWatchdogT, Log, SaHpiBoolType ),
  dStructElement( SaHpiWatchdogT, Running, SaHpiBoolType ),
  dStructElement( SaHpiWatchdogT, TimerUse, SaHpiWatchdogTimerUseType ),
  dStructElement( SaHpiWatchdogT, TimerAction, SaHpiWatchdogActionType ),
  dStructElement( SaHpiWatchdogT, PretimerInterrupt, SaHpiWatchdogPretimerInterruptType ),
  dStructElement( SaHpiWatchdogT, PreTimeoutInterval, SaHpiUint32Type ),
  dStructElement( SaHpiWatchdogT, TimerUseExpFlags, SaHpiWatchdogExpFlagsType ),
  dStructElement( SaHpiWatchdogT, InitialCount, SaHpiUint32Type ),
  dStructElement( SaHpiWatchdogT, PresentCount, SaHpiUint32Type ),
  dStructElementEnd()
};

cMarshalType SaHpiWatchdogType = dStruct( SaHpiWatchdogElements );


// watchdog resource data records

static cMarshalType SaHpiWatchdogRecElements[] =
{
  dStructElement( SaHpiWatchdogRecT, WatchdogNum, SaHpiWatchdogNumType ),
  dStructElement( SaHpiWatchdogRecT, Oem, SaHpiUint32Type ),
  dStructElementEnd()
};

cMarshalType SaHpiWatchdogRecType = dStruct( SaHpiWatchdogRecElements );


// annunciators

static cMarshalType SaHpiNameDataArray = dArray( SA_HPI_MAX_NAME_LENGTH, SaHpiUint8T, SaHpiUint8Type );

static cMarshalType SaHpiNameElements[] =
{
  dStructElement( SaHpiNameT, Length, SaHpiUint16Type ),
  dStructElement( SaHpiNameT, Value, SaHpiNameDataArray ),
  dStructElementEnd()
};

cMarshalType SaHpiNameType = dStruct( SaHpiNameElements );



static cMarshalType SaHpiConditionTypeElements[] =
{
  dStructElement( SaHpiConditionT, Type, SaHpiStatusCondTypeType ),
  dStructElement( SaHpiConditionT, Entity, SaHpiEntityPathType ),
  dStructElement( SaHpiConditionT, DomainId, SaHpiDomainIdType ),
  dStructElement( SaHpiConditionT, ResourceId, SaHpiResourceIdType ),
  dStructElement( SaHpiConditionT, SensorNum, SaHpiSensorNumType ),
  dStructElement( SaHpiConditionT, EventState, SaHpiEventStateType ),
  dStructElement( SaHpiConditionT, Name, SaHpiNameType ),
  dStructElement( SaHpiConditionT, Mid, SaHpiManufacturerIdType ),
  dStructElement( SaHpiConditionT, Data, SaHpiTextBufferType ),
  dStructElementEnd()
};

cMarshalType SaHpiConditionType = dStruct( SaHpiConditionTypeElements );


static cMarshalType SaHpiAnnouncementTypeElements[] =
{
  dStructElement( SaHpiAnnouncementT, EntryId, SaHpiEntryIdType ),
  dStructElement( SaHpiAnnouncementT, Timestamp, SaHpiTimeType ),
  dStructElement( SaHpiAnnouncementT, AddedByUser, SaHpiBoolType ),
  dStructElement( SaHpiAnnouncementT, Severity, SaHpiSeverityType ),
  dStructElement( SaHpiAnnouncementT, Acknowledged, SaHpiBoolType ),
  dStructElement( SaHpiAnnouncementT, StatusCond, SaHpiConditionType ),
  dStructElementEnd()
};

cMarshalType SaHpiAnnouncementType = dStruct( SaHpiAnnouncementTypeElements );


// annunciators rdr

static cMarshalType SaHpiAnnunciatorRecElements[] =
{
	dStructElement( SaHpiAnnunciatorRecT, AnnunciatorNum, SaHpiAnnunciatorNumType ),
	dStructElement( SaHpiAnnunciatorRecT, AnnunciatorType, SaHpiAnnunciatorTypeType ),
	dStructElement( SaHpiAnnunciatorRecT, ModeReadOnly, SaHpiBoolType ),
	dStructElement( SaHpiAnnunciatorRecT, MaxConditions, SaHpiUint32Type ),
	dStructElement( SaHpiAnnunciatorRecT, Oem, SaHpiUint32Type ),
	dStructElementEnd()
};

cMarshalType SaHpiAnnunciatorRecType = dStruct( SaHpiAnnunciatorRecElements );


//DIMIs

static cMarshalType SaHpiDimiInfoElements[] =
{
	dStructElement( SaHpiDimiInfoT, NumberOfTests, SaHpiDimiTotalTestsType ),
	dStructElement( SaHpiDimiInfoT, TestNumUpdateCounter, SaHpiUint32Type ),
	dStructElementEnd()
};
cMarshalType SaHpiDimiInfoType = dStruct( SaHpiDimiInfoElements );

static cMarshalType SaHpiDimiTestParameterValueUnionTypeElements[] =
{
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_INT32, SaHpiInt32Type ),
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_FLOAT64, SaHpiFloat64Type ),
        /* These two types are disregarded but must be spepcified */
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_BOOLEAN, SaHpiFloat64Type ),
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_TEXT, SaHpiFloat64Type ),
        dUnionElementEnd()
};
cMarshalType SaHpiDimiTestParameterValueUnionType = dUnion( 2, SaHpiDimiTestParameterValueUnionTypeElements );

static cMarshalType SaHpiDimiTestParamValue2TypeElements[] =
{
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_INT32, SaHpiInt32Type ),
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_FLOAT64, SaHpiFloat64Type ),
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_BOOLEAN, SaHpiBoolType ),
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_TEXT, SaHpiTextBufferType ),
        dUnionElementEnd()
};
cMarshalType SaHpiDimiTestParamValue2Type = dUnion( 2, SaHpiDimiTestParamValue2TypeElements );
static cMarshalType SaHpiDimiTestParamValue1TypeElements[] =
{
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_INT32, SaHpiInt32Type ),
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_FLOAT64, SaHpiFloat64Type ),
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_BOOLEAN, SaHpiBoolType ),
        dUnionElement( SAHPI_DIMITEST_PARAM_TYPE_TEXT, SaHpiTextBufferType ),
        dUnionElementEnd()
};
cMarshalType SaHpiDimiTestParamValue1Type = dUnion( 1, SaHpiDimiTestParamValue1TypeElements );

static cMarshalType ParamNameArray = dArray( SAHPI_DIMITEST_PARAM_NAME_LEN, SaHpiUint8T, SaHpiUint8Type );
static cMarshalType SaHpiDimiTestParamsDefinitionTypeElements[] =
{
        dStructElement( SaHpiDimiTestParamsDefinitionT, ParamName, ParamNameArray ),
        dStructElement( SaHpiDimiTestParamsDefinitionT, ParamInfo, SaHpiTextBufferType ),
        dStructElement( SaHpiDimiTestParamsDefinitionT, ParamType, SaHpiDimiTestParamTypeType ),
        dStructElement( SaHpiDimiTestParamsDefinitionT, MinValue, SaHpiDimiTestParameterValueUnionType ),
        dStructElement( SaHpiDimiTestParamsDefinitionT, MaxValue, SaHpiDimiTestParameterValueUnionType ),
        dStructElement( SaHpiDimiTestParamsDefinitionT, DefaultParam, SaHpiDimiTestParamValue2Type ),
        dStructElementEnd()
};
cMarshalType SaHpiDimiTestParamsDefinitionType = dStruct( SaHpiDimiTestParamsDefinitionTypeElements );

static cMarshalType SaHpiDimiTestAffectedEntityTypeElements[] =
{
        dStructElement( SaHpiDimiTestAffectedEntityT, EntityImpacted, SaHpiEntityPathType ),
        dStructElement( SaHpiDimiTestAffectedEntityT, ServiceImpact, SaHpiDimiTestServiceImpactType ),
        dStructElementEnd()
};
cMarshalType SaHpiDimiTestAffectedEntityType = dStruct( SaHpiDimiTestAffectedEntityTypeElements );

static cMarshalType EntitiesImpactedArray = dArray( SAHPI_DIMITEST_MAX_ENTITIESIMPACTED, SaHpiDimiTestAffectedEntityT, SaHpiDimiTestAffectedEntityType );
static cMarshalType TestParametersArray = dArray( SAHPI_DIMITEST_MAX_PARAMETERS, SaHpiDimiTestParamsDefinitionT, SaHpiDimiTestParamsDefinitionType );
static cMarshalType SaHpiDimiTestTypeElements[] =
{
        dStructElement( SaHpiDimiTestT, TestName,  SaHpiTextBufferType ),
        dStructElement( SaHpiDimiTestT, ServiceImpact,  SaHpiDimiTestServiceImpactType ),
        dStructElement( SaHpiDimiTestT, EntitiesImpacted, EntitiesImpactedArray ),
        dStructElement( SaHpiDimiTestT, NeedServiceOS, SaHpiBoolType ),
        dStructElement( SaHpiDimiTestT, ServiceOS, SaHpiTextBufferType ),
        dStructElement( SaHpiDimiTestT, ExpectedRunDuration, SaHpiTimeType ),
        dStructElement( SaHpiDimiTestT, TestCapabilities, SaHpiDimiTestCapabilityType ),
        dStructElement( SaHpiDimiTestT, TestParameters, TestParametersArray ),
        dStructElementEnd()
};
cMarshalType SaHpiDimiTestType = dStruct( SaHpiDimiTestTypeElements );

static cMarshalType SaHpiDimiTestResultsTypeElements[] =
{
        dStructElement( SaHpiDimiTestResultsT, ResultTimeStamp, SaHpiTimeType ),
        dStructElement( SaHpiDimiTestResultsT, RunDuration, SaHpiTimeoutType ),
        dStructElement( SaHpiDimiTestResultsT, LastRunStatus, SaHpiDimiTestRunStatusType ),
        dStructElement( SaHpiDimiTestResultsT, TestErrorCode, SaHpiDimiTestErrCodeType ),
        dStructElement( SaHpiDimiTestResultsT, TestResultString, SaHpiTextBufferType ),
        dStructElement( SaHpiDimiTestResultsT, TestResultStringIsURI, SaHpiBoolType ),
        dStructElementEnd()
};
cMarshalType SaHpiDimiTestResultsType = dStruct( SaHpiDimiTestResultsTypeElements );

static cMarshalType SaHpiDimiTestVariableParamsTypeElements[] =
{
        dStructElement( SaHpiDimiTestVariableParamsT, ParamName, ParamNameArray ),
        dStructElement( SaHpiDimiTestVariableParamsT, ParamType, SaHpiDimiTestParamTypeType ),
        dStructElement( SaHpiDimiTestVariableParamsT, Value, SaHpiDimiTestParamValue1Type ),
        dStructElementEnd()
};
cMarshalType SaHpiDimiTestVariableParamsType = dStruct( SaHpiDimiTestVariableParamsTypeElements );

static cMarshalType ParamsListArray = dVarArray( 0, SaHpiDimiTestVariableParamsT, SaHpiDimiTestVariableParamsType );
static cMarshalType SaHpiDimiTestVariableParamsListTypeElements[] =
{
        dStructElement( SaHpiDimiTestVariableParamsListT, NumberOfParams, SaHpiUint8Type ),
        dStructElement( SaHpiDimiTestVariableParamsListT, ParamsList, ParamsListArray ),
        dStructElementEnd()
};
cMarshalType SaHpiDimiTestVariableParamsListType = dStruct( SaHpiDimiTestVariableParamsListTypeElements );

// DIMI rdr
static cMarshalType SaHpiDimiRecElements[] =
{
   dStructElement( SaHpiDimiRecT, DimiNum, SaHpiDimiNumType ),
   dStructElement( SaHpiDimiRecT, Oem, SaHpiUint32Type ),
   dStructElementEnd()
};

cMarshalType SaHpiDimiRecType = dStruct( SaHpiDimiRecElements );

// FUMIs
static cMarshalType SaHpiFumiSafDefinedSpecInfoElements[] =
{
        dStructElement( SaHpiFumiSafDefinedSpecInfoT, SpecID, SaHpiFumiSafDefinedSpecIdType ),
        dStructElement( SaHpiFumiSafDefinedSpecInfoT, RevisionID, SaHpiUint32Type ),
        dStructElementEnd()
};
static cMarshalType SaHpiFumiSafDefinedSpecInfoType = dStruct( SaHpiFumiSafDefinedSpecInfoElements );

static cMarshalType SaHpiFumiOemDefinedSpecInfoBodyArray = dArray( SAHPI_FUMI_MAX_OEM_BODY_LENGTH, SaHpiUint8T, SaHpiUint8Type );

static cMarshalType SaHpiFumiOemDefinedSpecInfoElements[] =
{
        dStructElement( SaHpiFumiOemDefinedSpecInfoT, Mid, SaHpiManufacturerIdType ),
        dStructElement( SaHpiFumiOemDefinedSpecInfoT, BodyLength, SaHpiUint8Type ),
        dStructElement( SaHpiFumiOemDefinedSpecInfoT, Body, SaHpiFumiOemDefinedSpecInfoBodyArray ),
        dStructElementEnd()
};
static cMarshalType SaHpiFumiOemDefinedSpecInfoType = dStruct( SaHpiFumiOemDefinedSpecInfoElements );

static cMarshalType SaHpiFumiSpecInfoTypeUnionElements[] =
{
        dUnionElement( SAHPI_FUMI_SPEC_INFO_NONE, SaHpiVoidType ),
        dUnionElement( SAHPI_FUMI_SPEC_INFO_SAF_DEFINED, SaHpiFumiSafDefinedSpecInfoType ),
        dUnionElement( SAHPI_FUMI_SPEC_INFO_OEM_DEFINED, SaHpiFumiOemDefinedSpecInfoType ),
        dUnionElementEnd()
};

static cMarshalType SaHpiFumiSpecInfoTypeUnionType = dUnion( 0, SaHpiFumiSpecInfoTypeUnionElements );

static cMarshalType SaHpiFumiSpecInfoTypeElements[] =
{
        dStructElement( SaHpiFumiSpecInfoT, SpecInfoType, SaHpiFumiSpecInfoTypeType ),
        dStructElement( SaHpiFumiSpecInfoT, SpecInfoTypeUnion, SaHpiFumiSpecInfoTypeUnionType ),
        dStructElementEnd()
};
cMarshalType SaHpiFumiSpecInfoType = dStruct( SaHpiFumiSpecInfoTypeElements );
 
static cMarshalType SaHpiFumiImpactedEntityTypeElements[] =
{
        dStructElement( SaHpiFumiImpactedEntityT, ImpactedEntity, SaHpiEntityPathType ),
        dStructElement( SaHpiFumiImpactedEntityT, ServiceImpact, SaHpiFumiServiceImpactType ),
        dStructElementEnd()
};
static cMarshalType SaHpiFumiImpactedEntityType = dStruct( SaHpiFumiImpactedEntityTypeElements );

static cMarshalType SaHpiFumiServiceImpactDataImpactedEntitiesArray = dArray( SAHPI_FUMI_MAX_ENTITIES_IMPACTED, SaHpiFumiImpactedEntityT, SaHpiFumiImpactedEntityType );

static cMarshalType SaHpiFumiServiceImpactDataElements[] =
{
        dStructElement( SaHpiFumiServiceImpactDataT, NumEntities, SaHpiUint32Type ),
        dStructElement( SaHpiFumiServiceImpactDataT, ImpactedEntities, SaHpiFumiServiceImpactDataImpactedEntitiesArray ),
        dStructElementEnd()
};
cMarshalType SaHpiFumiServiceImpactDataType = dStruct( SaHpiFumiServiceImpactDataElements );

static cMarshalType SaHpiFumiSourceInfoTypeElements[] =
{
        dStructElement( SaHpiFumiSourceInfoT, SourceUri, SaHpiTextBufferType ),
        dStructElement( SaHpiFumiSourceInfoT, SourceStatus, SaHpiFumiSourceStatusType ),
        dStructElement( SaHpiFumiSourceInfoT, Identifier, SaHpiTextBufferType ),
        dStructElement( SaHpiFumiSourceInfoT, Description, SaHpiTextBufferType ),
        dStructElement( SaHpiFumiSourceInfoT, DateTime, SaHpiTextBufferType ),
        dStructElement( SaHpiFumiSourceInfoT, MajorVersion, SaHpiUint32Type ),
        dStructElement( SaHpiFumiSourceInfoT, MinorVersion, SaHpiUint32Type ),
        dStructElement( SaHpiFumiSourceInfoT, AuxVersion, SaHpiUint32Type ),
        dStructElementEnd()
};
cMarshalType SaHpiFumiSourceInfoType = dStruct( SaHpiFumiSourceInfoTypeElements );

static cMarshalType SaHpiFumiBankInfoTypeElements[] =
{
        dStructElement( SaHpiFumiBankInfoT, BankId, SaHpiUint8Type ),
        dStructElement( SaHpiFumiBankInfoT, BankSize, SaHpiUint32Type ),
        dStructElement( SaHpiFumiBankInfoT, Position, SaHpiUint32Type ),
        dStructElement( SaHpiFumiBankInfoT, BankState, SaHpiFumiBankStateType ),
        dStructElement( SaHpiFumiBankInfoT, Identifier, SaHpiTextBufferType ),
        dStructElement( SaHpiFumiBankInfoT, Description, SaHpiTextBufferType ),
        dStructElement( SaHpiFumiBankInfoT, DateTime, SaHpiTextBufferType ),
        dStructElement( SaHpiFumiBankInfoT, MajorVersion, SaHpiUint32Type ),
        dStructElement( SaHpiFumiBankInfoT, MinorVersion, SaHpiUint32Type ),
        dStructElement( SaHpiFumiBankInfoT, AuxVersion, SaHpiUint32Type ),
        dStructElementEnd()
};
cMarshalType SaHpiFumiBankInfoType = dStruct( SaHpiFumiBankInfoTypeElements );

static cMarshalType SaHpiFumiFirmwareInstanceInfoTypeElements[] =
{
        dStructElement( SaHpiFumiFirmwareInstanceInfoT, InstancePresent, SaHpiBoolType ),
        dStructElement( SaHpiFumiFirmwareInstanceInfoT, Identifier, SaHpiTextBufferType ),
        dStructElement( SaHpiFumiFirmwareInstanceInfoT, Description, SaHpiTextBufferType ),
        dStructElement( SaHpiFumiFirmwareInstanceInfoT, DateTime,  SaHpiTextBufferType ),
        dStructElement( SaHpiFumiFirmwareInstanceInfoT, MajorVersion, SaHpiUint32Type ),
        dStructElement( SaHpiFumiFirmwareInstanceInfoT, MinorVersion, SaHpiUint32Type ),
        dStructElement( SaHpiFumiFirmwareInstanceInfoT, AuxVersion, SaHpiUint32Type ),
        dStructElementEnd()
};
static cMarshalType SaHpiFumiFirmwareInstanceInfoType = dStruct( SaHpiFumiFirmwareInstanceInfoTypeElements );

static cMarshalType SaHpiFumiLogicalBankInfoTypeElements[] =
{
        dStructElement( SaHpiFumiLogicalBankInfoT, FirmwarePersistentLocationCount, SaHpiUint8Type ),
        dStructElement( SaHpiFumiLogicalBankInfoT, BankStateFlags, SaHpiFumiLogicalBankStateFlagsType ),
        dStructElement( SaHpiFumiLogicalBankInfoT, PendingFwInstance, SaHpiFumiFirmwareInstanceInfoType ),
        dStructElement( SaHpiFumiLogicalBankInfoT, RollbackFwInstance, SaHpiFumiFirmwareInstanceInfoType ),
        dStructElementEnd()
};
cMarshalType SaHpiFumiLogicalBankInfoType = dStruct( SaHpiFumiLogicalBankInfoTypeElements );

static cMarshalType SaHpiFumiComponentInfoTypeElements[] =
{
        dStructElement( SaHpiFumiComponentInfoT, EntryId, SaHpiEntryIdType ),
        dStructElement( SaHpiFumiComponentInfoT, ComponentId, SaHpiUint32Type ),
        dStructElement( SaHpiFumiComponentInfoT, MainFwInstance, SaHpiFumiFirmwareInstanceInfoType ),
        dStructElement( SaHpiFumiComponentInfoT, ComponentFlags, SaHpiUint32Type ),
        dStructElementEnd()
};
cMarshalType SaHpiFumiComponentInfoType = dStruct( SaHpiFumiComponentInfoTypeElements );

static cMarshalType SaHpiFumiLogicalComponentInfoTypeElements[] =
{
        dStructElement( SaHpiFumiLogicalComponentInfoT, EntryId, SaHpiEntryIdType ),
        dStructElement( SaHpiFumiLogicalComponentInfoT, ComponentId, SaHpiUint32Type ),
        dStructElement( SaHpiFumiLogicalComponentInfoT, PendingFwInstance, SaHpiFumiFirmwareInstanceInfoType ),
        dStructElement( SaHpiFumiLogicalComponentInfoT, RollbackFwInstance, SaHpiFumiFirmwareInstanceInfoType ),
        dStructElement( SaHpiFumiLogicalComponentInfoT, ComponentFlags, SaHpiUint32Type ),
        dStructElementEnd()
};
cMarshalType SaHpiFumiLogicalComponentInfoType = dStruct( SaHpiFumiLogicalComponentInfoTypeElements );

// FUMI rdr
static cMarshalType SaHpiFumiRecElements[] =
{
   dStructElement( SaHpiFumiRecT, Num, SaHpiFumiNumType ),
   dStructElement( SaHpiFumiRecT, AccessProt, SaHpiFumiProtocolType ),
   dStructElement( SaHpiFumiRecT, Capability, SaHpiFumiCapabilityType ),
   dStructElement( SaHpiFumiRecT, NumBanks, SaHpiUint8Type ),
   dStructElement( SaHpiFumiRecT, Oem, SaHpiUint32Type ),
   dStructElementEnd()
};

cMarshalType SaHpiFumiRecType = dStruct( SaHpiFumiRecElements );

// rdr

static cMarshalType SaHpiRdrTypeUnionTypeElements[] =
{
        dUnionElement( SAHPI_NO_RECORD, SaHpiVoidType ),
        dUnionElement( SAHPI_CTRL_RDR, SaHpiCtrlRecType ),
        dUnionElement( SAHPI_SENSOR_RDR, SaHpiSensorRecType ),
        dUnionElement( SAHPI_INVENTORY_RDR, SaHpiInventoryRecType ),
        dUnionElement( SAHPI_WATCHDOG_RDR, SaHpiWatchdogRecType ),
        dUnionElement( SAHPI_ANNUNCIATOR_RDR, SaHpiAnnunciatorRecType ),
        dUnionElement( SAHPI_DIMI_RDR, SaHpiDimiRecType ),
        dUnionElement( SAHPI_FUMI_RDR, SaHpiFumiRecType ),
        dUnionElementEnd()
};

static cMarshalType SaHpiRdrTypeUnionType = dUnion( 1, SaHpiRdrTypeUnionTypeElements );

static cMarshalType SaHpiRdrElements[] =
{
	dStructElement( SaHpiRdrT, RecordId, SaHpiEntryIdType ),
	dStructElement( SaHpiRdrT, RdrType, SaHpiRdrTypeType ),
	dStructElement( SaHpiRdrT, Entity, SaHpiEntityPathType ),
	dStructElement( SaHpiRdrT, IsFru, SaHpiBoolType ),
	dStructElement( SaHpiRdrT, RdrTypeUnion, SaHpiRdrTypeUnionType ),
	dStructElement( SaHpiRdrT, IdString, SaHpiTextBufferType ),
	dStructElementEnd()
};

cMarshalType SaHpiRdrType = dStruct( SaHpiRdrElements );


// events part 2

static cMarshalType SaHpiResourceEventTypeElements[] =
{
  dStructElement( SaHpiResourceEventT, ResourceEventType, SaHpiResourceEventTypeType ),
  dStructElementEnd()
};

cMarshalType SaHpiResourceEventType = dStruct( SaHpiResourceEventTypeElements );


static cMarshalType SaHpiDomainEventTypeElements[] =
{
  dStructElement( SaHpiDomainEventT, Type, SaHpiDomainEventTypeType ),
  dStructElement( SaHpiDomainEventT, DomainId, SaHpiDomainIdType ),
  dStructElementEnd()
};

cMarshalType SaHpiDomainEventType = dStruct( SaHpiDomainEventTypeElements );


static cMarshalType SaHpiSensorEventElements[] =
{
  dStructElement( SaHpiSensorEventT, SensorNum, SaHpiSensorNumType ),
  dStructElement( SaHpiSensorEventT, SensorType, SaHpiSensorTypeType ),
  dStructElement( SaHpiSensorEventT, EventCategory, SaHpiEventCategoryType ),
  dStructElement( SaHpiSensorEventT, Assertion, SaHpiBoolType ),
  dStructElement( SaHpiSensorEventT, EventState, SaHpiEventStateType ),
  dStructElement( SaHpiSensorEventT, OptionalDataPresent, SaHpiSensorOptionalDataType ),
  dStructElement( SaHpiSensorEventT, TriggerReading, SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorEventT, TriggerThreshold, SaHpiSensorReadingType ),
  dStructElement( SaHpiSensorEventT, PreviousState, SaHpiEventStateType ),
  dStructElement( SaHpiSensorEventT, CurrentState, SaHpiEventStateType ),
  dStructElement( SaHpiSensorEventT, Oem, SaHpiUint32Type ),
  dStructElement( SaHpiSensorEventT, SensorSpecific, SaHpiUint32Type ),
  dStructElementEnd()
};

cMarshalType SaHpiSensorEventType = dStruct( SaHpiSensorEventElements );


static cMarshalType SaHpiSensorEnableChangeEventTElements[] =
{
  dStructElement( SaHpiSensorEnableChangeEventT, SensorNum, SaHpiSensorNumType ),
  dStructElement( SaHpiSensorEnableChangeEventT, SensorType, SaHpiSensorTypeType ),
  dStructElement( SaHpiSensorEnableChangeEventT, EventCategory, SaHpiEventCategoryType ),
  dStructElement( SaHpiSensorEnableChangeEventT, SensorEnable, SaHpiBoolType ),
  dStructElement( SaHpiSensorEnableChangeEventT, SensorEventEnable, SaHpiBoolType ),
  dStructElement( SaHpiSensorEnableChangeEventT, AssertEventMask, SaHpiEventStateType ),
  dStructElement( SaHpiSensorEnableChangeEventT, DeassertEventMask, SaHpiEventStateType ),
  dStructElement( SaHpiSensorEnableChangeEventT, OptionalDataPresent, SaHpiSensorEnableOptDataType ),
  dStructElement( SaHpiSensorEnableChangeEventT, CurrentState, SaHpiEventStateType ),
  dStructElementEnd()
};

cMarshalType SaHpiSensorEnableChangeEventType = dStruct( SaHpiSensorEnableChangeEventTElements );


static cMarshalType SaHpiHotSwapEventElements[] =
{
  dStructElement( SaHpiHotSwapEventT, HotSwapState, SaHpiHsStateType ),
  dStructElement( SaHpiHotSwapEventT, PreviousHotSwapState, SaHpiHsStateType ),
  dStructElement( SaHpiHotSwapEventT, CauseOfStateChange, SaHpiHsCauseOfStateChangeType ),
  dStructElementEnd()
};

cMarshalType SaHpiHotSwapEventType = dStruct( SaHpiHotSwapEventElements );


static cMarshalType SaHpiWatchdogEventElements[] =
{
  dStructElement( SaHpiWatchdogEventT, WatchdogNum, SaHpiWatchdogNumType ),
  dStructElement( SaHpiWatchdogEventT, WatchdogAction, SaHpiWatchdogActionEventType ),
  dStructElement( SaHpiWatchdogEventT, WatchdogPreTimerAction, SaHpiWatchdogPretimerInterruptType ),
  dStructElement( SaHpiWatchdogEventT, WatchdogUse, SaHpiWatchdogTimerUseType ),
  dStructElementEnd()
};

cMarshalType SaHpiWatchdogEventType = dStruct( SaHpiWatchdogEventElements );

static cMarshalType SaHpiHpiSwEventTypeElements[] =
{
  dStructElement( SaHpiHpiSwEventT, MId, SaHpiManufacturerIdType ),
  dStructElement( SaHpiHpiSwEventT, Type, SaHpiSwEventTypeType ),
  dStructElement( SaHpiHpiSwEventT, EventData, SaHpiTextBufferType ),
  dStructElementEnd()
};

cMarshalType SaHpiHpiSwEventType = dStruct( SaHpiHpiSwEventTypeElements );

static cMarshalType SaHpiOemEventTypeElements[] =
{
  dStructElement( SaHpiOemEventT, MId, SaHpiManufacturerIdType ),
  dStructElement( SaHpiOemEventT, OemEventData, SaHpiTextBufferType ),
  dStructElementEnd()
};

cMarshalType SaHpiOemEventType = dStruct( SaHpiOemEventTypeElements );

static cMarshalType SaHpiUserEventElements[] =
{
  dStructElement( SaHpiUserEventT, UserEventData, SaHpiTextBufferType ),
  dStructElementEnd()
};

cMarshalType SaHpiUserEventType = dStruct( SaHpiUserEventElements );

static cMarshalType SaHpiDimiEventElements[] =
{
  dStructElement( SaHpiDimiEventT, DimiNum, SaHpiDimiNumType ),
  dStructElement( SaHpiDimiEventT, TestNum, SaHpiDimiTestNumType ),
  dStructElement( SaHpiDimiEventT, DimiTestRunStatus, SaHpiDimiTestRunStatusType ),
  dStructElement( SaHpiDimiEventT, DimiTestPercentCompleted, SaHpiDimiTestPercentCompletedType ),
  dStructElementEnd()
};

cMarshalType SaHpiDimiEventType = dStruct( SaHpiDimiEventElements );


static cMarshalType SaHpiDimiUpdateEventElements[] =
{
  dStructElement( SaHpiDimiUpdateEventT, DimiNum, SaHpiDimiNumType ),
  dStructElementEnd()
};

cMarshalType SaHpiDimiUpdateEventType = dStruct( SaHpiDimiUpdateEventElements );

static cMarshalType SaHpiFumiEventElements[] =
{
  dStructElement( SaHpiFumiEventT, FumiNum, SaHpiFumiNumType ),
  dStructElement( SaHpiFumiEventT, BankNum, SaHpiUint8Type ),
  dStructElement( SaHpiFumiEventT, UpgradeStatus, SaHpiFumiUpgradeStatusType ),
  dStructElementEnd()
};

cMarshalType SaHpiFumiEventType = dStruct( SaHpiFumiEventElements );

static cMarshalType SaHpiEventUnionElements[] =
{
  dUnionElement( SAHPI_ET_RESOURCE, SaHpiResourceEventType ),
  dUnionElement( SAHPI_ET_DOMAIN, SaHpiDomainEventType ),
  dUnionElement( SAHPI_ET_SENSOR, SaHpiSensorEventType ),
  dUnionElement( SAHPI_ET_SENSOR_ENABLE_CHANGE, SaHpiSensorEnableChangeEventType ),
  dUnionElement( SAHPI_ET_HOTSWAP, SaHpiHotSwapEventType ),
  dUnionElement( SAHPI_ET_WATCHDOG, SaHpiWatchdogEventType ),
  dUnionElement( SAHPI_ET_HPI_SW, SaHpiHpiSwEventType ),
  dUnionElement( SAHPI_ET_OEM, SaHpiOemEventType ),
  dUnionElement( SAHPI_ET_USER, SaHpiUserEventType ),
  dUnionElement( SAHPI_ET_DIMI, SaHpiDimiEventType ),
  dUnionElement( SAHPI_ET_DIMI_UPDATE, SaHpiDimiUpdateEventType ),
  dUnionElement( SAHPI_ET_FUMI, SaHpiFumiEventType ),
  dUnionElementEnd()
};

static cMarshalType SaHpiEventUnionType = dUnion( 1, SaHpiEventUnionElements );

static cMarshalType SaHpiEventElements[] =
{
  dStructElement( SaHpiEventT, Source, SaHpiResourceIdType ),
  dStructElement( SaHpiEventT, EventType, SaHpiEventTypeType ),
  dStructElement( SaHpiEventT, Timestamp, SaHpiTimeType ),
  dStructElement( SaHpiEventT, Severity, SaHpiSeverityType ),
  dStructElement( SaHpiEventT, EventDataUnion, SaHpiEventUnionType ),
  dStructElementEnd()
};

cMarshalType SaHpiEventType = dStruct( SaHpiEventElements );


// resource presence table


static cMarshalType GuidDataArray = dArray( 16, SaHpiUint8T, SaHpiUint8Type );

static cMarshalType SaHpiResourceInfoElements[] =
{
	dStructElement( SaHpiResourceInfoT, ResourceRev, SaHpiUint8Type ),
	dStructElement( SaHpiResourceInfoT, SpecificVer, SaHpiUint8Type ),
	dStructElement( SaHpiResourceInfoT, DeviceSupport, SaHpiUint8Type ),
	dStructElement( SaHpiResourceInfoT, ManufacturerId, SaHpiManufacturerIdType ),
	dStructElement( SaHpiResourceInfoT, ProductId, SaHpiUint16Type ),
	dStructElement( SaHpiResourceInfoT, FirmwareMajorRev, SaHpiUint8Type ),
	dStructElement( SaHpiResourceInfoT, FirmwareMinorRev, SaHpiUint8Type ),
	dStructElement( SaHpiResourceInfoT, AuxFirmwareRev, SaHpiUint8Type ),
	dStructElement( SaHpiResourceInfoT, Guid, GuidDataArray ),
	dStructElementEnd()
};

cMarshalType SaHpiResourceInfoType = dStruct( SaHpiResourceInfoElements );


static cMarshalType SaHpiRptEntryElements[] =
{
	dStructElement( SaHpiRptEntryT, EntryId, SaHpiEntryIdType ),
	dStructElement( SaHpiRptEntryT, ResourceId, SaHpiResourceIdType ),
	dStructElement( SaHpiRptEntryT, ResourceInfo, SaHpiResourceInfoType ),	
	dStructElement( SaHpiRptEntryT, ResourceEntity, SaHpiEntityPathType ),
	dStructElement( SaHpiRptEntryT, ResourceCapabilities,SaHpiCapabilitiesType ),
	dStructElement( SaHpiRptEntryT, HotSwapCapabilities, SaHpiHsCapabilitiesType ),
	dStructElement( SaHpiRptEntryT, ResourceSeverity, SaHpiSeverityType ),
	dStructElement( SaHpiRptEntryT, ResourceFailed, SaHpiBoolType ),
	dStructElement( SaHpiRptEntryT, ResourceTag, SaHpiTextBufferType ),	
	dStructElementEnd()
};

cMarshalType SaHpiRptEntryType = dStruct( SaHpiRptEntryElements );


static cMarshalType SaHpiLoadIdElements[] =
{
        dStructElement( SaHpiLoadIdT, LoadNumber, SaHpiLoadNumberType ),
        dStructElement( SaHpiLoadIdT, LoadName, SaHpiTextBufferType ),
        dStructElementEnd()
};

cMarshalType SaHpiLoadIdType = dStruct( SaHpiLoadIdElements );

// domains

static cMarshalType SaHpiDomainInfoTElements[] =
{
	dStructElement( SaHpiDomainInfoT, DomainId, SaHpiDomainIdType ),
	dStructElement( SaHpiDomainInfoT, DomainCapabilities, SaHpiDomainCapabilitiesType ),
	dStructElement( SaHpiDomainInfoT, IsPeer, SaHpiBoolType ),
	dStructElement( SaHpiDomainInfoT, DomainTag, SaHpiTextBufferType ),
	dStructElement( SaHpiDomainInfoT, DrtUpdateCount, SaHpiUint32Type ),
	dStructElement( SaHpiDomainInfoT, DrtUpdateTimestamp, SaHpiTimeType ),
	dStructElement( SaHpiDomainInfoT, RptUpdateCount, SaHpiUint32Type ),
	dStructElement( SaHpiDomainInfoT, RptUpdateTimestamp, SaHpiTimeType ),
	dStructElement( SaHpiDomainInfoT, DatUpdateCount, SaHpiUint32Type ),
	dStructElement( SaHpiDomainInfoT, DatUpdateTimestamp, SaHpiTimeType ),
	dStructElement( SaHpiDomainInfoT, ActiveAlarms, SaHpiUint32Type ),
	dStructElement( SaHpiDomainInfoT, CriticalAlarms, SaHpiUint32Type ),
	dStructElement( SaHpiDomainInfoT, MajorAlarms, SaHpiUint32Type ),
	dStructElement( SaHpiDomainInfoT, MinorAlarms, SaHpiUint32Type ),
	dStructElement( SaHpiDomainInfoT, DatUserAlarmLimit, SaHpiUint32Type ),
	dStructElement( SaHpiDomainInfoT, DatOverflow, SaHpiBoolType ),
	dStructElement( SaHpiDomainInfoT, Guid, GuidDataArray ),
	dStructElementEnd()
};

cMarshalType SaHpiDomainInfoType = dStruct( SaHpiDomainInfoTElements );

static cMarshalType SaHpiDrtEntryTElements[] =
{
	dStructElement( SaHpiDrtEntryT, EntryId, SaHpiEntryIdType ),
	dStructElement( SaHpiDrtEntryT, DomainId, SaHpiDomainIdType ),
	dStructElement( SaHpiDrtEntryT, IsPeer, SaHpiBoolType ),
	dStructElementEnd()
};

cMarshalType SaHpiDrtEntryType = dStruct( SaHpiDrtEntryTElements );

static cMarshalType SaHpiAlarmTElements[] =
{
	dStructElement( SaHpiAlarmT, AlarmId, SaHpiAlarmIdType ),
	dStructElement( SaHpiAlarmT, Timestamp, SaHpiTimeType ),
	dStructElement( SaHpiAlarmT, Severity, SaHpiSeverityType ),
	dStructElement( SaHpiAlarmT, Acknowledged, SaHpiBoolType ),
	dStructElement( SaHpiAlarmT, AlarmCond, SaHpiConditionType ),
	dStructElementEnd()
};

cMarshalType SaHpiAlarmType = dStruct( SaHpiAlarmTElements );


// event log

static cMarshalType SaHpiEventLogInfoTElements[] =
{
	dStructElement( SaHpiEventLogInfoT, Entries, SaHpiUint32Type ),
	dStructElement( SaHpiEventLogInfoT, Size, SaHpiUint32Type ),
	dStructElement( SaHpiEventLogInfoT, UserEventMaxSize, SaHpiUint32Type ),
	dStructElement( SaHpiEventLogInfoT, UpdateTimestamp, SaHpiTimeType ),
	dStructElement( SaHpiEventLogInfoT, CurrentTime, SaHpiTimeType ),
	dStructElement( SaHpiEventLogInfoT, Enabled, SaHpiBoolType ),
	dStructElement( SaHpiEventLogInfoT, OverflowFlag, SaHpiBoolType ),
	dStructElement( SaHpiEventLogInfoT, OverflowResetable, SaHpiBoolType ),
	dStructElement( SaHpiEventLogInfoT, OverflowAction, SaHpiSelOverflowActionType ),
	dStructElementEnd()
};

cMarshalType SaHpiEventLogInfoType = dStruct( SaHpiEventLogInfoTElements );


static cMarshalType SaHpiEventLogEntryElements[] =
{
  dStructElement( SaHpiEventLogEntryT, EntryId, SaHpiEventLogEntryIdType ),
  dStructElement( SaHpiEventLogEntryT, Timestamp, SaHpiTimeType ),
  dStructElement( SaHpiEventLogEntryT, Event, SaHpiEventType ),
  dStructElementEnd()
};

cMarshalType SaHpiEventLogEntryType = dStruct( SaHpiEventLogEntryElements );


// handler info
static cMarshalType plugin_nameBufferArray = dArray( MAX_PLUGIN_NAME_LENGTH, SaHpiUint8T, SaHpiUint8Type );

static cMarshalType oHpiHandlerInfoElements[] =
{
  dStructElement( oHpiHandlerInfoT, id, oHpiHandlerIdType),
  dStructElement( oHpiHandlerInfoT, plugin_name, plugin_nameBufferArray ),
  dStructElement( oHpiHandlerInfoT, entity_root, SaHpiEntityPathType ),
  dStructElement( oHpiHandlerInfoT, load_failed, SaHpiInt32Type ),
  dStructElementEnd()
};

cMarshalType oHpiHandlerInfoType = dStruct( oHpiHandlerInfoElements );


// global param
static cMarshalType GlobalParamPathArray = dArray( OH_PATH_PARAM_MAX_LENGTH, SaHpiUint8T, SaHpiUint8Type );
static cMarshalType GlobalParamVarPathArray = dArray( OH_PATH_PARAM_MAX_LENGTH, SaHpiUint8T, SaHpiUint8Type );
static cMarshalType GlobalParamConfArray = dArray( OH_PATH_PARAM_MAX_LENGTH, SaHpiUint8T, SaHpiUint8Type );

static cMarshalType oHpiGlobalParamUnionTypeElements[] =
{
  dUnionElement( OHPI_ON_EP,            SaHpiEntityPathType ),
  dUnionElement( OHPI_LOG_ON_SEV,       SaHpiSeverityType ),
  dUnionElement( OHPI_EVT_QUEUE_LIMIT,  SaHpiUint32Type ),
  dUnionElement( OHPI_DEL_SIZE_LIMIT,   SaHpiUint32Type ),
  dUnionElement( OHPI_DEL_SAVE,         SaHpiBoolType ),
  dUnionElement( OHPI_DAT_SIZE_LIMIT,   SaHpiUint32Type ),
  dUnionElement( OHPI_DAT_USER_LIMIT,   SaHpiUint32Type ),
  dUnionElement( OHPI_DAT_SAVE,         SaHpiBoolType ),
  dUnionElement( OHPI_PATH,             GlobalParamPathArray ),
  dUnionElement( OHPI_VARPATH,          GlobalParamVarPathArray ),
  dUnionElement( OHPI_CONF,             GlobalParamConfArray ),
  dUnionElementEnd()
};

static cMarshalType oHpiGlobalParamUnionType = dUnion( 0, oHpiGlobalParamUnionTypeElements );

static cMarshalType oHpiGlobalParamTypeElements[] =
{
  dStructElement( oHpiGlobalParamT, Type, oHpiGlobalParamTypeType ),
  dStructElement( oHpiGlobalParamT, u,    oHpiGlobalParamUnionType ),
  dStructElementEnd()
};

cMarshalType oHpiGlobalParamType = dStruct( oHpiGlobalParamTypeElements );

