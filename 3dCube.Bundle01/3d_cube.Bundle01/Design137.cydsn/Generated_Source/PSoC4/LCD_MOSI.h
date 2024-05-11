/*******************************************************************************
* File Name: LCD_MOSI.h  
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

#if !defined(CY_PINS_LCD_MOSI_H) /* Pins LCD_MOSI_H */
#define CY_PINS_LCD_MOSI_H

#include "cytypes.h"
#include "cyfitter.h"
#include "LCD_MOSI_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    LCD_MOSI_Write(uint8 value) ;
void    LCD_MOSI_SetDriveMode(uint8 mode) ;
uint8   LCD_MOSI_ReadDataReg(void) ;
uint8   LCD_MOSI_Read(void) ;
uint8   LCD_MOSI_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define LCD_MOSI_DRIVE_MODE_BITS        (3)
#define LCD_MOSI_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - LCD_MOSI_DRIVE_MODE_BITS))
#define LCD_MOSI_DRIVE_MODE_SHIFT       (0x00u)
#define LCD_MOSI_DRIVE_MODE_MASK        (0x07u << LCD_MOSI_DRIVE_MODE_SHIFT)

#define LCD_MOSI_DM_ALG_HIZ         (0x00u << LCD_MOSI_DRIVE_MODE_SHIFT)
#define LCD_MOSI_DM_DIG_HIZ         (0x01u << LCD_MOSI_DRIVE_MODE_SHIFT)
#define LCD_MOSI_DM_RES_UP          (0x02u << LCD_MOSI_DRIVE_MODE_SHIFT)
#define LCD_MOSI_DM_RES_DWN         (0x03u << LCD_MOSI_DRIVE_MODE_SHIFT)
#define LCD_MOSI_DM_OD_LO           (0x04u << LCD_MOSI_DRIVE_MODE_SHIFT)
#define LCD_MOSI_DM_OD_HI           (0x05u << LCD_MOSI_DRIVE_MODE_SHIFT)
#define LCD_MOSI_DM_STRONG          (0x06u << LCD_MOSI_DRIVE_MODE_SHIFT)
#define LCD_MOSI_DM_RES_UPDWN       (0x07u << LCD_MOSI_DRIVE_MODE_SHIFT)

/* Digital Port Constants */
#define LCD_MOSI_MASK               LCD_MOSI__MASK
#define LCD_MOSI_SHIFT              LCD_MOSI__SHIFT
#define LCD_MOSI_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define LCD_MOSI_PS                     (* (reg32 *) LCD_MOSI__PS)
/* Port Configuration */
#define LCD_MOSI_PC                     (* (reg32 *) LCD_MOSI__PC)
/* Data Register */
#define LCD_MOSI_DR                     (* (reg32 *) LCD_MOSI__DR)
/* Input Buffer Disable Override */
#define LCD_MOSI_INP_DIS                (* (reg32 *) LCD_MOSI__PC2)


#if defined(LCD_MOSI__INTSTAT)  /* Interrupt Registers */

    #define LCD_MOSI_INTSTAT                (* (reg32 *) LCD_MOSI__INTSTAT)

#endif /* Interrupt Registers */

#endif /* End Pins LCD_MOSI_H */


/* [] END OF FILE */
