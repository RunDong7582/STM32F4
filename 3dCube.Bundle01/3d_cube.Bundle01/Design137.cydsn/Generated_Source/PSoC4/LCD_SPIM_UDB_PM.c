/*******************************************************************************
* File Name: LCD_SPIM_UDB_PM.c
* Version 2.40
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LCD_SPIM_UDB_PVT.h"

static LCD_SPIM_UDB_BACKUP_STRUCT LCD_SPIM_UDB_backup =
{
    LCD_SPIM_UDB_DISABLED,
    LCD_SPIM_UDB_BITCTR_INIT,
    #if(CY_UDB_V0)
        LCD_SPIM_UDB_TX_INIT_INTERRUPTS_MASK,
        LCD_SPIM_UDB_RX_INIT_INTERRUPTS_MASK
    #endif /* CY_UDB_V0 */
};


/*******************************************************************************
* Function Name: LCD_SPIM_UDB_SaveConfig
********************************************************************************
*
* Summary:
*  Saves SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LCD_SPIM_UDB_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LCD_SPIM_UDB_SaveConfig(void) 
{
    /* Store Status Mask registers */
    #if(CY_UDB_V0)
       LCD_SPIM_UDB_backup.cntrPeriod      = LCD_SPIM_UDB_COUNTER_PERIOD_REG;
       LCD_SPIM_UDB_backup.saveSrTxIntMask = LCD_SPIM_UDB_TX_STATUS_MASK_REG;
       LCD_SPIM_UDB_backup.saveSrRxIntMask = LCD_SPIM_UDB_RX_STATUS_MASK_REG;
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_UDB_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LCD_SPIM_UDB_backup - used when non-retention registers are restored.
*
* Side Effects:
*  If this API is called without first calling SaveConfig then in the following
*  registers will be default values from Customizer:
*  LCD_SPIM_UDB_STATUS_MASK_REG and LCD_SPIM_UDB_COUNTER_PERIOD_REG.
*
*******************************************************************************/
void LCD_SPIM_UDB_RestoreConfig(void) 
{
    /* Restore the data, saved by SaveConfig() function */
    #if(CY_UDB_V0)
        LCD_SPIM_UDB_COUNTER_PERIOD_REG = LCD_SPIM_UDB_backup.cntrPeriod;
        LCD_SPIM_UDB_TX_STATUS_MASK_REG = ((uint8) LCD_SPIM_UDB_backup.saveSrTxIntMask);
        LCD_SPIM_UDB_RX_STATUS_MASK_REG = ((uint8) LCD_SPIM_UDB_backup.saveSrRxIntMask);
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_UDB_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LCD_SPIM_UDB_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LCD_SPIM_UDB_Sleep(void) 
{
    /* Save components enable state */
    LCD_SPIM_UDB_backup.enableState = ((uint8) LCD_SPIM_UDB_IS_ENABLED);

    LCD_SPIM_UDB_Stop();
    LCD_SPIM_UDB_SaveConfig();
}


/*******************************************************************************
* Function Name: LCD_SPIM_UDB_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LCD_SPIM_UDB_backup - used when non-retention registers are restored.
*  LCD_SPIM_UDB_txBufferWrite - modified every function call - resets to
*  zero.
*  LCD_SPIM_UDB_txBufferRead - modified every function call - resets to
*  zero.
*  LCD_SPIM_UDB_rxBufferWrite - modified every function call - resets to
*  zero.
*  LCD_SPIM_UDB_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LCD_SPIM_UDB_Wakeup(void) 
{
    LCD_SPIM_UDB_RestoreConfig();

    #if(LCD_SPIM_UDB_RX_SOFTWARE_BUF_ENABLED)
        LCD_SPIM_UDB_rxBufferFull  = 0u;
        LCD_SPIM_UDB_rxBufferRead  = 0u;
        LCD_SPIM_UDB_rxBufferWrite = 0u;
    #endif /* (LCD_SPIM_UDB_RX_SOFTWARE_BUF_ENABLED) */

    #if(LCD_SPIM_UDB_TX_SOFTWARE_BUF_ENABLED)
        LCD_SPIM_UDB_txBufferFull  = 0u;
        LCD_SPIM_UDB_txBufferRead  = 0u;
        LCD_SPIM_UDB_txBufferWrite = 0u;
    #endif /* (LCD_SPIM_UDB_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    LCD_SPIM_UDB_ClearFIFO();

    /* Restore components block enable state */
    if(0u != LCD_SPIM_UDB_backup.enableState)
    {
        LCD_SPIM_UDB_Enable();
    }
}


/* [] END OF FILE */
