/*******************************************************************************
* File Name: LCD_Reset.h  
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

#if !defined(CY_PINS_LCD_Reset_H) /* Pins LCD_Reset_H */
#define CY_PINS_LCD_Reset_H

#include "cytypes.h"
#include "cyfitter.h"
#include "LCD_Reset_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    LCD_Reset_Write(uint8 value) ;
void    LCD_Reset_SetDriveMode(uint8 mode) ;
uint8   LCD_Reset_ReadDataReg(void) ;
uint8   LCD_Reset_Read(void) ;
uint8   LCD_Reset_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define LCD_Reset_DRIVE_MODE_BITS        (3)
#define LCD_Reset_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - LCD_Reset_DRIVE_MODE_BITS))
#define LCD_Reset_DRIVE_MODE_SHIFT       (0x00u)
#define LCD_Reset_DRIVE_MODE_MASK        (0x07u << LCD_Reset_DRIVE_MODE_SHIFT)

#define LCD_Reset_DM_ALG_HIZ         (0x00u << LCD_Reset_DRIVE_MODE_SHIFT)
#define LCD_Reset_DM_DIG_HIZ         (0x01u << LCD_Reset_DRIVE_MODE_SHIFT)
#define LCD_Reset_DM_RES_UP          (0x02u << LCD_Reset_DRIVE_MODE_SHIFT)
#define LCD_Reset_DM_RES_DWN         (0x03u << LCD_Reset_DRIVE_MODE_SHIFT)
#define LCD_Reset_DM_OD_LO           (0x04u << LCD_Reset_DRIVE_MODE_SHIFT)
#define LCD_Reset_DM_OD_HI           (0x05u << LCD_Reset_DRIVE_MODE_SHIFT)
#define LCD_Reset_DM_STRONG          (0x06u << LCD_Reset_DRIVE_MODE_SHIFT)
#define LCD_Reset_DM_RES_UPDWN       (0x07u << LCD_Reset_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define LCD_Reset_MASK               LCD_Reset__MASK
#define LCD_Reset_SHIFT              LCD_Reset__SHIFT
#define LCD_Reset_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LCD_Reset_PS                     (* (reg32 *) LCD_Reset__PS)
/* Port Configuration */
#define LCD_Reset_PC                     (* (reg32 *) LCD_Reset__PC)
/* Data Register */
#define LCD_Reset_DR                     (* (reg32 *) LCD_Reset__DR)
/* Input Buffer Disable Override */
#define LCD_Reset_INP_DIS                (* (reg32 *) LCD_Reset__PC2)


#if defined(LCD_Reset__INTSTAT)  /* Interrupt Registers */

    #define LCD_Reset_INTSTAT                (* (reg32 *) LCD_Reset__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins LCD_Reset_H */


/* [] END OF FILE */
