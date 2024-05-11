/*******************************************************************************
* File Name: LCD_SPIM_SCB_SPI.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the SCB Component in
*  SPI mode.
*
* Note:
*
*******************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LCD_SPIM_SCB_PVT.h"
#include "LCD_SPIM_SCB_SPI_UART_PVT.h"

#if(LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Config Structure Initialization
    ***************************************/

    const LCD_SPIM_SCB_SPI_INIT_STRUCT LCD_SPIM_SCB_configSpi =
    {
        LCD_SPIM_SCB_SPI_MODE,
        LCD_SPIM_SCB_SPI_SUB_MODE,
        LCD_SPIM_SCB_SPI_CLOCK_MODE,
        LCD_SPIM_SCB_SPI_OVS_FACTOR,
        LCD_SPIM_SCB_SPI_MEDIAN_FILTER_ENABLE,
        LCD_SPIM_SCB_SPI_LATE_MISO_SAMPLE_ENABLE,
        LCD_SPIM_SCB_SPI_WAKE_ENABLE,
        LCD_SPIM_SCB_SPI_RX_DATA_BITS_NUM,
        LCD_SPIM_SCB_SPI_TX_DATA_BITS_NUM,
        LCD_SPIM_SCB_SPI_BITS_ORDER,
        LCD_SPIM_SCB_SPI_TRANSFER_SEPARATION,
        0u,
        NULL,
        0u,
        NULL,
        LCD_SPIM_SCB_SPI_INTERRUPT_ENABLE,
        LCD_SPIM_SCB_SPI_INTR_RX_MASK,
        LCD_SPIM_SCB_SPI_RX_TRIGGER_LEVEL,
        LCD_SPIM_SCB_SPI_INTR_TX_MASK,
        LCD_SPIM_SCB_SPI_TX_TRIGGER_LEVEL
    };


    /*******************************************************************************
    * Function Name: LCD_SPIM_SCB_SpiInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for SPI operation.
    *
    * Parameters:
    *  config:  Pointer to a structure that contains the following ordered list of
    *           fields. These fields match the selections available in the
    *           customizer.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LCD_SPIM_SCB_SpiInit(const LCD_SPIM_SCB_SPI_INIT_STRUCT *config)
    {
        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due bad fucntion parameter */
        }
        else
        {
            /* Configure pins */
            LCD_SPIM_SCB_SetPins(LCD_SPIM_SCB_SCB_MODE_SPI, config->mode, LCD_SPIM_SCB_DUMMY_PARAM);

            /* Store internal configuration */
            LCD_SPIM_SCB_scbMode       = (uint8) LCD_SPIM_SCB_SCB_MODE_SPI;
            LCD_SPIM_SCB_scbEnableWake = (uint8) config->enableWake;

            /* Set RX direction internal variables */
            LCD_SPIM_SCB_rxBuffer      =         config->rxBuffer;
            LCD_SPIM_SCB_rxDataBits    = (uint8) config->rxDataBits;
            LCD_SPIM_SCB_rxBufferSize  = (uint8) config->rxBufferSize;

            /* Set TX direction internal variables */
            LCD_SPIM_SCB_txBuffer      =         config->txBuffer;
            LCD_SPIM_SCB_txDataBits    = (uint8) config->txDataBits;
            LCD_SPIM_SCB_txBufferSize  = (uint8) config->txBufferSize;


            /* Configure SPI interface */
            LCD_SPIM_SCB_CTRL_REG     = LCD_SPIM_SCB_GET_CTRL_OVS(config->oversample)        |
                                            LCD_SPIM_SCB_GET_CTRL_EC_AM_MODE(config->enableWake) |
                                            LCD_SPIM_SCB_CTRL_SPI;

            LCD_SPIM_SCB_SPI_CTRL_REG = LCD_SPIM_SCB_GET_SPI_CTRL_CONTINUOUS    (config->transferSeperation)  |
                                            LCD_SPIM_SCB_GET_SPI_CTRL_SELECT_PRECEDE(config->submode & 
                                                                          LCD_SPIM_SCB_SPI_MODE_TI_PRECEDES_MASK) |
                                            LCD_SPIM_SCB_GET_SPI_CTRL_SCLK_MODE     (config->sclkMode)            |
                                            LCD_SPIM_SCB_GET_SPI_CTRL_LATE_MISO_SAMPLE(config->enableLateSampling)|
                                            LCD_SPIM_SCB_GET_SPI_CTRL_SUB_MODE      (config->submode)             |
                                            LCD_SPIM_SCB_GET_SPI_CTRL_MASTER_MODE   (config->mode);

            /* Configure RX direction */
            LCD_SPIM_SCB_RX_CTRL_REG     =  LCD_SPIM_SCB_GET_RX_CTRL_DATA_WIDTH(config->rxDataBits)         |
                                                LCD_SPIM_SCB_GET_RX_CTRL_BIT_ORDER (config->bitOrder)           |
                                                LCD_SPIM_SCB_GET_RX_CTRL_MEDIAN    (config->enableMedianFilter) |
                                                LCD_SPIM_SCB_SPI_RX_CTRL;

            LCD_SPIM_SCB_RX_FIFO_CTRL_REG = LCD_SPIM_SCB_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure TX direction */
            LCD_SPIM_SCB_TX_CTRL_REG      = LCD_SPIM_SCB_GET_TX_CTRL_DATA_WIDTH(config->txDataBits) |
                                                LCD_SPIM_SCB_GET_TX_CTRL_BIT_ORDER (config->bitOrder)   |
                                                LCD_SPIM_SCB_SPI_TX_CTRL;

            LCD_SPIM_SCB_TX_FIFO_CTRL_REG = LCD_SPIM_SCB_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

            /* Configure interrupt sources */
            LCD_SPIM_SCB_INTR_I2C_EC_MASK_REG = LCD_SPIM_SCB_NO_INTR_SOURCES;
            LCD_SPIM_SCB_INTR_SPI_EC_MASK_REG = LCD_SPIM_SCB_NO_INTR_SOURCES;
            LCD_SPIM_SCB_INTR_SLAVE_MASK_REG  = LCD_SPIM_SCB_GET_SPI_INTR_SLAVE_MASK(config->rxInterruptMask);
            LCD_SPIM_SCB_INTR_MASTER_MASK_REG = LCD_SPIM_SCB_GET_SPI_INTR_MASTER_MASK(config->txInterruptMask);
            LCD_SPIM_SCB_INTR_RX_MASK_REG     = LCD_SPIM_SCB_GET_SPI_INTR_RX_MASK(config->rxInterruptMask);
            LCD_SPIM_SCB_INTR_TX_MASK_REG     = LCD_SPIM_SCB_GET_SPI_INTR_TX_MASK(config->txInterruptMask);

            /* Configure interrupt source */
            LCD_SPIM_SCB_SCB_IRQ_StartEx(&LCD_SPIM_SCB_SPI_UART_ISR);
            if(0u == config->enableInterrupt)
            {
                LCD_SPIM_SCB_SCB_IRQ_Disable();
            }


            /* Clear RX buffer indexes */
            LCD_SPIM_SCB_rxBufferHead     = 0u;
            LCD_SPIM_SCB_rxBufferTail     = 0u;
            LCD_SPIM_SCB_rxBufferOverflow = 0u;

            /* Clrea TX buffer indexes */
            LCD_SPIM_SCB_txBufferHead = 0u;
            LCD_SPIM_SCB_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: LCD_SPIM_SCB_SpiInit
    ********************************************************************************
    *
    * Summary:
    *  Configures the SCB for SPI operation.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LCD_SPIM_SCB_SpiInit(void)
    {
        /* Configure SPI interface */
        LCD_SPIM_SCB_CTRL_REG     = LCD_SPIM_SCB_SPI_DEFAULT_CTRL;
        LCD_SPIM_SCB_SPI_CTRL_REG = LCD_SPIM_SCB_SPI_DEFAULT_SPI_CTRL;

        /* Configure TX and RX direction */
        LCD_SPIM_SCB_RX_CTRL_REG      = LCD_SPIM_SCB_SPI_DEFAULT_RX_CTRL;
        LCD_SPIM_SCB_RX_FIFO_CTRL_REG = LCD_SPIM_SCB_SPI_DEFAULT_RX_FIFO_CTRL;

        /* Configure TX and RX direction */
        LCD_SPIM_SCB_TX_CTRL_REG      = LCD_SPIM_SCB_SPI_DEFAULT_TX_CTRL;
        LCD_SPIM_SCB_TX_FIFO_CTRL_REG = LCD_SPIM_SCB_SPI_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt sources */
        LCD_SPIM_SCB_INTR_I2C_EC_MASK_REG = LCD_SPIM_SCB_SPI_DEFAULT_INTR_I2C_EC_MASK;
        LCD_SPIM_SCB_INTR_SPI_EC_MASK_REG = LCD_SPIM_SCB_SPI_DEFAULT_INTR_SPI_EC_MASK;
        LCD_SPIM_SCB_INTR_SLAVE_MASK_REG  = LCD_SPIM_SCB_SPI_DEFAULT_INTR_SLAVE_MASK;
        LCD_SPIM_SCB_INTR_MASTER_MASK_REG = LCD_SPIM_SCB_SPI_DEFAULT_INTR_MASTER_MASK;
        LCD_SPIM_SCB_INTR_RX_MASK_REG     = LCD_SPIM_SCB_SPI_DEFAULT_INTR_RX_MASK;
        LCD_SPIM_SCB_INTR_TX_MASK_REG     = LCD_SPIM_SCB_SPI_DEFAULT_INTR_TX_MASK;


        /* Configure interrupt source */
        #if(LCD_SPIM_SCB_SCB_IRQ_INTERNAL)
            LCD_SPIM_SCB_SCB_IRQ_StartEx(&LCD_SPIM_SCB_SPI_UART_ISR);

            #if(LCD_SPIM_SCB_SPI_INTERRUPT_DISABLE)
                LCD_SPIM_SCB_SCB_IRQ_Disable();
            #endif /* (LCD_SPIM_SCB_SPI_INTERRUPT_DISABLE) */
        #endif /* (LCD_SPIM_SCB_SCB_IRQ_INTERNAL) */


        #if(LCD_SPIM_SCB_INTERNAL_RX_SW_BUFFER_CONST)
            LCD_SPIM_SCB_rxBufferHead     = 0u;
            LCD_SPIM_SCB_rxBufferTail     = 0u;
            LCD_SPIM_SCB_rxBufferOverflow = 0u;
        #endif /* (LCD_SPIM_SCB_INTERNAL_RX_SW_BUFFER_CONST) */

        #if(LCD_SPIM_SCB_INTERNAL_TX_SW_BUFFER_CONST)
            LCD_SPIM_SCB_txBufferHead = 0u;
            LCD_SPIM_SCB_txBufferTail = 0u;
        #endif /* (LCD_SPIM_SCB_INTERNAL_TX_SW_BUFFER_CONST) */
    }

#endif /* (LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_SetActiveSlaveSelect
********************************************************************************
*
* Summary:
*  Selects one of the four SPI slave select signals.
*  The component should be in one of the following states to change the active
*  slave select signal source correctly:
*   - the component is disabled
*   - the component has completed all transactions (TX FIFO is empty and the
*     SpiDone flag is set)
*  This function does not check that these conditions are met.
*  At initialization time the active slave select line is slave select 0.
*
* Parameters:
*  activeSelect: The four lines available to utilize Slave Select function.
*
* Return:
*  None
*
*******************************************************************************/
void LCD_SPIM_SCB_SpiSetActiveSlaveSelect(uint32 activeSelect)
{
    uint32 spiCtrl;
    
    spiCtrl = LCD_SPIM_SCB_SPI_CTRL_REG;
    
    spiCtrl &= (uint32) ~LCD_SPIM_SCB_SPI_CTRL_SLAVE_SELECT_MASK; /* Clear SS bits */
    spiCtrl |= (uint32)  LCD_SPIM_SCB_GET_SPI_CTRL_SS(activeSelect);
    
    LCD_SPIM_SCB_SPI_CTRL_REG = spiCtrl;
}


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_SpiSaveConfig
********************************************************************************
*
* Summary:
*  Wakeup disabled: does nothing.
*  Wakeup  enabled: clears SCB_backup.enableState to keep component enabled
*  while DeepSleep. The SCB_INTR_SPI_EC_WAKE_UP enabled while initialization
*  and tracks in active mode therefore it does not require to be cleared.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void LCD_SPIM_SCB_SpiSaveConfig(void)
{
    #if(LCD_SPIM_SCB_CHECK_SPI_WAKE_ENABLE)
    {
        LCD_SPIM_SCB_backup.enableState = 0u; /* Keep SCB enabled */
        
        /* Clear wake-up before enable */
        LCD_SPIM_SCB_ClearSpiExtClkInterruptSource(LCD_SPIM_SCB_INTR_SPI_EC_WAKE_UP);
        
        /* Enable interrupt to wakeup the device */
        LCD_SPIM_SCB_SetSpiExtClkInterruptMode(LCD_SPIM_SCB_INTR_SPI_EC_WAKE_UP);
    }
    #endif
}


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_SpiRestoreConfig
********************************************************************************
*
* Summary:
*  Does nothing.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  None
*
*******************************************************************************/
void LCD_SPIM_SCB_SpiRestoreConfig(void)
{
    #if(LCD_SPIM_SCB_CHECK_SPI_WAKE_ENABLE)
    {
        /* Disable interrupt to wakeup the device */
        LCD_SPIM_SCB_SetSpiExtClkInterruptMode(LCD_SPIM_SCB_NO_INTR_SOURCES);
    }
    #endif
}


/* [] END OF FILE */
