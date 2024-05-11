/*******************************************************************************
* File Name: LCD_SPIM_SCB_SCBCLK.h
* Version 2.0
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_LCD_SPIM_SCB_SCBCLK_H)
#define CY_CLOCK_LCD_SPIM_SCB_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/

void LCD_SPIM_SCB_SCBCLK_Start(void);
void LCD_SPIM_SCB_SCBCLK_Stop(void);

void LCD_SPIM_SCB_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 LCD_SPIM_SCB_SCBCLK_GetDividerRegister(void);
uint8  LCD_SPIM_SCB_SCBCLK_GetFractionalDividerRegister(void);

#define LCD_SPIM_SCB_SCBCLK_Enable()                         LCD_SPIM_SCB_SCBCLK_Start()
#define LCD_SPIM_SCB_SCBCLK_Disable()                        LCD_SPIM_SCB_SCBCLK_Stop()
#define LCD_SPIM_SCB_SCBCLK_SetDividerRegister(clkDivider, reset)  \
                        LCD_SPIM_SCB_SCBCLK_SetFractionalDividerRegister((clkDivider), 0)
#define LCD_SPIM_SCB_SCBCLK_SetDivider(clkDivider)           LCD_SPIM_SCB_SCBCLK_SetDividerRegister((clkDivider), 1)
#define LCD_SPIM_SCB_SCBCLK_SetDividerValue(clkDivider)      LCD_SPIM_SCB_SCBCLK_SetDividerRegister((clkDivider) - 1, 1)


/***************************************
*             Registers
***************************************/

#define LCD_SPIM_SCB_SCBCLK_DIV_REG    (*(reg32 *)LCD_SPIM_SCB_SCBCLK__REGISTER)
#define LCD_SPIM_SCB_SCBCLK_ENABLE_REG LCD_SPIM_SCB_SCBCLK_DIV_REG

#endif /* !defined(CY_CLOCK_LCD_SPIM_SCB_SCBCLK_H) */

/* [] END OF FILE */
