/*******************************************************************************
* File Name: .h
* Version 2.40
*
* Description:
*  This private header file contains internal definitions for the SPIM
*  component. Do not use these definitions directly in your application.
*
* Note:
*
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_PVT_LCD_SPIM_H)
#define CY_SPIM_PVT_LCD_SPIM_H

#include "LCD_SPIM.h"


/**********************************
*   Functions with external linkage
**********************************/


/**********************************
*   Variables with external linkage
**********************************/

extern volatile uint8 LCD_SPIM_swStatusTx;
extern volatile uint8 LCD_SPIM_swStatusRx;

#if(LCD_SPIM_TX_SOFTWARE_BUF_ENABLED)
    extern volatile uint16 LCD_SPIM_txBuffer[LCD_SPIM_TX_BUFFER_SIZE];
    extern volatile uint8 LCD_SPIM_txBufferRead;
    extern volatile uint8 LCD_SPIM_txBufferWrite;
    extern volatile uint8 LCD_SPIM_txBufferFull;
#endif /* (LCD_SPIM_TX_SOFTWARE_BUF_ENABLED) */

#if(LCD_SPIM_RX_SOFTWARE_BUF_ENABLED)
    extern volatile uint16 LCD_SPIM_rxBuffer[LCD_SPIM_RX_BUFFER_SIZE];
    extern volatile uint8 LCD_SPIM_rxBufferRead;
    extern volatile uint8 LCD_SPIM_rxBufferWrite;
    extern volatile uint8 LCD_SPIM_rxBufferFull;
#endif /* (LCD_SPIM_RX_SOFTWARE_BUF_ENABLED) */

#endif /* CY_SPIM_PVT_LCD_SPIM_H */


/* [] END OF FILE */
