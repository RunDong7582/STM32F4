/*******************************************************************************
* File Name: LCD_SCLK.h  
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

#if !defined(CY_PINS_LCD_SCLK_H) /* Pins LCD_SCLK_H */
#define CY_PINS_LCD_SCLK_H

#include "cytypes.h"
#include "cyfitter.h"
#include "LCD_SCLK_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    LCD_SCLK_Write(uint8 value) ;
void    LCD_SCLK_SetDriveMode(uint8 mode) ;
uint8   LCD_SCLK_ReadDataReg(void) ;
uint8   LCD_SCLK_Read(void) ;
uint8   LCD_SCLK_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define LCD_SCLK_DRIVE_MODE_BITS        (3)
#define LCD_SCLK_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - LCD_SCLK_DRIVE_MODE_BITS))
#define LCD_SCLK_DRIVE_MODE_SHIFT       (0x00u)
#define LCD_SCLK_DRIVE_MODE_MASK        (0x07u << LCD_SCLK_DRIVE_MODE_SHIFT)

#define LCD_SCLK_DM_ALG_HIZ         (0x00u << LCD_SCLK_DRIVE_MODE_SHIFT)
#define LCD_SCLK_DM_DIG_HIZ         (0x01u << LCD_SCLK_DRIVE_MODE_SHIFT)
#define LCD_SCLK_DM_RES_UP          (0x02u << LCD_SCLK_DRIVE_MODE_SHIFT)
#define LCD_SCLK_DM_RES_DWN         (0x03u << LCD_SCLK_DRIVE_MODE_SHIFT)
#define LCD_SCLK_DM_OD_LO           (0x04u << LCD_SCLK_DRIVE_MODE_SHIFT)
#define LCD_SCLK_DM_OD_HI           (0x05u << LCD_SCLK_DRIVE_MODE_SHIFT)
#define LCD_SCLK_DM_STRONG          (0x06u << LCD_SCLK_DRIVE_MODE_SHIFT)
#define LCD_SCLK_DM_RES_UPDWN       (0x07u << LCD_SCLK_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define LCD_SCLK_MASK               LCD_SCLK__MASK
#define LCD_SCLK_SHIFT              LCD_SCLK__SHIFT
#define LCD_SCLK_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LCD_SCLK_PS                     (* (reg32 *) LCD_SCLK__PS)
/* Port Configuration */
#define LCD_SCLK_PC                     (* (reg32 *) LCD_SCLK__PC)
/* Data Register */
#define LCD_SCLK_DR                     (* (reg32 *) LCD_SCLK__DR)
/* Input Buffer Disable Override */
#define LCD_SCLK_INP_DIS                (* (reg32 *) LCD_SCLK__PC2)


#if defined(LCD_SCLK__INTSTAT)  /* Interrupt Registers */

    #define LCD_SCLK_INTSTAT                (* (reg32 *) LCD_SCLK__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins LCD_SCLK_H */


/* [] END OF FILE */
