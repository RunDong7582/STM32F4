/*******************************************************************************
* File Name: LCD_SS.h  
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

#if !defined(CY_PINS_LCD_SS_H) /* Pins LCD_SS_H */
#define CY_PINS_LCD_SS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "LCD_SS_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    LCD_SS_Write(uint8 value) ;
void    LCD_SS_SetDriveMode(uint8 mode) ;
uint8   LCD_SS_ReadDataReg(void) ;
uint8   LCD_SS_Read(void) ;
uint8   LCD_SS_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define LCD_SS_DRIVE_MODE_BITS        (3)
#define LCD_SS_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - LCD_SS_DRIVE_MODE_BITS))
#define LCD_SS_DRIVE_MODE_SHIFT       (0x00u)
#define LCD_SS_DRIVE_MODE_MASK        (0x07u << LCD_SS_DRIVE_MODE_SHIFT)

#define LCD_SS_DM_ALG_HIZ         (0x00u << LCD_SS_DRIVE_MODE_SHIFT)
#define LCD_SS_DM_DIG_HIZ         (0x01u << LCD_SS_DRIVE_MODE_SHIFT)
#define LCD_SS_DM_RES_UP          (0x02u << LCD_SS_DRIVE_MODE_SHIFT)
#define LCD_SS_DM_RES_DWN         (0x03u << LCD_SS_DRIVE_MODE_SHIFT)
#define LCD_SS_DM_OD_LO           (0x04u << LCD_SS_DRIVE_MODE_SHIFT)
#define LCD_SS_DM_OD_HI           (0x05u << LCD_SS_DRIVE_MODE_SHIFT)
#define LCD_SS_DM_STRONG          (0x06u << LCD_SS_DRIVE_MODE_SHIFT)
#define LCD_SS_DM_RES_UPDWN       (0x07u << LCD_SS_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define LCD_SS_MASK               LCD_SS__MASK
#define LCD_SS_SHIFT              LCD_SS__SHIFT
#define LCD_SS_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LCD_SS_PS                     (* (reg32 *) LCD_SS__PS)
/* Port Configuration */
#define LCD_SS_PC                     (* (reg32 *) LCD_SS__PC)
/* Data Register */
#define LCD_SS_DR                     (* (reg32 *) LCD_SS__DR)
/* Input Buffer Disable Override */
#define LCD_SS_INP_DIS                (* (reg32 *) LCD_SS__PC2)


#if defined(LCD_SS__INTSTAT)  /* Interrupt Registers */

    #define LCD_SS_INTSTAT                (* (reg32 *) LCD_SS__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins LCD_SS_H */


/* [] END OF FILE */
