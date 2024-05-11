/*******************************************************************************
* File Name: LCD_SPIM_SCB_PINS.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_LCD_SPIM_SCB_H)
#define CY_SCB_PINS_LCD_SPIM_SCB_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define LCD_SPIM_SCB_REMOVE_UART_RX_WAKE_PIN        (1u)
#define LCD_SPIM_SCB_REMOVE_MOSI_SCL_RX_WAKE_PIN    (1u)
#define LCD_SPIM_SCB_REMOVE_MOSI_SCL_RX_PIN         (1u)
#define LCD_SPIM_SCB_REMOVE_MISO_SDA_TX_PIN         (1u)
#define LCD_SPIM_SCB_REMOVE_SCLK_PIN                (1u)
#define LCD_SPIM_SCB_REMOVE_SS0_PIN                 (1u)
#define LCD_SPIM_SCB_REMOVE_SS1_PIN                 (1u)
#define LCD_SPIM_SCB_REMOVE_SS2_PIN                 (1u)
#define LCD_SPIM_SCB_REMOVE_SS3_PIN                 (1u)
#define LCD_SPIM_SCB_REMOVE_I2C_PINS                (1u)
#define LCD_SPIM_SCB_REMOVE_SPI_SLAVE_PINS          (1u)

#define LCD_SPIM_SCB_UART_RX_WAKE_PIN       (0u == LCD_SPIM_SCB_REMOVE_UART_RX_WAKE_PIN)
#define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN   (0u == LCD_SPIM_SCB_REMOVE_MOSI_SCL_RX_WAKE_PIN)
#define LCD_SPIM_SCB_MOSI_SCL_RX_PIN        (0u == LCD_SPIM_SCB_REMOVE_MOSI_SCL_RX_PIN)
#define LCD_SPIM_SCB_MISO_SDA_TX_PIN        (0u == LCD_SPIM_SCB_REMOVE_MISO_SDA_TX_PIN)
#define LCD_SPIM_SCB_SCLK_PIN               (0u == LCD_SPIM_SCB_REMOVE_SCLK_PIN)
#define LCD_SPIM_SCB_SS0_PIN                (0u == LCD_SPIM_SCB_REMOVE_SS0_PIN)
#define LCD_SPIM_SCB_SS1_PIN                (0u == LCD_SPIM_SCB_REMOVE_SS1_PIN)
#define LCD_SPIM_SCB_SS2_PIN                (0u == LCD_SPIM_SCB_REMOVE_SS2_PIN)
#define LCD_SPIM_SCB_SS3_PIN                (0u == LCD_SPIM_SCB_REMOVE_SS3_PIN)
#define LCD_SPIM_SCB_I2C_PINS               (0u == LCD_SPIM_SCB_REMOVE_I2C_PINS)
#define LCD_SPIM_SCB_SPI_SLAVE_PINS         (0u == LCD_SPIM_SCB_REMOVE_SPI_SLAVE_PINS)


/***************************************
*             Includes
****************************************/

#if(LCD_SPIM_SCB_UART_RX_WAKE_PIN)
    #include "LCD_SPIM_SCB_rx_wake.h"
#endif /* (LCD_SPIM_SCB_UART_RX_WAKE_PIN) */

#if(LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN)
    #include "LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake.h"
#endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN) */

#if(LCD_SPIM_SCB_MOSI_SCL_RX_PIN)
    #include "LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx.h"
#endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_PIN) */

#if(LCD_SPIM_SCB_MISO_SDA_TX_PIN)
    #include "LCD_SPIM_SCB_spi_miso_i2c_sda_uart_tx.h"
#endif /* (LCD_SPIM_SCB_MISO_SDA_TX_PIN_PIN) */

#if(LCD_SPIM_SCB_SCLK_PIN)
    #include "LCD_SPIM_SCB_spi_sclk.h"
#endif /* (LCD_SPIM_SCB_SCLK_PIN) */

#if(LCD_SPIM_SCB_SS0_PIN)
    #include "LCD_SPIM_SCB_spi_ss0.h"
#endif /* (LCD_SPIM_SCB_SS1_PIN) */

#if(LCD_SPIM_SCB_SS1_PIN)
    #include "LCD_SPIM_SCB_spi_ss1.h"
#endif /* (LCD_SPIM_SCB_SS1_PIN) */

#if(LCD_SPIM_SCB_SS2_PIN)
    #include "LCD_SPIM_SCB_spi_ss2.h"
#endif /* (LCD_SPIM_SCB_SS2_PIN) */

#if(LCD_SPIM_SCB_SS3_PIN)
    #include "LCD_SPIM_SCB_spi_ss3.h"
#endif /* (LCD_SPIM_SCB_SS3_PIN) */

#if(LCD_SPIM_SCB_I2C_PINS)
    #include "LCD_SPIM_SCB_scl.h"
    #include "LCD_SPIM_SCB_sda.h"
#endif /* (LCD_SPIM_SCB_I2C_PINS) */


/***************************************
*              Registers
***************************************/

#if(LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_REG  \
                                                (*(reg32 *) LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_PTR  \
                                                ( (reg32 *) LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_MASK \
                                                (LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_POS  \
                                                (LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake__0__HSIOM_SHIFT)

    #define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_INTCFG_REG    (*(reg32 *) \
                                                              LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_INTCFG_PTR    ( (reg32 *) \
                                                              LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake__0__INTCFG)

    #define LCD_SPIM_SCB_INTCFG_TYPE_MASK                  (0x03u)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS  (LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake__SHIFT)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK ((uint32)                                           \
                                                                    ((uint32) LCD_SPIM_SCB_INTCFG_TYPE_MASK << \
                                                                    LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS))
#endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN) */

#if(LCD_SPIM_SCB_MOSI_SCL_RX_PIN)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_REG      (*(reg32 *) LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_PTR      ( (reg32 *) LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx__0__HSIOM)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_MASK     (LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_POS      (LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx__0__HSIOM_SHIFT)
#endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_PIN) */

#if(LCD_SPIM_SCB_MISO_SDA_TX_PIN)
    #define LCD_SPIM_SCB_MISO_SDA_TX_HSIOM_REG      (*(reg32 *) LCD_SPIM_SCB_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define LCD_SPIM_SCB_MISO_SDA_TX_HSIOM_PTR      ( (reg32 *) LCD_SPIM_SCB_spi_miso_i2c_sda_uart_tx__0__HSIOM)
    #define LCD_SPIM_SCB_MISO_SDA_TX_HSIOM_MASK     (LCD_SPIM_SCB_spi_miso_i2c_sda_uart_tx__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_MISO_SDA_TX_HSIOM_POS      (LCD_SPIM_SCB_spi_miso_i2c_sda_uart_tx__0__HSIOM_SHIFT)
#endif /* (LCD_SPIM_SCB_MISO_SDA_TX_PIN_PIN) */

#if(LCD_SPIM_SCB_SCLK_PIN)
    #define LCD_SPIM_SCB_SCLK_HSIOM_REG     (*(reg32 *) LCD_SPIM_SCB_spi_sclk__0__HSIOM)
    #define LCD_SPIM_SCB_SCLK_HSIOM_PTR     ( (reg32 *) LCD_SPIM_SCB_spi_sclk__0__HSIOM)
    #define LCD_SPIM_SCB_SCLK_HSIOM_MASK    (LCD_SPIM_SCB_spi_sclk__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_SCLK_HSIOM_POS     (LCD_SPIM_SCB_spi_sclk__0__HSIOM_SHIFT)
#endif /* (LCD_SPIM_SCB_SCLK_PIN) */

#if(LCD_SPIM_SCB_SS0_PIN)
    #define LCD_SPIM_SCB_SS0_HSIOM_REG      (*(reg32 *) LCD_SPIM_SCB_spi_ss0__0__HSIOM)
    #define LCD_SPIM_SCB_SS0_HSIOM_PTR      ( (reg32 *) LCD_SPIM_SCB_spi_ss0__0__HSIOM)
    #define LCD_SPIM_SCB_SS0_HSIOM_MASK     (LCD_SPIM_SCB_spi_ss0__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_SS0_HSIOM_POS      (LCD_SPIM_SCB_spi_ss0__0__HSIOM_SHIFT)
#endif /* (LCD_SPIM_SCB_SS1_PIN) */

#if(LCD_SPIM_SCB_SS1_PIN)
    #define LCD_SPIM_SCB_SS1_HSIOM_REG      (*(reg32 *) LCD_SPIM_SCB_spi_ss1__0__HSIOM)
    #define LCD_SPIM_SCB_SS1_HSIOM_PTR      ( (reg32 *) LCD_SPIM_SCB_spi_ss1__0__HSIOM)
    #define LCD_SPIM_SCB_SS1_HSIOM_MASK     (LCD_SPIM_SCB_spi_ss1__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_SS1_HSIOM_POS      (LCD_SPIM_SCB_spi_ss1__0__HSIOM_SHIFT)
#endif /* (LCD_SPIM_SCB_SS1_PIN) */

#if(LCD_SPIM_SCB_SS2_PIN)
    #define LCD_SPIM_SCB_SS2_HSIOM_REG     (*(reg32 *) LCD_SPIM_SCB_spi_ss2__0__HSIOM)
    #define LCD_SPIM_SCB_SS2_HSIOM_PTR     ( (reg32 *) LCD_SPIM_SCB_spi_ss2__0__HSIOM)
    #define LCD_SPIM_SCB_SS2_HSIOM_MASK    (LCD_SPIM_SCB_spi_ss2__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_SS2_HSIOM_POS     (LCD_SPIM_SCB_spi_ss2__0__HSIOM_SHIFT)
#endif /* (LCD_SPIM_SCB_SS2_PIN) */

#if(LCD_SPIM_SCB_SS3_PIN)
    #define LCD_SPIM_SCB_SS3_HSIOM_REG     (*(reg32 *) LCD_SPIM_SCB_spi_ss3__0__HSIOM)
    #define LCD_SPIM_SCB_SS3_HSIOM_PTR     ( (reg32 *) LCD_SPIM_SCB_spi_ss3__0__HSIOM)
    #define LCD_SPIM_SCB_SS3_HSIOM_MASK    (LCD_SPIM_SCB_spi_ss3__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_SS3_HSIOM_POS     (LCD_SPIM_SCB_spi_ss3__0__HSIOM_SHIFT)
#endif /* (LCD_SPIM_SCB_SS3_PIN) */

#if(LCD_SPIM_SCB_I2C_PINS)
    #define LCD_SPIM_SCB_SCL_HSIOM_REG     (*(reg32 *) LCD_SPIM_SCB_scl__0__HSIOM)
    #define LCD_SPIM_SCB_SCL_HSIOM_PTR     ( (reg32 *) LCD_SPIM_SCB_scl__0__HSIOM)
    #define LCD_SPIM_SCB_SCL_HSIOM_MASK    (LCD_SPIM_SCB_scl__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_SCL_HSIOM_POS     (LCD_SPIM_SCB_scl__0__HSIOM_SHIFT)
    
    #define LCD_SPIM_SCB_SDA_HSIOM_REG     (*(reg32 *) LCD_SPIM_SCB_sda__0__HSIOM)
    #define LCD_SPIM_SCB_SDA_HSIOM_PTR     ( (reg32 *) LCD_SPIM_SCB_sda__0__HSIOM)
    #define LCD_SPIM_SCB_SDA_HSIOM_MASK    (LCD_SPIM_SCB_sda__0__HSIOM_MASK)
    #define LCD_SPIM_SCB_SDA_HSIOM_POS     (LCD_SPIM_SCB_sda__0__HSIOM_SHIFT)
#endif /* (LCD_SPIM_SCB_I2C_PINS) */


/***************************************
*        Registers Constants
***************************************/

/* Pins constanst */
#define LCD_SPIM_SCB_HSIOM_DEF_SEL      (0x00u)
#define LCD_SPIM_SCB_HSIOM_GPIO_SEL     (0x00u)
#define LCD_SPIM_SCB_HSIOM_UART_SEL     (0x09u)
#define LCD_SPIM_SCB_HSIOM_I2C_SEL      (0x0Eu)
#define LCD_SPIM_SCB_HSIOM_SPI_SEL      (0x0Fu)

#define LCD_SPIM_SCB_SCB_PINS_NUMBER            (7u)
#define LCD_SPIM_SCB_MOSI_SCL_RX_PIN_INDEX      (0u) /* RX pins without interrupt */
#define LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN_INDEX (0u) /* RX pin with interrupt     */
#define LCD_SPIM_SCB_MISO_SDA_TX_PIN_INDEX      (1u)
#define LCD_SPIM_SCB_SCLK_PIN_INDEX             (2u)
#define LCD_SPIM_SCB_SS0_PIN_INDEX              (3u)
#define LCD_SPIM_SCB_SS1_PIN_INDEX              (4u)
#define LCD_SPIM_SCB_SS2_PIN_INDEX              (5u)
#define LCD_SPIM_SCB_SS3_PIN_INDEX              (6u)

#define LCD_SPIM_SCB_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin DM defines */
#define LCD_SPIM_SCB_PIN_DM_ALG_HIZ  (0u)
#define LCD_SPIM_SCB_PIN_DM_DIG_HIZ  (1u)
#define LCD_SPIM_SCB_PIN_DM_OD_LO    (4u)
#define LCD_SPIM_SCB_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

#define LCD_SPIM_SCB_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        /* Sets new bits-mask */                 \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

#define LCD_SPIM_SCB_SET_HSIOM_SEL(reg, mask, pos, sel) LCD_SPIM_SCB_SET_REGISTER_BITS(reg, mask, pos, sel)
#define LCD_SPIM_SCB_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        LCD_SPIM_SCB_SET_REGISTER_BITS(reg, mask, pos, intType)


/* LCD_SPIM_SCB_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  LCD_SPIM_SCB_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* Unconfigured SCB: scl singnal */
#if(LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_PIN)
    #define LCD_SPIM_SCB_SET_I2C_SCL_DR(val) \
                            LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_wake_Write(val)

    #define LCD_SPIM_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                    LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_REG,  \
                                                   LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_MASK, \
                                                   LCD_SPIM_SCB_MOSI_SCL_RX_WAKE_HSIOM_POS,  \
                                                   (sel))

#elif(LCD_SPIM_SCB_MOSI_SCL_RX_PIN)
    #define LCD_SPIM_SCB_SET_I2C_SCL_DR(val) \
                            LCD_SPIM_SCB_spi_mosi_i2c_scl_uart_rx_Write(val)
                            

    #define LCD_SPIM_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                            LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_REG,  \
                                                           LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_MASK, \
                                                           LCD_SPIM_SCB_MOSI_SCL_RX_HSIOM_POS,  \
                                                           (sel))
#else
    #if(!LCD_SPIM_SCB_I2C_PINS)
        #define LCD_SPIM_SCB_SET_I2C_SCL_DR(val) \
                                                 do{ ; }while(0)
        #define LCD_SPIM_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                                                        do{ ; }while(0)
        
    #endif /* (!LCD_SPIM_SCB_I2C_PINS) */
#endif /* (LCD_SPIM_SCB_MOSI_SCL_RX_PIN) */

/* SCB I2C: scl singal */
#if(LCD_SPIM_SCB_I2C_PINS)
    #define LCD_SPIM_SCB_SET_I2C_SCL_DR(val) LCD_SPIM_SCB_scl_Write(val)
    
    #define LCD_SPIM_SCB_SET_I2C_SCL_HSIOM_SEL(sel) \
                          LCD_SPIM_SCB_SET_HSIOM_SEL(LCD_SPIM_SCB_SCL_HSIOM_REG,  \
                                                         LCD_SPIM_SCB_SCL_HSIOM_MASK, \
                                                         LCD_SPIM_SCB_SCL_HSIOM_POS,  \
                                                         (sel))
    
#endif /* (LCD_SPIM_SCB_I2C_PINS) */


#endif /* (CY_SCB_PINS_LCD_SPIM_SCB_H) */


/* [] END OF FILE */
