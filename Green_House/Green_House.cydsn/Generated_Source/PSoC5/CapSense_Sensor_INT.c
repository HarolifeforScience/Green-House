/*******************************************************************************
* File Name: CapSense_Sensor_INT.c
* Version 3.50
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for CapSense CSD component.
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
#include "cyapicallbacks.h"


/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START CapSense_Sensor_IsrCH_ISR_intc` */

/* `#END` */


#if (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_ONE_CHANNEL_DESIGN)
    /*******************************************************************************
    * Function Name: CapSense_Sensor_IsrCH0_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor and stores measure result.
    *   2. Stores result of current measure and starts next scan till all 
    *      enabled sensors will be scanned.
    *  This interrupt handler only used in one channel designs.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_Sensor_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  CapSense_Sensor_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(CapSense_Sensor_IsrCH0_ISR)
    {
        #ifdef CapSense_Sensor_ONE_CHANNEL_CH0_ISR_ENTRY_CALLBACK
            CapSense_Sensor_IsrCH0_ISR_EntryCallback();
        #endif /* CapSense_Sensor_ONE_CHANNEL_CH0_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here. */
        /* `#START CapSense_Sensor_ONE_CHANNEL_IsrCH0_ISR_ENTER` */

        /* `#END` */

        /* Save results and disable sensor */
        CapSense_Sensor_PostScan(CapSense_Sensor_sensorIndex);
                
        if ((CapSense_Sensor_csv & CapSense_Sensor_SW_CTRL_SINGLE_SCAN) != 0u)
        {
            CapSense_Sensor_csv &= (uint8)~CapSense_Sensor_SW_STS_BUSY;
        }
        else
        {
            /* Proceed the scanning */
            CapSense_Sensor_sensorIndex = CapSense_Sensor_FindNextSensor(CapSense_Sensor_sensorIndex);
            
            /* Check end of scan */
            if(CapSense_Sensor_sensorIndex < CapSense_Sensor_TOTAL_SENSOR_COUNT)
            {
                CapSense_Sensor_PreScan(CapSense_Sensor_sensorIndex);
            }
            else
            {
                CapSense_Sensor_csv &= (uint8)~CapSense_Sensor_SW_STS_BUSY;
            } 
        }

        /*  Place your Interrupt code here. */
        /* `#START CapSense_Sensor_ONE_CHANNEL_IsrCH0_ISR_EXIT` */

        /* `#END` */

        #ifdef CapSense_Sensor_ONE_CHANNEL_CH0_ISR_EXIT_CALLBACK
            CapSense_Sensor_IsrCH0_ISR_ExitCallback();
        #endif /* CapSense_Sensor_ONE_CHANNEL_CH0_ISR_EXIT_CALLBACK */
    }
 #endif  /* (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_ONE_CHANNEL_DESIGN) */


 #if (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_TWO_CHANNELS_DESIGN)   
    /*******************************************************************************
    * Function Name: CapSense_Sensor_IsrCH0_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor or pair of sensors and stores measure result.
    *   2. Stores results of current measure when complete scan of sensors pair or 
    *      sensor scan and starts next enabled sensors pair or sensor scan. 
    *  This interrupt handler only used in two channel designs.
    *  The synchronization is added to scanning process. The ISR channel 0 or 
    *  channel 1 stores the result and starts next scan. It depends which channel
    *  scans longer. The biggest scan resolution parameter of sensor within pair 
    *  defines scanning time of pair.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_Sensor_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  CapSense_Sensor_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(CapSense_Sensor_IsrCH0_ISR)
    {
        #ifdef CapSense_Sensor_TWO_CHANNEL_CH0_ISR_ENTRY_CALLBACK
            CapSense_Sensor_IsrCH0_ISR_EntryCallback();
        #endif /* CapSense_Sensor_TWO_CHANNEL_CH0_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here. */
        /* `#START CapSense_Sensor_TWO_CHANNELS_IsrCH0_ISR_ENTER` */

        /* `#END` */
        
        CapSense_Sensor_CONTROL_REG &= ~CapSense_Sensor_CTRL_WINDOW_EN__CH0;
        CapSense_Sensor_PostScanCh0(CapSense_Sensor_sensorIndex);
        
        if((CapSense_Sensor_CONTROL_REG & CapSense_Sensor_CTRL_WINDOW_EN__CH1) == 0u)
        {
            if ((CapSense_Sensor_csv & CapSense_Sensor_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                CapSense_Sensor_csv &= (uint8)~CapSense_Sensor_SW_STS_BUSY;
            }
            else
            {
                /* Procced the scanning */
                CapSense_Sensor_sensorIndex = CapSense_Sensor_FindNextPair(CapSense_Sensor_sensorIndex);
                
                /* Check end of scan conditions */
                if((CapSense_Sensor_sensorIndex < CapSense_Sensor_TOTAL_SENSOR_COUNT__CH0) || 
                   (CapSense_Sensor_sensorIndex < CapSense_Sensor_TOTAL_SENSOR_COUNT__CH1))
                {
                    CapSense_Sensor_PreScan(CapSense_Sensor_sensorIndex);
                }
                else
                {
                    CapSense_Sensor_csv &= (uint8)~CapSense_Sensor_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }

		/*  Place your Interrupt code here. */
        /* `#START CapSense_Sensor_TWO_CHANNELS_IsrCH0_ISR_EXIT` */

        /* `#END` */

        #ifdef CapSense_Sensor_TWO_CHANNEL_CH0_ISR_EXIT_CALLBACK
            CapSense_Sensor_IsrCH0_ISR_ExitCallback();
        #endif /* CapSense_Sensor_TWO_CHANNEL_CH0_ISR_EXIT_CALLBACK */
    }

    
    /*******************************************************************************
    * Function Name: CapSense_Sensor_IsrCH1_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor or pair of sensors and stores measure result.
    *   2. Stores results of current measure when complete scan of sensors pair or 
    *      sensor scan and starts next enabled sensors pair or sensor scan. 
    *  This interrupt handler only used in two channel designs.
    *  The synchronization is added to scanning process. The ISR channel 0 or 
    *  channel 1 stores the result and starts next scan. It depends which channel
    *  scans longer. The biggest scan resolution parameter of sensor within pair 
    *  defines scanning time of pair.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_Sensor_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  CapSense_Sensor_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(CapSense_Sensor_IsrCH1_ISR)
    {
        #ifdef CapSense_Sensor_TWO_CHANNEL_CH1_ISR_ENTRY_CALLBACK
            CapSense_Sensor_IsrCH1_ISR_EntryCallback();
        #endif /* CapSense_Sensor_TWO_CHANNEL_CH1_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here */
        /* `#START CapSense_Sensor_TWO_CHANNELS_IsrCH1_ISR_ENTER` */

        /* `#END` */
        
        CapSense_Sensor_CONTROL_REG &= (uint8)~CapSense_Sensor_CTRL_WINDOW_EN__CH1;
        CapSense_Sensor_PostScanCh1(CapSense_Sensor_sensorIndex + CapSense_Sensor_TOTAL_SENSOR_COUNT__CH0);
        
        if((CapSense_Sensor_CONTROL_REG & CapSense_Sensor_CTRL_WINDOW_EN__CH0) == 0u)
        {
            if ((CapSense_Sensor_csv & CapSense_Sensor_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                CapSense_Sensor_csv &= (uint8)~CapSense_Sensor_SW_STS_BUSY;
            }
            else 
            {
                /* Procced the scanning */
                CapSense_Sensor_sensorIndex = CapSense_Sensor_FindNextPair(CapSense_Sensor_sensorIndex);
                
                /* Check end of scan conditions */
                if((CapSense_Sensor_sensorIndex < CapSense_Sensor_TOTAL_SENSOR_COUNT__CH0) || 
                   (CapSense_Sensor_sensorIndex < CapSense_Sensor_TOTAL_SENSOR_COUNT__CH1))
                {
                    CapSense_Sensor_PreScan(CapSense_Sensor_sensorIndex);
                }
                else
                {
                    CapSense_Sensor_csv &= ~CapSense_Sensor_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }

		/*  Place your Interrupt code here */
        /* `#START CapSense_Sensor_TWO_CHANNELS_IsrCH1_ISR_EXIT` */

        /* `#END` */

        #ifdef CapSense_Sensor_TWO_CHANNEL_CH1_ISR_EXIT_CALLBACK
            CapSense_Sensor_IsrCH1_ISR_ExitCallback();
        #endif /* CapSense_Sensor_TWO_CHANNEL_CH1_ISR_EXIT_CALLBACK */
    }
#endif  /* (CapSense_Sensor_DESIGN_TYPE == CapSense_Sensor_TWO_CHANNELS_DESIGN) */


/* [] END OF FILE */
