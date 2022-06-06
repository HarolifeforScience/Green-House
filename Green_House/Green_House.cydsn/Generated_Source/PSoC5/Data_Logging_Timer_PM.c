/*******************************************************************************
* File Name: Data_Logging_Timer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Data_Logging_Timer.h"

static Data_Logging_Timer_backupStruct Data_Logging_Timer_backup;


/*******************************************************************************
* Function Name: Data_Logging_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Data_Logging_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Data_Logging_Timer_SaveConfig(void) 
{
    #if (!Data_Logging_Timer_UsingFixedFunction)
        Data_Logging_Timer_backup.TimerUdb = Data_Logging_Timer_ReadCounter();
        Data_Logging_Timer_backup.InterruptMaskValue = Data_Logging_Timer_STATUS_MASK;
        #if (Data_Logging_Timer_UsingHWCaptureCounter)
            Data_Logging_Timer_backup.TimerCaptureCounter = Data_Logging_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Data_Logging_Timer_UDB_CONTROL_REG_REMOVED)
            Data_Logging_Timer_backup.TimerControlRegister = Data_Logging_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Data_Logging_Timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Data_Logging_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Data_Logging_Timer_RestoreConfig(void) 
{   
    #if (!Data_Logging_Timer_UsingFixedFunction)

        Data_Logging_Timer_WriteCounter(Data_Logging_Timer_backup.TimerUdb);
        Data_Logging_Timer_STATUS_MASK =Data_Logging_Timer_backup.InterruptMaskValue;
        #if (Data_Logging_Timer_UsingHWCaptureCounter)
            Data_Logging_Timer_SetCaptureCount(Data_Logging_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Data_Logging_Timer_UDB_CONTROL_REG_REMOVED)
            Data_Logging_Timer_WriteControlRegister(Data_Logging_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Data_Logging_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Data_Logging_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Data_Logging_Timer_Sleep(void) 
{
    #if(!Data_Logging_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Data_Logging_Timer_CTRL_ENABLE == (Data_Logging_Timer_CONTROL & Data_Logging_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Data_Logging_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Data_Logging_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Data_Logging_Timer_Stop();
    Data_Logging_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Data_Logging_Timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Data_Logging_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Data_Logging_Timer_Wakeup(void) 
{
    Data_Logging_Timer_RestoreConfig();
    #if(!Data_Logging_Timer_UDB_CONTROL_REG_REMOVED)
        if(Data_Logging_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Data_Logging_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
