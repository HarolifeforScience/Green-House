/*******************************************************************************
* File Name: OneWirePin2.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_OneWirePin2_H) /* Pins OneWirePin2_H */
#define CY_PINS_OneWirePin2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "OneWirePin2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 OneWirePin2__PORT == 15 && ((OneWirePin2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    OneWirePin2_Write(uint8 value);
void    OneWirePin2_SetDriveMode(uint8 mode);
uint8   OneWirePin2_ReadDataReg(void);
uint8   OneWirePin2_Read(void);
void    OneWirePin2_SetInterruptMode(uint16 position, uint16 mode);
uint8   OneWirePin2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the OneWirePin2_SetDriveMode() function.
     *  @{
     */
        #define OneWirePin2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define OneWirePin2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define OneWirePin2_DM_RES_UP          PIN_DM_RES_UP
        #define OneWirePin2_DM_RES_DWN         PIN_DM_RES_DWN
        #define OneWirePin2_DM_OD_LO           PIN_DM_OD_LO
        #define OneWirePin2_DM_OD_HI           PIN_DM_OD_HI
        #define OneWirePin2_DM_STRONG          PIN_DM_STRONG
        #define OneWirePin2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define OneWirePin2_MASK               OneWirePin2__MASK
#define OneWirePin2_SHIFT              OneWirePin2__SHIFT
#define OneWirePin2_WIDTH              1u

/* Interrupt constants */
#if defined(OneWirePin2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in OneWirePin2_SetInterruptMode() function.
     *  @{
     */
        #define OneWirePin2_INTR_NONE      (uint16)(0x0000u)
        #define OneWirePin2_INTR_RISING    (uint16)(0x0001u)
        #define OneWirePin2_INTR_FALLING   (uint16)(0x0002u)
        #define OneWirePin2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define OneWirePin2_INTR_MASK      (0x01u) 
#endif /* (OneWirePin2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define OneWirePin2_PS                     (* (reg8 *) OneWirePin2__PS)
/* Data Register */
#define OneWirePin2_DR                     (* (reg8 *) OneWirePin2__DR)
/* Port Number */
#define OneWirePin2_PRT_NUM                (* (reg8 *) OneWirePin2__PRT) 
/* Connect to Analog Globals */                                                  
#define OneWirePin2_AG                     (* (reg8 *) OneWirePin2__AG)                       
/* Analog MUX bux enable */
#define OneWirePin2_AMUX                   (* (reg8 *) OneWirePin2__AMUX) 
/* Bidirectional Enable */                                                        
#define OneWirePin2_BIE                    (* (reg8 *) OneWirePin2__BIE)
/* Bit-mask for Aliased Register Access */
#define OneWirePin2_BIT_MASK               (* (reg8 *) OneWirePin2__BIT_MASK)
/* Bypass Enable */
#define OneWirePin2_BYP                    (* (reg8 *) OneWirePin2__BYP)
/* Port wide control signals */                                                   
#define OneWirePin2_CTL                    (* (reg8 *) OneWirePin2__CTL)
/* Drive Modes */
#define OneWirePin2_DM0                    (* (reg8 *) OneWirePin2__DM0) 
#define OneWirePin2_DM1                    (* (reg8 *) OneWirePin2__DM1)
#define OneWirePin2_DM2                    (* (reg8 *) OneWirePin2__DM2) 
/* Input Buffer Disable Override */
#define OneWirePin2_INP_DIS                (* (reg8 *) OneWirePin2__INP_DIS)
/* LCD Common or Segment Drive */
#define OneWirePin2_LCD_COM_SEG            (* (reg8 *) OneWirePin2__LCD_COM_SEG)
/* Enable Segment LCD */
#define OneWirePin2_LCD_EN                 (* (reg8 *) OneWirePin2__LCD_EN)
/* Slew Rate Control */
#define OneWirePin2_SLW                    (* (reg8 *) OneWirePin2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define OneWirePin2_PRTDSI__CAPS_SEL       (* (reg8 *) OneWirePin2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define OneWirePin2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) OneWirePin2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define OneWirePin2_PRTDSI__OE_SEL0        (* (reg8 *) OneWirePin2__PRTDSI__OE_SEL0) 
#define OneWirePin2_PRTDSI__OE_SEL1        (* (reg8 *) OneWirePin2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define OneWirePin2_PRTDSI__OUT_SEL0       (* (reg8 *) OneWirePin2__PRTDSI__OUT_SEL0) 
#define OneWirePin2_PRTDSI__OUT_SEL1       (* (reg8 *) OneWirePin2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define OneWirePin2_PRTDSI__SYNC_OUT       (* (reg8 *) OneWirePin2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(OneWirePin2__SIO_CFG)
    #define OneWirePin2_SIO_HYST_EN        (* (reg8 *) OneWirePin2__SIO_HYST_EN)
    #define OneWirePin2_SIO_REG_HIFREQ     (* (reg8 *) OneWirePin2__SIO_REG_HIFREQ)
    #define OneWirePin2_SIO_CFG            (* (reg8 *) OneWirePin2__SIO_CFG)
    #define OneWirePin2_SIO_DIFF           (* (reg8 *) OneWirePin2__SIO_DIFF)
#endif /* (OneWirePin2__SIO_CFG) */

/* Interrupt Registers */
#if defined(OneWirePin2__INTSTAT)
    #define OneWirePin2_INTSTAT            (* (reg8 *) OneWirePin2__INTSTAT)
    #define OneWirePin2_SNAP               (* (reg8 *) OneWirePin2__SNAP)
    
	#define OneWirePin2_0_INTTYPE_REG 		(* (reg8 *) OneWirePin2__0__INTTYPE)
#endif /* (OneWirePin2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_OneWirePin2_H */


/* [] END OF FILE */
