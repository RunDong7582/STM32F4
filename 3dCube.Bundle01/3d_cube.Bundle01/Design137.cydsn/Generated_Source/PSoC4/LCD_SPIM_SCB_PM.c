/*******************************************************************************
* File Name: LCD_SPIM_SCB_PM.c
* Version 1.0
*
* Description:
*  This file provides the source code to the Power Management support for
*  the SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LCD_SPIM_SCB.h"

#if(LCD_SPIM_SCB_SCB_MODE_I2C_INC)
    #include "LCD_SPIM_SCB_I2C_PVT.h"
#endif /* (LCD_SPIM_SCB_SCB_MODE_I2C_INC) */

#if(LCD_SPIM_SCB_SCB_MODE_SPI_INC || LCD_SPIM_SCB_SCB_MODE_UART_INC)
    #include "LCD_SPIM_SCB_SPI_UART_PVT.h"
#endif /* (LCD_SPIM_SCB_SCB_MODE_SPI_INC || LCD_SPIM_SCB_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

LCD_SPIM_SCB_BACKUP_STRUCT LCD_SPIM_SCB_backup =
{
    0u, /* enableState */
};


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_Sleep
********************************************************************************
*
* Summary:
*  Calls SaveConfig function fucntion for selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LCD_SPIM_SCB_Sleep(void)
{
    LCD_SPIM_SCB_backup.enableState = (uint8) LCD_SPIM_SCB_GET_CTRL_ENABLED;

    #if(LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

        if(LCD_SPIM_SCB_SCB_MODE_I2C_RUNTM_CFG)
        {
            LCD_SPIM_SCB_I2CSaveConfig();
        }
        else if(LCD_SPIM_SCB_SCB_MODE_SPI_RUNTM_CFG)
        {
            LCD_SPIM_SCB_SpiSaveConfig();
        }
        else if(LCD_SPIM_SCB_SCB_MODE_UART_RUNTM_CFG)
        {
            LCD_SPIM_SCB_UartSaveConfig();
        }
        else
        {
            /* Unknown mode: do nothing */
        }

    #elif(LCD_SPIM_SCB_SCB_MODE_I2C_CONST_CFG)
        LCD_SPIM_SCB_I2CSaveConfig();

    #elif(LCD_SPIM_SCB_SCB_MODE_SPI_CONST_CFG)
        LCD_SPIM_SCB_SpiSaveConfig();

    #elif(LCD_SPIM_SCB_SCB_MODE_UART_CONST_CFG)
        LCD_SPIM_SCB_UartSaveConfig();

    #else
        /* Do nothing */

    #endif /* (LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

    if(0u != LCD_SPIM_SCB_backup.enableState)
    {
        LCD_SPIM_SCB_Stop();
    }
}


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_Wakeup
********************************************************************************
*
* Summary:
*  Calls RestoreConfig function fucntion for selected mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LCD_SPIM_SCB_Wakeup(void)
{
    #if(LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

        if(LCD_SPIM_SCB_SCB_MODE_I2C_RUNTM_CFG)
        {
            LCD_SPIM_SCB_I2CRestoreConfig();
        }
        else if(LCD_SPIM_SCB_SCB_MODE_SPI_RUNTM_CFG)
        {
            LCD_SPIM_SCB_SpiRestoreConfig();
        }
        else if(LCD_SPIM_SCB_SCB_MODE_UART_RUNTM_CFG)
        {
            LCD_SPIM_SCB_UartRestoreConfig();
        }
        else
        {
            /* Unknown mode: do nothing */
        }

    #elif(LCD_SPIM_SCB_SCB_MODE_I2C_CONST_CFG)
        LCD_SPIM_SCB_I2CRestoreConfig();

    #elif(LCD_SPIM_SCB_SCB_MODE_SPI_CONST_CFG)
        LCD_SPIM_SCB_SpiRestoreConfig();

    #elif(LCD_SPIM_SCB_SCB_MODE_UART_CONST_CFG)
        LCD_SPIM_SCB_UartRestoreConfig();

    #else
        /* Do nothing */

    #endif /* (LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */

    if(0u != LCD_SPIM_SCB_backup.enableState)
    {
        LCD_SPIM_SCB_Enable();
    }
}


/* [] END OF FILE */
