/*******************************************************************************
* File Name: CapSense_Sensor_SMS_Wrapper.c
* Version 3.50
*
* Description:
*  This file provides the source code of wrapper between CapSense CSD component 
*  and Auto Tuning library.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CapSense_Sensor.h"
#include "CapSense_Sensor_CSHL.h"

#if (CapSense_Sensor_TUNING_METHOD == CapSense_Sensor_AUTO_TUNING)

extern uint8 CapSense_Sensor_noiseThreshold[CapSense_Sensor_WIDGET_CSHL_PARAMETERS_COUNT];
extern uint8 CapSense_Sensor_hysteresis[CapSense_Sensor_WIDGET_CSHL_PARAMETERS_COUNT];
extern uint8 CapSense_Sensor_widgetResolution[CapSense_Sensor_WIDGET_RESOLUTION_PARAMETERS_COUNT];
extern const uint8 CYCODE CapSense_Sensor_numberOfSensors[CapSense_Sensor_SENSORS_TBL_SIZE];
extern const uint8 CYCODE CapSense_Sensor_rawDataIndex[CapSense_Sensor_SENSORS_TBL_SIZE];

extern uint8 CapSense_Sensor_fingerThreshold[CapSense_Sensor_WIDGET_CSHL_PARAMETERS_COUNT];
extern uint8 CapSense_Sensor_idacSettings[CapSense_Sensor_TOTAL_SENSOR_COUNT];
extern uint8 CapSense_Sensor_analogSwitchDivider[CapSense_Sensor_TOTAL_SCANSLOT_COUNT];

extern void SMS_LIB_V3_50_CalculateThresholds(uint8 SensorNumber);
extern void SMS_LIB_V3_50_AutoTune1Ch(void);
extern void SMS_LIB_V3_50_AutoTune2Ch(void);

uint8  * SMS_LIB_noiseThreshold = CapSense_Sensor_noiseThreshold;
uint8 * SMS_LIB_hysteresis = CapSense_Sensor_hysteresis;

uint8 * SMS_LIB_widgetResolution = CapSense_Sensor_widgetResolution;

const uint8 CYCODE * SMS_LIB_widgetNumber = CapSense_Sensor_widgetNumber;
const uint8 CYCODE * SMS_LIB_numberOfSensors = CapSense_Sensor_numberOfSensors;
const uint8 CYCODE * SMS_LIB_rawDataIndex = CapSense_Sensor_rawDataIndex;

uint8 * SMS_LIB_fingerThreshold = CapSense_Sensor_fingerThreshold;
uint8 * SMS_LIB_idacSettings = CapSense_Sensor_idacSettings;
uint8 * SMS_LIB_prescaler = CapSense_Sensor_analogSwitchDivider;

uint16 * SMS_LIB_SensorRaw = CapSense_Sensor_sensorRaw;
uint16 * SMS_LIB_SensorBaseline = CapSense_Sensor_sensorBaseline;

const uint8 CYCODE SMS_LIB_SensorSensitivity[] = {
    2, 
};


const uint8 CYCODE SMS_LIB_PrescalerTbl[] = {
    1u, 1u, 1u, 1u, 1u, 1u, 2u, 2u, 2u, 2u, 2u, 3u, 3u, 3u, 3u, 3u, 3u, 4u, 4u, 4u, 4u, 4u, 4u, 5u, 5u, 5u, 5u, 5u, 5u, 6u, 6u, 6u, 
};



uint8 SMS_LIB_Table2[CapSense_Sensor_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table3[CapSense_Sensor_TOTAL_SENSOR_COUNT];
uint16 SMS_LIB_Table4[CapSense_Sensor_TOTAL_SENSOR_COUNT];
uint16 SMS_LIB_Table5[CapSense_Sensor_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table6[CapSense_Sensor_TOTAL_SENSOR_COUNT];
uint8 SMS_LIB_Table7[CapSense_Sensor_TOTAL_SENSOR_COUNT];

uint8 SMS_LIB_Table8[CapSense_Sensor_END_OF_WIDGETS_INDEX];
uint8 SMS_LIB_Table9[CapSense_Sensor_END_OF_WIDGETS_INDEX];

uint8 SMS_LIB_Var1 = (5u);
uint16 SMS_LIB_Var2 = (10738u);

uint8 SMS_LIB_TotalSnsCnt = CapSense_Sensor_TOTAL_SENSOR_COUNT;
uint8 SMS_LIB_TotalScanSlCnt = CapSense_Sensor_TOTAL_SCANSLOT_COUNT;
uint8 SMS_LIB_EndOfWidgInd = CapSense_Sensor_END_OF_WIDGETS_INDEX;

#if (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_TWO_CHANNELS_DESIGN)
    uint8 SMS_LIB_TotalSnsCnt_CH0 = CapSense_Sensor_TOTAL_SENSOR_COUNT__CH0;
    uint8 SMS_LIB_TotalSnsCnt_CH1 = CapSense_Sensor_TOTAL_SENSOR_COUNT__CH1;
#else
    uint8 SMS_LIB_TotalSnsCnt_CH0 = 0u;
    uint8 SMS_LIB_TotalSnsCnt_CH1 = 0u;
#endif  /* (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_TWO_CHANNELS_DESIGN) */

/*******************************************************************************
* Function Name: SMS_LIB_ScanSensor
********************************************************************************
*
* Summary:
*  Wrapper to CapSense_Sensor_ScanSensor function.
*
* Parameters:
*  SensorNumber:  Sensor number.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void SMS_LIB_ScanSensor(uint8 SensorNumber) 
{
    CapSense_Sensor_ScanSensor(SensorNumber);
}

/*******************************************************************************
* Function Name: SMS_LIB_IsBusy
********************************************************************************
*
* Summary:
*  Wrapper to CapSense_Sensor_IsBusy function.
*  
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
uint8 SMS_LIB_IsBusy(void) 
{
    return CapSense_Sensor_IsBusy();
}


/*******************************************************************************
* Function Name: CapSense_Sensor_CalculateThresholds
********************************************************************************
*
* Summary:
*  Wrapper to SMS_LIB_CalculateThresholds function.
*
* Parameters:
*  SensorNumber:  Sensor number.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void CapSense_Sensor_CalculateThresholds(uint8 SensorNumber) 
{
    SMS_LIB_V3_50_CalculateThresholds(SensorNumber);
}


/*******************************************************************************
* Function Name: CapSense_Sensor_AutoTune
********************************************************************************
*
* Summary:
*  Wrapper for SMS_LIB_AutoTune1Ch or SMS_LIB_AutoTune2Ch function.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void CapSense_Sensor_AutoTune(void) 
{
    #if (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_ONE_CHANNEL_DESIGN)
        SMS_LIB_V3_50_AutoTune1Ch();
    #elif (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_TWO_CHANNELS_DESIGN)
        SMS_LIB_V3_50_AutoTune2Ch();
    #endif /* (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_ONE_CHANNEL_DESIGN) */
}

/*******************************************************************************
* Function Name: SMS_LIB_SetPrescaler
********************************************************************************
*
* Summary:
*  Empty wrapper for version compliance.
*
* Parameters:
*  prescaler:  prascaler value.
*
* Return:
*  None
*
*******************************************************************************/
void SMS_LIB_SetPrescaler(uint8 prescaler) 
{
    prescaler = prescaler;
}

void SMS_LIB_V3_50_SetAnalogSwitchesSrc_PRS(void) 
{
	CapSense_Sensor_SetAnalogSwitchesSource(CapSense_Sensor_ANALOG_SWITCHES_SRC_PRS);
}

void SMS_LIB_V3_50_SetAnalogSwitchesSrc_Prescaler(void) 
{
	CapSense_Sensor_SetAnalogSwitchesSource(CapSense_Sensor_ANALOG_SWITCHES_SRC_PRESCALER);
}

#endif  /* (CapSense_Sensor_TUNING_METHOD == CapSense_Sensor_AUTO_TUNING) */


/* [] END OF FILE */
