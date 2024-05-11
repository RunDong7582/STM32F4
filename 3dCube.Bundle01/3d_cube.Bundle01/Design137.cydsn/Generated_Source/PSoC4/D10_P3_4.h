/*******************************************************************************
* File Name: D10_P3_4.h  
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

#if !defined(CY_PINS_D10_P3_4_H) /* Pins D10_P3_4_H */
#define CY_PINS_D10_P3_4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "D10_P3_4_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    D10_P3_4_Write(uint8 value) ;
void    D10_P3_4_SetDriveMode(uint8 mode) ;
uint8   D10_P3_4_ReadDataReg(void) ;
uint8   D10_P3_4_Read(void) ;
uint8   D10_P3_4_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define D10_P3_4_DRIVE_MODE_BITS        (3)
#define D10_P3_4_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - D10_P3_4_DRIVE_MODE_BITS))
#define D10_P3_4_DRIVE_MODE_SHIFT       (0x00u)
#define D10_P3_4_DRIVE_MODE_MASK        (0x07u << D10_P3_4_DRIVE_MODE_SHIFT)

#define D10_P3_4_DM_ALG_HIZ         (0x00u << D10_P3_4_DRIVE_MODE_SHIFT)
#define D10_P3_4_DM_DIG_HIZ         (0x01u << D10_P3_4_DRIVE_MODE_SHIFT)
#define D10_P3_4_DM_RES_UP          (0x02u << D10_P3_4_DRIVE_MODE_SHIFT)
#define D10_P3_4_DM_RES_DWN         (0x03u << D10_P3_4_DRIVE_MODE_SHIFT)
#define D10_P3_4_DM_OD_LO           (0x04u << D10_P3_4_DRIVE_MODE_SHIFT)
#define D10_P3_4_DM_OD_HI           (0x05u << D10_P3_4_DRIVE_MODE_SHIFT)
#define D10_P3_4_DM_STRONG          (0x06u << D10_P3_4_DRIVE_MODE_SHIFT)
#define D10_P3_4_DM_RES_UPDWN       (0x07u << D10_P3_4_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define D10_P3_4_MASK               D10_P3_4__MASK
#define D10_P3_4_SHIFT              D10_P3_4__SHIFT
#define D10_P3_4_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define D10_P3_4_PS                     (* (reg32 *) D10_P3_4__PS)
/* Port Configuration */
#define D10_P3_4_PC                     (* (reg32 *) D10_P3_4__PC)
/* Data Register */
#define D10_P3_4_DR                     (* (reg32 *) D10_P3_4__DR)
/* Input Buffer Disable Override */
#define D10_P3_4_INP_DIS                (* (reg32 *) D10_P3_4__PC2)


#if defined(D10_P3_4__INTSTAT)  /* Interrupt Registers */

    #define D10_P3_4_INTSTAT                (* (reg32 *) D10_P3_4__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins D10_P3_4_H */


/* [] END OF FILE */
