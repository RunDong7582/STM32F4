/*******************************************************************************
* File Name: LCD_SPIM_SCB_SPI_UART_PVT.h
* Version 1.0
*
* Description:
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_LCD_SPIM_SCB_H)
#define CY_SCB_SPI_UART_PVT_LCD_SPIM_SCB_H

#include "LCD_SPIM_SCB_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if(LCD_SPIM_SCB_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  LCD_SPIM_SCB_rxBufferHead;
    extern volatile uint32  LCD_SPIM_SCB_rxBufferTail;
    extern volatile uint8   LCD_SPIM_SCB_rxBufferOverflow;
#endif /* (LCD_SPIM_SCB_INTERNAL_RX_SW_BUFFER_CONST) */

#if(LCD_SPIM_SCB_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  LCD_SPIM_SCB_txBufferHead;
    extern volatile uint32  LCD_SPIM_SCB_txBufferTail;
#endif /* (LCD_SPIM_SCB_INTERNAL_TX_SW_BUFFER_CONST) */

#if(LCD_SPIM_SCB_INTERNAL_RX_SW_BUFFER)
    extern volatile uint16 LCD_SPIM_SCB_rxBufferInternal[LCD_SPIM_SCB_RX_BUFFER_SIZE];
#endif /* (LCD_SPIM_SCB_INTERNAL_RX_SW_BUFFER) */

#if(LCD_SPIM_SCB_INTERNAL_TX_SW_BUFFER)
    extern volatile uint16 LCD_SPIM_SCB_txBufferInternal[LCD_SPIM_SCB_TX_BUFFER_SIZE];
#endif /* (LCD_SPIM_SCB_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

#if(LCD_SPIM_SCB_SCB_MODE_SPI_CONST_CFG)
    void LCD_SPIM_SCB_SpiInit(void);
#endif /* (LCD_SPIM_SCB_SCB_MODE_SPI_CONST_CFG) */

void LCD_SPIM_SCB_SpiSaveConfig(void);
void LCD_SPIM_SCB_SpiRestoreConfig(void);

#if(LCD_SPIM_SCB_SCB_MODE_UART_CONST_CFG)
    void LCD_SPIM_SCB_UartInit(void);
#endif /* (LCD_SPIM_SCB_SCB_MODE_UART_CONST_CFG) */

void LCD_SPIM_SCB_UartSaveConfig(void);
void LCD_SPIM_SCB_UartRestoreConfig(void);

/* Interrupt processing */
#define LCD_SPIM_SCB_SpiUartEnableIntRx(intSourceMask)  LCD_SPIM_SCB_SetRxInterruptMode(intSourceMask)
#define LCD_SPIM_SCB_SpiUartEnableIntTx(intSourceMask)  LCD_SPIM_SCB_SetTxInterruptMode(intSourceMask)
uint32 LCD_SPIM_SCB_SpiUartDisableIntRx(void);
uint32 LCD_SPIM_SCB_SpiUartDisableIntTx(void);

#endif /* (CY_SCB_SPI_UART_PVT_LCD_SPIM_SCB_H) */


/* [] END OF FILE */
