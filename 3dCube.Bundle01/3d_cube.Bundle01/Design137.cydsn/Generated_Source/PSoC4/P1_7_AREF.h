/*******************************************************************************
* File Name: P1_7_AREF.h  
* Version 1.90
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_P1_7_AREF_H) /* Pins P1_7_AREF_H */
#define CY_PINS_P1_7_AREF_H

#include "cytypes.h"
#include "cyfitter.h"
#include "P1_7_AREF_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    P1_7_AREF_Write(uint8 value) ;
void    P1_7_AREF_SetDriveMode(uint8 mode) ;
uint8   P1_7_AREF_ReadDataReg(void) ;
uint8   P1_7_AREF_Read(void) ;
uint8   P1_7_AREF_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define P1_7_AREF_DRIVE_MODE_BITS        (3)
#define P1_7_AREF_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - P1_7_AREF_DRIVE_MODE_BITS))
#define P1_7_AREF_DRIVE_MODE_SHIFT       (0x00u)
#define P1_7_AREF_DRIVE_MODE_MASK        (0x07u << P1_7_AREF_DRIVE_MODE_SHIFT)

#define P1_7_AREF_DM_ALG_HIZ         (0x00u << P1_7_AREF_DRIVE_MODE_SHIFT)
#define P1_7_AREF_DM_DIG_HIZ         (0x01u << P1_7_AREF_DRIVE_MODE_SHIFT)
#define P1_7_AREF_DM_RES_UP          (0x02u << P1_7_AREF_DRIVE_MODE_SHIFT)
#define P1_7_AREF_DM_RES_DWN         (0x03u << P1_7_AREF_DRIVE_MODE_SHIFT)
#define P1_7_AREF_DM_OD_LO           (0x04u << P1_7_AREF_DRIVE_MODE_SHIFT)
#define P1_7_AREF_DM_OD_HI           (0x05u << P1_7_AREF_DRIVE_MODE_SHIFT)
#define P1_7_AREF_DM_STRONG          (0x06u << P1_7_AREF_DRIVE_MODE_SHIFT)
#define P1_7_AREF_DM_RES_UPDWN       (0x07u << P1_7_AREF_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define P1_7_AREF_MASK               P1_7_AREF__MASK
#define P1_7_AREF_SHIFT              P1_7_AREF__SHIFT
#define P1_7_AREF_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define P1_7_AREF_PS                     (* (reg32 *) P1_7_AREF__PS)
/* Port Configuration */
#define P1_7_AREF_PC                     (* (reg32 *) P1_7_AREF__PC)
/* Data Register */
#define P1_7_AREF_DR                     (* (reg32 *) P1_7_AREF__DR)
/* Input Buffer Disable Override */
#define P1_7_AREF_INP_DIS                (* (reg32 *) P1_7_AREF__PC2)


#if defined(P1_7_AREF__INTSTAT)  /* Interrupt Registers */

    #define P1_7_AREF_INTSTAT                (* (reg32 *) P1_7_AREF__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins P1_7_AREF_H */


/* [] END OF FILE */
