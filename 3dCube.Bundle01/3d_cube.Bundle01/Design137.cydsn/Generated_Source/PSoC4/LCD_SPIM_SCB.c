/*******************************************************************************
* File Name: LCD_SPIM_SCB.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the SCB Component.
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

#if(LCD_SPIM_SCB_SCB_MODE_I2C_INC)
    #include "LCD_SPIM_SCB_I2C_PVT.h"
#endif /* (LCD_SPIM_SCB_SCB_MODE_I2C_INC) */

#if(LCD_SPIM_SCB_SCB_MODE_SPI_INC || LCD_SPIM_SCB_SCB_MODE_UART_INC)
    #include "LCD_SPIM_SCB_SPI_UART_PVT.h"
#endif /* (LCD_SPIM_SCB_SCB_MODE_SPI_INC || LCD_SPIM_SCB_SCB_MODE_UART_INC) */


/**********************************
*    Run Time Configuration Vars
**********************************/

/* Stores internal component configuration for unconfigured mode */
#if(LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common config vars */
    volatile uint8 LCD_SPIM_SCB_scbMode = LCD_SPIM_SCB_SCB_MODE_UNCONFIG;
    volatile uint8 LCD_SPIM_SCB_scbEnableWake;

    /* I2C config vars */
    volatile uint8 LCD_SPIM_SCB_mode;
    volatile uint8 LCD_SPIM_SCB_acceptAddr;

    /* SPI/UART config vars */
    volatile uint8 * LCD_SPIM_SCB_rxBuffer;
    volatile uint8   LCD_SPIM_SCB_rxDataBits;
    volatile uint32  LCD_SPIM_SCB_rxBufferSize;

    volatile uint8 * LCD_SPIM_SCB_txBuffer;
    volatile uint8   LCD_SPIM_SCB_txDataBits;
    volatile uint32  LCD_SPIM_SCB_txBufferSize;

#endif /* (LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/**********************************
*     Common SCB Vars
**********************************/

uint8 LCD_SPIM_SCB_initVar = 0u;
cyisraddress LCD_SPIM_SCB_customIntrHandler = NULL;


/***************************************
*    Private Function Prototypes
***************************************/

static void LCD_SPIM_SCB_ScbModeStop(void);


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_Init
********************************************************************************
*
* Summary:
*  Initializes SCB component to operate in one of selected configurations:
*  I2C, SPI, UART, EZI2C or EZSPI.
*  This function does not do any initialization when configuration is set to
*  Unconfigured SCB.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LCD_SPIM_SCB_Init(void)
{
    #if(LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
        if(LCD_SPIM_SCB_SCB_MODE_UNCONFIG_RUNTM_CFG)
        {
            LCD_SPIM_SCB_initVar = 0u; /* Clear init var */
        }
        else
        {
            /* Initialization was done before call */
        }

    #elif(LCD_SPIM_SCB_SCB_MODE_I2C_CONST_CFG)
        LCD_SPIM_SCB_I2CInit();

    #elif(LCD_SPIM_SCB_SCB_MODE_SPI_CONST_CFG)
        LCD_SPIM_SCB_SpiInit();

    #elif(LCD_SPIM_SCB_SCB_MODE_UART_CONST_CFG)
        LCD_SPIM_SCB_UartInit();

    #else

    #endif /* (LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_Enable
********************************************************************************
*
* Summary:
*  Enables SCB component operation.
*  The SCB configuration should be not changed when the component is enabled.
*  Any configuration changes should be made after disabling the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LCD_SPIM_SCB_Enable(void)
{
    #if(LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
        if(!LCD_SPIM_SCB_SCB_MODE_UNCONFIG_RUNTM_CFG)
        {
            /* Enable SCB block, only if already configured */
            LCD_SPIM_SCB_CTRL_REG |= LCD_SPIM_SCB_CTRL_ENABLED;
        }
    #else
        LCD_SPIM_SCB_CTRL_REG |= LCD_SPIM_SCB_CTRL_ENABLED; /* Enable SCB block */
    #endif /* (LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_Start
********************************************************************************
*
* Summary:
*  Invokes SCB_Init() and SCB_Enable().
*  After this function call the component is enabled and ready for operation.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  LCD_SPIM_SCB_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void LCD_SPIM_SCB_Start(void)
{
    if(0u == LCD_SPIM_SCB_initVar)
    {
        LCD_SPIM_SCB_initVar = 1u; /* Component was initialized */
        LCD_SPIM_SCB_Init();       /* Initialize component      */
    }

    LCD_SPIM_SCB_Enable();
}


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_Stop
********************************************************************************
*
* Summary:
*  Disables the SCB component.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LCD_SPIM_SCB_Stop(void)
{
    LCD_SPIM_SCB_CTRL_REG &= (uint32) ~LCD_SPIM_SCB_CTRL_ENABLED;  /* Disable SCB block */

    #if(LCD_SPIM_SCB_SCB_IRQ_INTERNAL)
        LCD_SPIM_SCB_SCB_IRQ_ClearPending(); /* Clear pending interrupt */
    #endif /* (LCD_SPIM_SCB_SCB_IRQ_INTERNAL) */

    LCD_SPIM_SCB_ScbModeStop(); /* Calls scbMode specific Stop function */
}


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_SetCustomInterruptHandler
********************************************************************************
*
* Summary:
*  Registers a function to be called by the internal interrupt handler.
*  First the function that is registered is called, then the internal interrupt
*  handler performs any operations such as software buffer management functions
*  before the interrupt returns.  It is user's responsibility to not break the
*  software buffer operations. Only one custom handler is supported, which is
*  the function provided by the most recent call.
*  At initialization time no custom handler is registered.
*
* Parameters:
*  func: Pointer to the function to register.
*        The value NULL indicates to remove the current custom interrupt
*        handler.
*
* Return:
*  None
*
*******************************************************************************/
void LCD_SPIM_SCB_SetCustomInterruptHandler(cyisraddress func)
{
    LCD_SPIM_SCB_customIntrHandler = func; /* Register interrupt handler */
}


/*******************************************************************************
* Function Name: LCD_SPIM_SCB_ScbModeStop
********************************************************************************
*
* Summary:
*  Calls Stop function for specific operation mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void LCD_SPIM_SCB_ScbModeStop(void)
{
    #if(LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
        if(LCD_SPIM_SCB_SCB_MODE_I2C_RUNTM_CFG)
        {
            LCD_SPIM_SCB_I2CStop();
        }
    #elif(LCD_SPIM_SCB_SCB_MODE_I2C_CONST_CFG)
        LCD_SPIM_SCB_I2CStop();
    #else

    #endif /* (LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */
}



#if(LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: LCD_SPIM_SCB_SetPins
    ********************************************************************************
    *
    * Summary:
    *  Sets pins settings accordingly to selected operation mode.
    *  Only available in Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when specific mode of operation
    *  selected in design time.
    *
    * Parameters:
    *  mode:      Mode of SCB operation.
    *  subMode:   Submode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  uartTxRx:  Direction for UART.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LCD_SPIM_SCB_SetPins(uint32 mode, uint32 subMode, uint32 uartTxRx)
    {
        uint32 hsiomSel[LCD_SPIM_SCB_SCB_PINS_NUMBER];
        uint32 pinsDm  [LCD_SPIM_SCB_SCB_PINS_NUMBER];
        uint32 i;

        /* Make all unused */
        for(i = 0u; i < LCD_SPIM_SCB_SCB_PINS_NUMBER; i++)
        {
            hsiomSel[i] = LCD_SPIM_SCB_HSIOM_DEF_SEL;
            pinsDm[i]   = LCD_SPIM_SCB_PIN_DM_ALG_HIZ;
        }

        /* Choice the Dm and HSIOM */
        if(LCD_SPIM_SCB_SCB_MODE_I2C == mode)
        {
            hsiomSel[LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_I2C_SEL;
            hsiomSel[LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_I2C_SEL;

            pinsDm[LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_OD_LO;
            pinsDm[LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_OD_LO;
        }
        else if(LCD_SPIM_SCB_SCB_MODE_SPI == mode)
        {
            hsiomSel[LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_SPI_SEL;
            hsiomSel[LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_SPI_SEL;
            hsiomSel[LCD_SPIM_SCB_SCLK_PIN_INDEX]        = LCD_SPIM_SCB_HSIOM_SPI_SEL;

            if(LCD_SPIM_SCB_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_DIG_HIZ;
                pinsDm[LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_STRONG;
                pinsDm[LCD_SPIM_SCB_SCLK_PIN_INDEX]        = LCD_SPIM_SCB_PIN_DM_DIG_HIZ;

                #if(LCD_SPIM_SCB_SS0_PIN)
                    /* Only SS0 is valid choice for Slave */
                    hsiomSel[LCD_SPIM_SCB_SS0_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_SPI_SEL;
                    pinsDm  [LCD_SPIM_SCB_SS0_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_DIG_HIZ;
                #endif /* (LCD_SPIM_SCB_SS1_PIN) */
            }
            else /* (Master) */
            {
                pinsDm[LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_STRONG;
                pinsDm[LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_DIG_HIZ;
                pinsDm[LCD_SPIM_SCB_SCLK_PIN_INDEX]        = LCD_SPIM_SCB_PIN_DM_STRONG;

                #if(LCD_SPIM_SCB_SS0_PIN)
                    hsiomSel[LCD_SPIM_SCB_SS0_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_SPI_SEL;
                    pinsDm  [LCD_SPIM_SCB_SS0_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_STRONG;
                #endif /* (LCD_SPIM_SCB_SS0_PIN) */

                #if(LCD_SPIM_SCB_SS1_PIN)
                    hsiomSel[LCD_SPIM_SCB_SS1_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_SPI_SEL;
                    pinsDm  [LCD_SPIM_SCB_SS1_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_STRONG;
                #endif /* (LCD_SPIM_SCB_SS1_PIN) */

                #if(LCD_SPIM_SCB_SS2_PIN)
                    hsiomSel[LCD_SPIM_SCB_SS2_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_SPI_SEL;
                    pinsDm  [LCD_SPIM_SCB_SS2_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_STRONG;
                #endif /* (LCD_SPIM_SCB_SS2_PIN) */

                #if(LCD_SPIM_SCB_SS3_PIN)
                    hsiomSel[LCD_SPIM_SCB_SS3_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_SPI_SEL;
                    pinsDm  [LCD_SPIM_SCB_SS3_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_STRONG;
                #endif /* (LCD_SPIM_SCB_SS2_PIN) */
            }
        }
        else /* UART */
        {
            if(LCD_SPIM_SCB_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
                hsiomSel[LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_UART_SEL;
                pinsDm  [LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_OD_LO;
            }
            else /* Standard or IrDA */
            {
                if(0u != (LCD_SPIM_SCB_UART_RX & uartTxRx))
                {
                    hsiomSel[LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_UART_SEL;
                    pinsDm  [LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_DIG_HIZ;
                }

                if(0u != (LCD_SPIM_SCB_UART_TX & uartTxRx))
                {
                    hsiomSel[LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX] = LCD_SPIM_SCB_HSIOM_UART_SEL;
                    pinsDm  [LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX] = LCD_SPIM_SCB_PIN_DM_STRONG;
                }
            }
        }

        /* Condfigure pins: set HSIOM and DM */
        /* Condfigure pins: DR registers configuration remains unchanged for cyfitter_cfg() */

        #if(LCD_SPIM_SCB_MOSI_SCL_RX_PIN)
            LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_REG,
                                           LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_MASK,
                                           LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_POS,
                                           hsiomSel[LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_PIN) */

        #if(LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN)
            LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_REG,
                                           LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_MASK,
                                           LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_POS,
                                           hsiomSel[LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN) */

        #if(LCD_SPIM_SCB_MISO_SDA_TX_PIN)
            LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_MISO_SDA_TX_HSIOM_REG,
                                           LCD_SPIM_SCB_MISO_SDA_TX_HSIOM_MASK,
                                           LCD_SPIM_SCB_MISO_SDA_TX_HSIOM_POS,
                                           hsiomSel[LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_PIN) */

        #if(LCD_SPIM_SCB_SCLK_PIN)
            LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_SCLK_HSIOM_REG, LCD_SPIM_SCB_SCLK_HSIOM_MASK,
                                           LCD_SPIM_SCB_SCLK_HSIOM_POS, hsiomSel[LCD_SPIM_SCB_SCLK_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SCLK_PIN) */

        #if(LCD_SPIM_SCB_SS0_PIN)
            LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_SS0_HSIOM_REG, LCD_SPIM_SCB_SS0_HSIOM_MASK,
                                           LCD_SPIM_SCB_SS0_HSIOM_POS, hsiomSel[LCD_SPIM_SCB_SS0_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SS1_PIN) */

        #if(LCD_SPIM_SCB_SS1_PIN)
            LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_SS1_HSIOM_REG, LCD_SPIM_SCB_SS1_HSIOM_MASK,
                                           LCD_SPIM_SCB_SS1_HSIOM_POS, hsiomSel[LCD_SPIM_SCB_SS1_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SS1_PIN) */

        #if(LCD_SPIM_SCB_SS2_PIN)
            LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_SS2_HSIOM_REG, LCD_SPIM_SCB_SS2_HSIOM_MASK,
                                           LCD_SPIM_SCB_SS2_HSIOM_POS, hsiomSel[LCD_SPIM_SCB_SS2_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SS2_PIN) */

        #if(LCD_SPIM_SCB_SS3_PIN)
            LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_SS3_HSIOM_REG,  LCD_SPIM_SCB_SS3_HSIOM_MASK,
                                           LCD_SPIM_SCB_SS3_HSIOM_POS, hsiomSel[LCD_SPIM_SCB_SS3_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SS3_PIN) */



        #if(LCD_SPIM_SCB_MOSI_SCL_RX_PIN)
            LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_SetDriveMode((uint8)
                                                                   pinsDm[LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_PIN) */

        #if(LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN)
        LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake_SetDriveMode((uint8)
                                                                   pinsDm[LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN_INDEX]);

        /* Set interrupt on rising edge */
        LCD_SPIM_SCB_SET_INCFG_TYPE(LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_INTCFG_REG,
                                        LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK,
                                        LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS,
                                        LCD_SPIM_SCB_INTCFG_TYPE_FALLING_EDGE);

        #endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN) */

        #if(LCD_SPIM_SCB_MISO_SDA_TX_PIN)
            LCD_SPIM_SCB_spi_miso_i2c_sda_uart_tx_SetDriveMode((uint8)
                                                                        pinsDm[LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_PIN) */

        #if(LCD_SPIM_SCB_SCLK_PIN)
            LCD_SPIM_SCB_spi_sclk_SetDriveMode((uint8) pinsDm[LCD_SPIM_SCB_SCLK_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SCLK_PIN) */

        #if(LCD_SPIM_SCB_SS0_PIN)
            LCD_SPIM_SCB_spi_ss0_SetDriveMode((uint8) pinsDm[LCD_SPIM_SCB_SS0_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SS0_PIN) */

        #if(LCD_SPIM_SCB_SS1_PIN)
            LCD_SPIM_SCB_spi_ss1_SetDriveMode((uint8) pinsDm[LCD_SPIM_SCB_SS1_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SS1_PIN) */

        #if(LCD_SPIM_SCB_SS2_PIN)
            LCD_SPIM_SCB_spi_ss2_SetDriveMode((uint8) pinsDm[LCD_SPIM_SCB_SS2_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SS2_PIN) */

        #if(LCD_SPIM_SCB_SS3_PIN)
            LCD_SPIM_SCB_spi_ss3_SetDriveMode((uint8) pinsDm[LCD_SPIM_SCB_SS3_PIN_INDEX]);
        #endif /* (LCD_SPIM_SCB_SS3_PIN) */
    }

#endif /* (LCD_SPIM_SCB_SCB_MODE_UNCONFIG_CONST_CFG) */


/* [] END OF FILE */
