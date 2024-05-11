/*******************************************************************************
* File Name: D3.h  
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

#if !defined(CY_PINS_D3_H) /* Pins D3_H */
#define CY_PINS_D3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "D3_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    D3_Write(uint8 value) ;
void    D3_SetDriveMode(uint8 mode) ;
uint8   D3_ReadDataReg(void) ;
uint8   D3_Read(void) ;
uint8   D3_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define D3_DRIVE_MODE_BITS        (3)
#define D3_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - D3_DRIVE_MODE_BITS))
#define D3_DRIVE_MODE_SHIFT       (0x00u)
#define D3_DRIVE_MODE_MASK        (0x07u << D3_DRIVE_MODE_SHIFT)

#define D3_DM_ALG_HIZ         (0x00u << D3_DRIVE_MODE_SHIFT)
#define D3_DM_DIG_HIZ         (0x01u << D3_DRIVE_MODE_SHIFT)
#define D3_DM_RES_UP          (0x02u << D3_DRIVE_MODE_SHIFT)
#define D3_DM_RES_DWN         (0x03u << D3_DRIVE_MODE_SHIFT)
#define D3_DM_OD_LO           (0x04u << D3_DRIVE_MODE_SHIFT)
#define D3_DM_OD_HI           (0x05u << D3_DRIVE_MODE_SHIFT)
#define D3_DM_STRONG          (0x06u << D3_DRIVE_MODE_SHIFT)
#define D3_DM_RES_UPDWN       (0x07u << D3_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define D3_MASK               D3__MASK
#define D3_SHIFT              D3__SHIFT
#define D3_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define D3_PS                     (* (reg32 *) D3__PS)
/* Port Configuration */
#define D3_PC                     (* (reg32 *) D3__PC)
/* Data Register */
#define D3_DR                     (* (reg32 *) D3__DR)
/* Input Buffer Disable Override */
#define D3_INP_DIS                (* (reg32 *) D3__PC2)


#if defined(D3__INTSTAT)  /* Interrupt Registers */

    #define D3_INTSTAT                (* (reg32 *) D3__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins D3_H */


/* [] END OF FILE */
