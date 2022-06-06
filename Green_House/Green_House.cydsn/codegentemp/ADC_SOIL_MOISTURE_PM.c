/*******************************************************************************
* File Name: ADC_SOIL_MOISTURE_PM.c
* Version 3.30
*
* Description:
*  This file provides the power manager source code to the API for the
*  Delta-Sigma ADC Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_SOIL_MOISTURE.h"

static ADC_SOIL_MOISTURE_BACKUP_STRUCT ADC_SOIL_MOISTURE_backup =
{
    ADC_SOIL_MOISTURE_DISABLED,
    ADC_SOIL_MOISTURE_CFG1_DEC_CR
};


/*******************************************************************************
* Function Name: ADC_SOIL_MOISTURE_SaveConfig
********************************************************************************
*
* Summary:
*  Save the register configuration which are not retention.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  ADC_SOIL_MOISTURE_backup:  This global structure variable is used to store
*  configuration registers which are non retention whenever user wants to go
*  sleep mode by calling Sleep() API.
*
*******************************************************************************/
void ADC_SOIL_MOISTURE_SaveConfig(void) 
{
    ADC_SOIL_MOISTURE_backup.deccr = ADC_SOIL_MOISTURE_DEC_CR_REG;
}


/*******************************************************************************
* Function Name: ADC_SOIL_MOISTURE_RestoreConfig
********************************************************************************
*
* Summary:
*  Restore the register configurations which are not retention.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  ADC_SOIL_MOISTURE_backup:  This global structure variable is used to restore
*  configuration registers which are non retention whenever user wants to switch
*  to active power mode by calling Wakeup() API.
*
*******************************************************************************/
void ADC_SOIL_MOISTURE_RestoreConfig(void) 
{
    ADC_SOIL_MOISTURE_DEC_CR_REG = ADC_SOIL_MOISTURE_backup.deccr;
}


/*******************************************************************************
* Function Name: ADC_SOIL_MOISTURE_Sleep
********************************************************************************
*
* Summary:
*  Stops the operation of the block and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  ADC_SOIL_MOISTURE_backup:  The structure field 'enableState' is modified
*  depending on the enable state of the block before entering to sleep mode.
*
*******************************************************************************/
void ADC_SOIL_MOISTURE_Sleep(void) 
{
    /* Save ADC enable state */
    if((ADC_SOIL_MOISTURE_ACT_PWR_DEC_EN == (ADC_SOIL_MOISTURE_PWRMGR_DEC_REG & ADC_SOIL_MOISTURE_ACT_PWR_DEC_EN)) &&
       (ADC_SOIL_MOISTURE_ACT_PWR_DSM_EN == (ADC_SOIL_MOISTURE_PWRMGR_DSM_REG & ADC_SOIL_MOISTURE_ACT_PWR_DSM_EN)))
    {
        /* Component is enabled */
        ADC_SOIL_MOISTURE_backup.enableState = ADC_SOIL_MOISTURE_ENABLED;
        if((ADC_SOIL_MOISTURE_DEC_CR_REG & ADC_SOIL_MOISTURE_DEC_START_CONV) != 0u)
        {   
            /* Conversion is started */
            ADC_SOIL_MOISTURE_backup.enableState |= ADC_SOIL_MOISTURE_STARTED;
        }
		
        /* Stop the configuration */
        ADC_SOIL_MOISTURE_Stop();
    }
    else
    {
        /* Component is disabled */
        ADC_SOIL_MOISTURE_backup.enableState = ADC_SOIL_MOISTURE_DISABLED;
    }

    /* Save the user configuration */
    ADC_SOIL_MOISTURE_SaveConfig();
}


/*******************************************************************************
* Function Name: ADC_SOIL_MOISTURE_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and enables the power to the block.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  ADC_SOIL_MOISTURE_backup:  The structure field 'enableState' is used to
*  restore the enable state of block after wakeup from sleep mode.
*
*******************************************************************************/
void ADC_SOIL_MOISTURE_Wakeup(void) 
{
    /* Restore the configuration */
    ADC_SOIL_MOISTURE_RestoreConfig();

    /* Enables the component operation */
    if(ADC_SOIL_MOISTURE_backup.enableState != ADC_SOIL_MOISTURE_DISABLED)
    {
        ADC_SOIL_MOISTURE_Enable();
        if((ADC_SOIL_MOISTURE_backup.enableState & ADC_SOIL_MOISTURE_STARTED) != 0u)
        {
            ADC_SOIL_MOISTURE_StartConvert();
        }
    } /* Do nothing if component was disable before */
}


/* [] END OF FILE */
