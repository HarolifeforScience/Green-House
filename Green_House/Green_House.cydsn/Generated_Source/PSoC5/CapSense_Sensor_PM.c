/*******************************************************************************
* File Name: CapSense_Sensor_PM.c
* Version 3.50
*
* Description:
*  This file provides Sleep APIs for CapSense CSD Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "CapSense_Sensor.h"

CapSense_Sensor_BACKUP_STRUCT CapSense_Sensor_backup =
{   
    0x00u, /* enableState; */
    
};


/*******************************************************************************
* Function Name: CapSense_Sensor_SaveConfig
********************************************************************************
*
* Summary:
*  Saves customer configuration of CapSense none-retention registers. Resets 
*  all sensors to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Function CapSense_Sensor_SaveConfig disconnects all sensors from the
*  Analog MUX Bus and puts them into inactive state. Call this function
*  during the active scan can cause unpredictable component behavior.
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  CapSense_Sensor_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
* Reentrant:
*  Yes.
*
*******************************************************************************/
void CapSense_Sensor_SaveConfig(void) 
{    

    /* Set CONTROL_REG */
    CapSense_Sensor_backup.ctrlReg = CapSense_Sensor_CONTROL_REG;

    /* Clear all sensors */
    CapSense_Sensor_ClearSensors();
    
    /* The pins disable is customer concern: Cmod and Rb */
}


/*******************************************************************************
* Function Name: CapSense_Sensor_Sleep
********************************************************************************
*
* Summary:
*  Disables Active mode power template bits for number of component used within 
*  CapSense. Calls CapSense_Sensor_SaveConfig() function to save customer 
*  configuration of CapSense none-retention registers and resets all sensors 
*  to an inactive state.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Function CapSense_Sensor_Sleep disconnects all sensors from the
*  Analog MUX Bus and puts them into inactive state. Call this function
*  during the active scan can cause unpredictable component behavior.
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  CapSense_Sensor_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
* Reentrant:
*  No
*
*******************************************************************************/
void CapSense_Sensor_Sleep(void) 
{
    /* Check and save enable state */
    if(CapSense_Sensor_IS_CAPSENSE_ENABLE(CapSense_Sensor_CONTROL_REG))
    {
        CapSense_Sensor_backup.enableState = 1u;
        CapSense_Sensor_Stop();
    }
    else
    {
        CapSense_Sensor_backup.enableState = 0u;
    }
    
    CapSense_Sensor_SaveConfig();
}


/*******************************************************************************
* Function Name: CapSense_Sensor_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores CapSense configuration and non-retention register values.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Must be called only after CapSense_Sensor_SaveConfig() routine. Otherwise 
*  the component configuration will be overwritten with its initial setting.
*  This finction modifies the CONTROL_REG register. 
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  CapSense_Sensor_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void CapSense_Sensor_RestoreConfig(void) 
{       
    /* Set PRS */
    #if (CapSense_Sensor_PRS_OPTIONS == CapSense_Sensor_PRS_8BITS)        
        /* Write FIFO with seed */
        CapSense_Sensor_SEED_COPY_REG = CapSense_Sensor_MEASURE_FULL_RANGE_LOW;               /* F0 register */
    
    #elif (CapSense_Sensor_PRS_OPTIONS == CapSense_Sensor_PRS_16BITS)
        /* Write FIFO with seed */
        CY_SET_REG16(CapSense_Sensor_SEED_COPY_PTR, CapSense_Sensor_MEASURE_FULL_RANGE);      /* F0 register */
                
    #elif (CapSense_Sensor_PRS_OPTIONS == CapSense_Sensor_PRS_16BITS_4X)
        
        /* Write FIFO with seed */
        CapSense_Sensor_SEED_COPY_A__F1_REG = CapSense_Sensor_MEASURE_FULL_RANGE_LOW;         /* F0 register */
        CapSense_Sensor_SEED_COPY_A__F0_REG =CapSense_Sensor_MEASURE_FULL_RANGE_LOW;          /* F1 register */
        
    #else
        /* Do nothing = config without PRS */
    #endif  /* (CapSense_Sensor_PRS_OPTIONS == CapSense_Sensor_PRS_8BITS) */
    
    /* Set the Measure */
    #if (CapSense_Sensor_IMPLEMENTATION_CH0 == CapSense_Sensor_MEASURE_IMPLEMENTATION_FF)
        /* Window PWM  - FF Timer register are retention */
        /* Raw Counter - FF Timer register are retention */
    #else
        
        /* Window PWM */
        CapSense_Sensor_PWM_CH0_PERIOD_LO_REG    = CapSense_Sensor_MEASURE_FULL_RANGE_LOW;        /* F0 register */
        
        /* Raw Counter */
        CapSense_Sensor_RAW_CH0_PERIOD_HI_REG    = CapSense_Sensor_MEASURE_FULL_RANGE_LOW;        /* F1 register */
        CapSense_Sensor_RAW_CH0_PERIOD_LO_REG    = CapSense_Sensor_MEASURE_FULL_RANGE_LOW;        /* F0 register */
    
    #endif  /* (CapSense_Sensor_IMPLEMENTATION_CH0 == CapSense_Sensor_MEASURE_IMPLEMENTATION_FF) */ 
    
    #if (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_TWO_CHANNELS_DESIGN)
        #if (CapSense_Sensor_IMPLEMENTATION_CH1 == CapSense_Sensor_MEASURE_IMPLEMENTATION_FF)
            /* Window PWM  - FF Timer register are retention */
            /* Raw Counter - FF Timer register are retention */
        #else
            
            /* Window PWM */
            CapSense_Sensor_PWM_CH1_PERIOD_LO_REG    = CapSense_Sensor_MEASURE_FULL_RANGE_LOW;       /* F0 register */
            
            /* Raw Counter */
            CapSense_Sensor_RAW_CH1_PERIOD_HI_REG    = CapSense_Sensor_MEASURE_FULL_RANGE_LOW;       /* F1 register */
            CapSense_Sensor_RAW_CH1_PERIOD_LO_REG    = CapSense_Sensor_MEASURE_FULL_RANGE_LOW;       /* F0 register */
            
        #endif  /* (CapSense_Sensor_IMPLEMENTATION_CH1 == CapSense_Sensor_MEASURE_IMPLEMENTATION_FF) */
    
    #endif  /* (CapSense_Sensor_DESIGN_TYPE == TWO_CHANNELS_DESIGN)*/

    /* Set CONTROL_REG */
    CapSense_Sensor_CONTROL_REG = CapSense_Sensor_backup.ctrlReg;

    /* Enable window generation */
    #if (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_ONE_CHANNEL_DESIGN)
        CapSense_Sensor_CONTROL_REG |= CapSense_Sensor_CTRL_WINDOW_EN__CH0;
    #elif (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_TWO_CHANNELS_DESIGN) 
        CapSense_Sensor_CONTROL_REG |= (CapSense_Sensor_CTRL_WINDOW_EN__CH0 | CapSense_Sensor_CTRL_WINDOW_EN__CH1); 
    #endif  /* CapSense_Sensor_DESIGN_TYPE */
 
    /* The pins enable are customer concern: Cmod and Rb */
 }
 

/*******************************************************************************
* Function Name: CapSense_Sensor_Wakeup
********************************************************************************
*
* Summary:
*  Restores CapSense configuration and non-retention register values. 
*  Restores enabled state of component by setting Active mode power template 
*  bits for number of component used within CapSense.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Must be called only after CapSense_Sensor_SaveConfig() routine. Otherwise 
*  the component configuration will be overwritten with its initial setting.
*  This finction modifies the CONTROL_REG register. 
*
* Note:
*  This function should be called after completion of all scans.
*
* Global Variables:
*  CapSense_Sensor_backup - used to save component state before enter sleep 
*  mode and none-retention registers.
*
*******************************************************************************/
void CapSense_Sensor_Wakeup(void) 
{
    CapSense_Sensor_RestoreConfig();
    
    /* Restore CapSense Enable state */
    if (CapSense_Sensor_backup.enableState != 0u)
    {
        CapSense_Sensor_Enable();
    }
}


/* [] END OF FILE */
