/*******************************************************************************
* File Name: OneWirePin.h  
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

#if !defined(CY_PINS_OneWirePin_H) /* Pins OneWirePin_H */
#define CY_PINS_OneWirePin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "OneWirePin_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 OneWirePin__PORT == 15 && ((OneWirePin__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    OneWirePin_Write(uint8 value);
void    OneWirePin_SetDriveMode(uint8 mode);
uint8   OneWirePin_ReadDataReg(void);
uint8   OneWirePin_Read(void);
void    OneWirePin_SetInterruptMode(uint16 position, uint16 mode);
uint8   OneWirePin_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the OneWirePin_SetDriveMode() function.
     *  @{
     */
        #define OneWirePin_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define OneWirePin_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define OneWirePin_DM_RES_UP          PIN_DM_RES_UP
        #define OneWirePin_DM_RES_DWN         PIN_DM_RES_DWN
        #define OneWirePin_DM_OD_LO           PIN_DM_OD_LO
        #define OneWirePin_DM_OD_HI           PIN_DM_OD_HI
        #define OneWirePin_DM_STRONG          PIN_DM_STRONG
        #define OneWirePin_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define OneWirePin_MASK               OneWirePin__MASK
#define OneWirePin_SHIFT              OneWirePin__SHIFT
#define OneWirePin_WIDTH              1u

/* Interrupt constants */
#if defined(OneWirePin__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in OneWirePin_SetInterruptMode() function.
     *  @{
     */
        #define OneWirePin_INTR_NONE      (uint16)(0x0000u)
        #define OneWirePin_INTR_RISING    (uint16)(0x0001u)
        #define OneWirePin_INTR_FALLING   (uint16)(0x0002u)
        #define OneWirePin_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define OneWirePin_INTR_MASK      (0x01u) 
#endif /* (OneWirePin__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define OneWirePin_PS                     (* (reg8 *) OneWirePin__PS)
/* Data Register */
#define OneWirePin_DR                     (* (reg8 *) OneWirePin__DR)
/* Port Number */
#define OneWirePin_PRT_NUM                (* (reg8 *) OneWirePin__PRT) 
/* Connect to Analog Globals */                                                  
#define OneWirePin_AG                     (* (reg8 *) OneWirePin__AG)                       
/* Analog MUX bux enable */
#define OneWirePin_AMUX                   (* (reg8 *) OneWirePin__AMUX) 
/* Bidirectional Enable */                                                        
#define OneWirePin_BIE                    (* (reg8 *) OneWirePin__BIE)
/* Bit-mask for Aliased Register Access */
#define OneWirePin_BIT_MASK               (* (reg8 *) OneWirePin__BIT_MASK)
/* Bypass Enable */
#define OneWirePin_BYP                    (* (reg8 *) OneWirePin__BYP)
/* Port wide control signals */                                                   
#define OneWirePin_CTL                    (* (reg8 *) OneWirePin__CTL)
/* Drive Modes */
#define OneWirePin_DM0                    (* (reg8 *) OneWirePin__DM0) 
#define OneWirePin_DM1                    (* (reg8 *) OneWirePin__DM1)
#define OneWirePin_DM2                    (* (reg8 *) OneWirePin__DM2) 
/* Input Buffer Disable Override */
#define OneWirePin_INP_DIS                (* (reg8 *) OneWirePin__INP_DIS)
/* LCD Common or Segment Drive */
#define OneWirePin_LCD_COM_SEG            (* (reg8 *) OneWirePin__LCD_COM_SEG)
/* Enable Segment LCD */
#define OneWirePin_LCD_EN                 (* (reg8 *) OneWirePin__LCD_EN)
/* Slew Rate Control */
#define OneWirePin_SLW                    (* (reg8 *) OneWirePin__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define OneWirePin_PRTDSI__CAPS_SEL       (* (reg8 *) OneWirePin__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define OneWirePin_PRTDSI__DBL_SYNC_IN    (* (reg8 *) OneWirePin__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define OneWirePin_PRTDSI__OE_SEL0        (* (reg8 *) OneWirePin__PRTDSI__OE_SEL0) 
#define OneWirePin_PRTDSI__OE_SEL1        (* (reg8 *) OneWirePin__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define OneWirePin_PRTDSI__OUT_SEL0       (* (reg8 *) OneWirePin__PRTDSI__OUT_SEL0) 
#define OneWirePin_PRTDSI__OUT_SEL1       (* (reg8 *) OneWirePin__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define OneWirePin_PRTDSI__SYNC_OUT       (* (reg8 *) OneWirePin__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(OneWirePin__SIO_CFG)
    #define OneWirePin_SIO_HYST_EN        (* (reg8 *) OneWirePin__SIO_HYST_EN)
    #define OneWirePin_SIO_REG_HIFREQ     (* (reg8 *) OneWirePin__SIO_REG_HIFREQ)
    #define OneWirePin_SIO_CFG            (* (reg8 *) OneWirePin__SIO_CFG)
    #define OneWirePin_SIO_DIFF           (* (reg8 *) OneWirePin__SIO_DIFF)
#endif /* (OneWirePin__SIO_CFG) */

/* Interrupt Registers */
#if defined(OneWirePin__INTSTAT)
    #define OneWirePin_INTSTAT            (* (reg8 *) OneWirePin__INTSTAT)
    #define OneWirePin_SNAP               (* (reg8 *) OneWirePin__SNAP)
    
	#define OneWirePin_0_INTTYPE_REG 		(* (reg8 *) OneWirePin__0__INTTYPE)
#endif /* (OneWirePin__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_OneWirePin_H */


/* [] END OF FILE */
