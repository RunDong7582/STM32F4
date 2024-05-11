/*******************************************************************************
* File Name: D4.h  
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

#if !defined(CY_PINS_D4_H) /* Pins D4_H */
#define CY_PINS_D4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "D4_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    D4_Write(uint8 value) ;
void    D4_SetDriveMode(uint8 mode) ;
uint8   D4_ReadDataReg(void) ;
uint8   D4_Read(void) ;
uint8   D4_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define D4_DRIVE_MODE_BITS        (3)
#define D4_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - D4_DRIVE_MODE_BITS))
#define D4_DRIVE_MODE_SHIFT       (0x00u)
#define D4_DRIVE_MODE_MASK        (0x07u << D4_DRIVE_MODE_SHIFT)

#define D4_DM_ALG_HIZ         (0x00u << D4_DRIVE_MODE_SHIFT)
#define D4_DM_DIG_HIZ         (0x01u << D4_DRIVE_MODE_SHIFT)
#define D4_DM_RES_UP          (0x02u << D4_DRIVE_MODE_SHIFT)
#define D4_DM_RES_DWN         (0x03u << D4_DRIVE_MODE_SHIFT)
#define D4_DM_OD_LO           (0x04u << D4_DRIVE_MODE_SHIFT)
#define D4_DM_OD_HI           (0x05u << D4_DRIVE_MODE_SHIFT)
#define D4_DM_STRONG          (0x06u << D4_DRIVE_MODE_SHIFT)
#define D4_DM_RES_UPDWN       (0x07u << D4_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define D4_MASK               D4__MASK
#define D4_SHIFT              D4__SHIFT
#define D4_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define D4_PS                     (* (reg32 *) D4__PS)
/* Port Configuration */
#define D4_PC                     (* (reg32 *) D4__PC)
/* Data Register */
#define D4_DR                     (* (reg32 *) D4__DR)
/* Input Buffer Disable Override */
#define D4_INP_DIS                (* (reg32 *) D4__PC2)


#if defined(D4__INTSTAT)  /* Interrupt Registers */

    #define D4_INTSTAT                (* (reg32 *) D4__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins D4_H */


/* [] END OF FILE */
