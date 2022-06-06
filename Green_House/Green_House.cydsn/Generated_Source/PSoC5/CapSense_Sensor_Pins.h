/*******************************************************************************
* File Name: CapSense_Sensor_Pins.h
* Version 3.50
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_Pins_CapSense_Sensor_H)
#define CY_CAPSENSE_CSD_Pins_CapSense_Sensor_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CapSense_Sensor.h"


/***************************************
*        Function Prototypes
***************************************/

void CapSense_Sensor_SetAllSensorsDriveMode(uint8 mode) ;
void CapSense_Sensor_SetAllCmodsDriveMode(uint8 mode) ;
#if (CapSense_Sensor_CURRENT_SOURCE == CapSense_Sensor_EXTERNAL_RB)
    void CapSense_Sensor_SetAllRbsDriveMode(uint8 mode) ;    
#endif  /* (CapSense_Sensor_CURRENT_SOURCE == CapSense_Sensor_EXTERNAL_RB) */   


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define CapSense_Sensor_DM_ALG_HIZ         (PIN_DM_ALG_HIZ)
#define CapSense_Sensor_DM_DIG_HIZ         (PIN_DM_DIG_HIZ)
#define CapSense_Sensor_DM_RES_UP          (PIN_DM_RES_UP)
#define CapSense_Sensor_DM_RES_DWN         (PIN_DM_RES_DWN)
#define CapSense_Sensor_DM_OD_LO           (PIN_DM_OD_LO)
#define CapSense_Sensor_DM_OD_HI           (PIN_DM_OD_HI)
#define CapSense_Sensor_DM_STRONG          (PIN_DM_STRONG)
#define CapSense_Sensor_DM_RES_UPDWN       (PIN_DM_RES_UPDWN)

/* PC registers defines for sensors */
#define CapSense_Sensor_PortCH0__DummyWidget__BTN  0
/* For Cmods*/
#define CapSense_Sensor_CmodCH0_Cmod_CH0       CapSense_Sensor_CmodCH0__Cmod_CH0__PC


#endif /* (CY_CAPSENSE_CSD_Pins_CapSense_Sensor_H) */


/* [] END OF FILE */
