/*******************************************************************************
* File Name: CapSense_Sensor_TunerHelper.h
* Version 3.50
*
* Description:
*  This file provides constants and structure declarations for the tunner hepl
*  APIs for CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_TUNERHELPER_CapSense_Sensor_H)
#define CY_CAPSENSE_CSD_TUNERHELPER_CapSense_Sensor_H

#include "CapSense_Sensor.h"
#include "CapSense_Sensor_CSHL.h"
#if (CapSense_Sensor_TUNER_API_GENERATE)
    #include "CapSense_Sensor_MBX.h"
    #include "EZI2C.h"
#endif /* (CapSense_Sensor_TUNER_API_GENERATE) */


/***************************************
*     Constants for mailboxes
***************************************/

#define CapSense_Sensor_DEFAULT_MAILBOXES_NUMBER   (1u)


/***************************************
*        Function Prototypes
***************************************/

void CapSense_Sensor_TunerStart(void) ;
void CapSense_Sensor_TunerComm(void) ;

#if (CapSense_Sensor_TUNER_API_GENERATE)
    CapSense_Sensor_NO_STRICT_VOLATILE void CapSense_Sensor_ProcessAllWidgets(volatile CapSense_Sensor_OUTBOX *outbox)
	                                        					;

    extern volatile CapSense_Sensor_MAILBOXES CapSense_Sensor_mailboxesComm;
#endif /* (CapSense_Sensor_TUNER_API_GENERATE) */

#endif  /* (CY_CAPSENSE_CSD_TUNERHELPER_CapSense_Sensor_H)*/


/* [] END OF FILE */
