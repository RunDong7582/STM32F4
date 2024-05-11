/*******************************************************************************
* File Name: LCD_SPIM_IntClock.h
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

#if !defined(CY_CLOCK_LCD_SPIM_IntClock_H)
#define CY_CLOCK_LCD_SPIM_IntClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/

void LCD_SPIM_IntClock_Start(void);
void LCD_SPIM_IntClock_Stop(void);

void LCD_SPIM_IntClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 LCD_SPIM_IntClock_GetDividerRegister(void);
uint8  LCD_SPIM_IntClock_GetFractionalDividerRegister(void);

#define LCD_SPIM_IntClock_Enable()                         LCD_SPIM_IntClock_Start()
#define LCD_SPIM_IntClock_Disable()                        LCD_SPIM_IntClock_Stop()
#define LCD_SPIM_IntClock_SetDividerRegister(clkDivider, reset)  \
                        LCD_SPIM_IntClock_SetFractionalDividerRegister((clkDivider), 0)
#define LCD_SPIM_IntClock_SetDivider(clkDivider)           LCD_SPIM_IntClock_SetDividerRegister((clkDivider), 1)
#define LCD_SPIM_IntClock_SetDividerValue(clkDivider)      LCD_SPIM_IntClock_SetDividerRegister((clkDivider) - 1, 1)


/***************************************
*             Registers
***************************************/

#define LCD_SPIM_IntClock_DIV_REG    (*(reg32 *)LCD_SPIM_IntClock__REGISTER)
#define LCD_SPIM_IntClock_ENABLE_REG LCD_SPIM_IntClock_DIV_REG

#endif /* !defined(CY_CLOCK_LCD_SPIM_IntClock_H) */

/* [] END OF FILE */
