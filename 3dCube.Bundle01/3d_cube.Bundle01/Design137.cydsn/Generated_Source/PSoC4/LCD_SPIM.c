/*******************************************************************************
* File Name: LCD_SPIM.c
* Version 2.40
*
* Description:
*  This file provides all API functionality of the SPI Master component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LCD_SPIM_PVT.h"

#if(LCD_SPIM_TX_SOFTWARE_BUF_ENABLED)
    volatile uint16 LCD_SPIM_txBuffer[LCD_SPIM_TX_BUFFER_SIZE] = {0u};
    volatile uint8 LCD_SPIM_txBufferFull;
    volatile uint8 LCD_SPIM_txBufferRead;
    volatile uint8 LCD_SPIM_txBufferWrite;
#endif /* (LCD_SPIM_TX_SOFTWARE_BUF_ENABLED) */

#if(LCD_SPIM_RX_SOFTWARE_BUF_ENABLED)
    volatile uint16 LCD_SPIM_rxBuffer[LCD_SPIM_RX_BUFFER_SIZE] = {0u};
    volatile uint8 LCD_SPIM_rxBufferFull;
    volatile uint8 LCD_SPIM_rxBufferRead;
    volatile uint8 LCD_SPIM_rxBufferWrite;
#endif /* (LCD_SPIM_RX_SOFTWARE_BUF_ENABLED) */

uint8 LCD_SPIM_initVar = 0u;

volatile uint8 LCD_SPIM_swStatusTx;
volatile uint8 LCD_SPIM_swStatusRx;


/*******************************************************************************
* Function Name: LCD_SPIM_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default SPIM configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  When this function is called it initializes all of the necessary parameters
*  for execution. i.e. setting the initial interrupt mask, configuring the
*  interrupt service routine, configuring the bit-counter parameters and
*  clearing the FIFO and Status Register.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LCD_SPIM_Init(void) 
{
    /* Initialize the Bit counter */
    LCD_SPIM_COUNTER_PERIOD_REG = LCD_SPIM_BITCTR_INIT;

    /* Init TX ISR  */
    #if(0u != LCD_SPIM_INTERNAL_TX_INT_ENABLED)
        CyIntDisable         (LCD_SPIM_TX_ISR_NUMBER);
        CyIntSetPriority     (LCD_SPIM_TX_ISR_NUMBER,  LCD_SPIM_TX_ISR_PRIORITY);
        (void) CyIntSetVector(LCD_SPIM_TX_ISR_NUMBER, &LCD_SPIM_TX_ISR);
    #endif /* (0u != LCD_SPIM_INTERNAL_TX_INT_ENABLED) */

    /* Init RX ISR  */
    #if(0u != LCD_SPIM_INTERNAL_RX_INT_ENABLED)
        CyIntDisable         (LCD_SPIM_RX_ISR_NUMBER);
        CyIntSetPriority     (LCD_SPIM_RX_ISR_NUMBER,  LCD_SPIM_RX_ISR_PRIORITY);
        (void) CyIntSetVector(LCD_SPIM_RX_ISR_NUMBER, &LCD_SPIM_RX_ISR);
    #endif /* (0u != LCD_SPIM_INTERNAL_RX_INT_ENABLED) */

    /* Clear any stray data from the RX and TX FIFO */
    LCD_SPIM_ClearFIFO();

    #if(LCD_SPIM_RX_SOFTWARE_BUF_ENABLED)
        LCD_SPIM_rxBufferFull  = 0u;
        LCD_SPIM_rxBufferRead  = 0u;
        LCD_SPIM_rxBufferWrite = 0u;
    #endif /* (LCD_SPIM_RX_SOFTWARE_BUF_ENABLED) */

    #if(LCD_SPIM_TX_SOFTWARE_BUF_ENABLED)
        LCD_SPIM_txBufferFull  = 0u;
        LCD_SPIM_txBufferRead  = 0u;
        LCD_SPIM_txBufferWrite = 0u;
    #endif /* (LCD_SPIM_TX_SOFTWARE_BUF_ENABLED) */

    (void) LCD_SPIM_ReadTxStatus(); /* Clear Tx status and swStatusTx */
    (void) LCD_SPIM_ReadRxStatus(); /* Clear Rx status and swStatusRx */

    /* Configure TX and RX interrupt mask */
    LCD_SPIM_TX_STATUS_MASK_REG = LCD_SPIM_TX_INIT_INTERRUPTS_MASK;
    LCD_SPIM_RX_STATUS_MASK_REG = LCD_SPIM_RX_INIT_INTERRUPTS_MASK;
}


/*******************************************************************************
* Function Name: LCD_SPIM_Enable
********************************************************************************
*
* Summary:
*  Enable SPIM component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void LCD_SPIM_Enable(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    LCD_SPIM_COUNTER_CONTROL_REG |= LCD_SPIM_CNTR_ENABLE;
    LCD_SPIM_TX_STATUS_ACTL_REG  |= LCD_SPIM_INT_ENABLE;
    LCD_SPIM_RX_STATUS_ACTL_REG  |= LCD_SPIM_INT_ENABLE;
    CyExitCriticalSection(enableInterrupts);

    #if(0u != LCD_SPIM_INTERNAL_CLOCK)
        LCD_SPIM_IntClock_Enable();
    #endif /* (0u != LCD_SPIM_INTERNAL_CLOCK) */

    LCD_SPIM_EnableTxInt();
    LCD_SPIM_EnableRxInt();
}


/*******************************************************************************
* Function Name: LCD_SPIM_Start
********************************************************************************
*
* Summary:
*  Initialize and Enable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  LCD_SPIM_initVar - used to check initial configuration, modified on
*  first function call.
*
* Theory:
*  Enable the clock input to enable operation.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LCD_SPIM_Start(void) 
{
    if(0u == LCD_SPIM_initVar)
    {
        LCD_SPIM_Init();
        LCD_SPIM_initVar = 1u;
    }

    LCD_SPIM_Enable();
}


/*******************************************************************************
* Function Name: LCD_SPIM_Stop
********************************************************************************
*
* Summary:
*  Disable the SPI Master component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the clock input to enable operation.
*
*******************************************************************************/
void LCD_SPIM_Stop(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    LCD_SPIM_TX_STATUS_ACTL_REG &= ((uint8) ~LCD_SPIM_INT_ENABLE);
    LCD_SPIM_RX_STATUS_ACTL_REG &= ((uint8) ~LCD_SPIM_INT_ENABLE);
    CyExitCriticalSection(enableInterrupts);

    #if(0u != LCD_SPIM_INTERNAL_CLOCK)
        LCD_SPIM_IntClock_Disable();
    #endif /* (0u != LCD_SPIM_INTERNAL_CLOCK) */

    LCD_SPIM_DisableTxInt();
    LCD_SPIM_DisableRxInt();
}


/*******************************************************************************
* Function Name: LCD_SPIM_EnableTxInt
********************************************************************************
*
* Summary:
*  Enable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LCD_SPIM_EnableTxInt(void) 
{
    #if(0u != LCD_SPIM_INTERNAL_TX_INT_ENABLED)
        CyIntEnable(LCD_SPIM_TX_ISR_NUMBER);
    #endif /* (0u != LCD_SPIM_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_EnableRxInt
********************************************************************************
*
* Summary:
*  Enable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LCD_SPIM_EnableRxInt(void) 
{
    #if(0u != LCD_SPIM_INTERNAL_RX_INT_ENABLED)
        CyIntEnable(LCD_SPIM_RX_ISR_NUMBER);
    #endif /* (0u != LCD_SPIM_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_DisableTxInt
********************************************************************************
*
* Summary:
*  Disable internal Tx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Tx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LCD_SPIM_DisableTxInt(void) 
{
    #if(0u != LCD_SPIM_INTERNAL_TX_INT_ENABLED)
        CyIntDisable(LCD_SPIM_TX_ISR_NUMBER);
    #endif /* (0u != LCD_SPIM_INTERNAL_TX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_DisableRxInt
********************************************************************************
*
* Summary:
*  Disable internal Rx interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal Rx interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LCD_SPIM_DisableRxInt(void) 
{
    #if(0u != LCD_SPIM_INTERNAL_RX_INT_ENABLED)
        CyIntDisable(LCD_SPIM_RX_ISR_NUMBER);
    #endif /* (0u != LCD_SPIM_INTERNAL_RX_INT_ENABLED) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_SetTxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void LCD_SPIM_SetTxInterruptMode(uint8 intSrc) 
{
    LCD_SPIM_TX_STATUS_MASK_REG = intSrc;
}


/*******************************************************************************
* Function Name: LCD_SPIM_SetRxInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void LCD_SPIM_SetRxInterruptMode(uint8 intSrc) 
{
    LCD_SPIM_RX_STATUS_MASK_REG  = intSrc;
}


/*******************************************************************************
* Function Name: LCD_SPIM_ReadTxStatus
********************************************************************************
*
* Summary:
*  Read the Tx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Tx status register.
*
* Global variables:
*  LCD_SPIM_swStatusTx - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Tx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 LCD_SPIM_ReadTxStatus(void) 
{
    uint8 tmpStatus;

    #if(LCD_SPIM_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        LCD_SPIM_DisableTxInt();

        tmpStatus = LCD_SPIM_GET_STATUS_TX(LCD_SPIM_swStatusTx);
        LCD_SPIM_swStatusTx = 0u;

        LCD_SPIM_EnableTxInt();

    #else

        tmpStatus = LCD_SPIM_TX_STATUS_REG;

    #endif /* (LCD_SPIM_TX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: LCD_SPIM_ReadRxStatus
********************************************************************************
*
* Summary:
*  Read the Rx status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the Rx status register.
*
* Global variables:
*  LCD_SPIM_swStatusRx - used to store in software Rx status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the Rx status register for error
*  detection and flow control.
*
* Side Effects:
*  Clear Rx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 LCD_SPIM_ReadRxStatus(void) 
{
    uint8 tmpStatus;

    #if(LCD_SPIM_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        LCD_SPIM_DisableRxInt();

        tmpStatus = LCD_SPIM_GET_STATUS_RX(LCD_SPIM_swStatusRx);
        LCD_SPIM_swStatusRx = 0u;

        LCD_SPIM_EnableRxInt();

    #else

        tmpStatus = LCD_SPIM_RX_STATUS_REG;

    #endif /* (LCD_SPIM_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/*******************************************************************************
* Function Name: LCD_SPIM_WriteTxData
********************************************************************************
*
* Summary:
*  Write a byte of data to be sent across the SPI.
*
* Parameters:
*  txDataByte: The data value to send across the SPI.
*
* Return:
*  None.
*
* Global variables:
*  LCD_SPIM_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call if TX Software Buffer is used.
*  LCD_SPIM_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*  LCD_SPIM_txBuffer[LCD_SPIM_TX_BUFFER_SIZE] - used to store
*  data to sending, modified every function call if TX Software Buffer is used.
*
* Theory:
*  Allows the user to transmit any byte of data in a single transfer.
*
* Side Effects:
*  If this function is called again before the previous byte is finished then
*  the next byte will be appended to the transfer with no time between
*  the byte transfers. Clear Tx status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LCD_SPIM_WriteTxData(uint16 txData) 
{
    #if(LCD_SPIM_TX_SOFTWARE_BUF_ENABLED)

        uint8 tempStatus;
        uint8 tmpTxBufferRead;

        /* Block if TX buffer is FULL: don't overwrite */
        do
        {
            tmpTxBufferRead = LCD_SPIM_txBufferRead;
            if(0u == tmpTxBufferRead)
            {
                tmpTxBufferRead = (LCD_SPIM_TX_BUFFER_SIZE - 1u);
            }
            else
            {
                tmpTxBufferRead--;
            }

        }while(tmpTxBufferRead == LCD_SPIM_txBufferWrite);

        /* Disable TX interrupt to protect global veriables */
        LCD_SPIM_DisableTxInt();

        tempStatus = LCD_SPIM_GET_STATUS_TX(LCD_SPIM_swStatusTx);
        LCD_SPIM_swStatusTx = tempStatus;


        if((LCD_SPIM_txBufferRead == LCD_SPIM_txBufferWrite) &&
           (0u != (LCD_SPIM_swStatusTx & LCD_SPIM_STS_TX_FIFO_NOT_FULL)))
        {
            /* Add directly to the TX FIFO */
            CY_SET_REG16(LCD_SPIM_TXDATA_PTR, txData);
        }
        else
        {
            /* Add to the TX software buffer */
            LCD_SPIM_txBufferWrite++;
            if(LCD_SPIM_txBufferWrite >= LCD_SPIM_TX_BUFFER_SIZE)
            {
                LCD_SPIM_txBufferWrite = 0u;
            }

            if(LCD_SPIM_txBufferWrite == LCD_SPIM_txBufferRead)
            {
                LCD_SPIM_txBufferRead++;
                if(LCD_SPIM_txBufferRead >= LCD_SPIM_TX_BUFFER_SIZE)
                {
                    LCD_SPIM_txBufferRead = 0u;
                }
                LCD_SPIM_txBufferFull = 1u;
            }

            LCD_SPIM_txBuffer[LCD_SPIM_txBufferWrite] = txData;

            LCD_SPIM_TX_STATUS_MASK_REG |= LCD_SPIM_STS_TX_FIFO_NOT_FULL;
        }

        LCD_SPIM_EnableTxInt();

    #else

        while(0u == (LCD_SPIM_TX_STATUS_REG & LCD_SPIM_STS_TX_FIFO_NOT_FULL))
        {
            ; /* Wait for room in FIFO */
        }

        /* Put byte in TX FIFO */
        CY_SET_REG16(LCD_SPIM_TXDATA_PTR, txData);

    #endif /* (LCD_SPIM_TX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_ReadRxData
********************************************************************************
*
* Summary:
*  Read the next byte of data received across the SPI.
*
* Parameters:
*  None.
*
* Return:
*  The next byte of data read from the FIFO.
*
* Global variables:
*  LCD_SPIM_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  LCD_SPIM_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function
*  call if RX Software Buffer is used.
*  LCD_SPIM_rxBuffer[LCD_SPIM_RX_BUFFER_SIZE] - used to store
*  received data.
*
* Theory:
*  Allows the user to read a byte of data received.
*
* Side Effects:
*  Will return invalid data if the FIFO is empty. The user should Call
*  GetRxBufferSize() and if it returns a non-zero value then it is safe to call
*  ReadByte() function.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint16 LCD_SPIM_ReadRxData(void) 
{
    uint16 rxData;

    #if(LCD_SPIM_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        LCD_SPIM_DisableRxInt();

        if(LCD_SPIM_rxBufferRead != LCD_SPIM_rxBufferWrite)
        {
            if(0u == LCD_SPIM_rxBufferFull)
            {
                LCD_SPIM_rxBufferRead++;
                if(LCD_SPIM_rxBufferRead >= LCD_SPIM_RX_BUFFER_SIZE)
                {
                    LCD_SPIM_rxBufferRead = 0u;
                }
            }
            else
            {
                LCD_SPIM_rxBufferFull = 0u;
            }
        }

        rxData = LCD_SPIM_rxBuffer[LCD_SPIM_rxBufferRead];

        LCD_SPIM_EnableRxInt();

    #else

        rxData = CY_GET_REG16(LCD_SPIM_RXDATA_PTR);

    #endif /* (LCD_SPIM_RX_SOFTWARE_BUF_ENABLED) */

    return(rxData);
}


/*******************************************************************************
* Function Name: LCD_SPIM_GetRxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the RX buffer.
*  If RX Software Buffer not used then function return 0 if FIFO empty or 1 if
*  FIFO not empty. In another case function return size of RX Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the RX buffer.
*
* Global variables:
*  LCD_SPIM_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer.
*  LCD_SPIM_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8 LCD_SPIM_GetRxBufferSize(void) 
{
    uint8 size;

    #if(LCD_SPIM_RX_SOFTWARE_BUF_ENABLED)

        /* Disable RX interrupt to protect global veriables */
        LCD_SPIM_DisableRxInt();

        if(LCD_SPIM_rxBufferRead == LCD_SPIM_rxBufferWrite)
        {
            size = 0u;
        }
        else if(LCD_SPIM_rxBufferRead < LCD_SPIM_rxBufferWrite)
        {
            size = (LCD_SPIM_rxBufferWrite - LCD_SPIM_rxBufferRead);
        }
        else
        {
            size = (LCD_SPIM_RX_BUFFER_SIZE - LCD_SPIM_rxBufferRead) + LCD_SPIM_rxBufferWrite;
        }

        LCD_SPIM_EnableRxInt();

    #else

        /* We can only know if there is data in the RX FIFO */
        size = (0u != (LCD_SPIM_RX_STATUS_REG & LCD_SPIM_STS_RX_FIFO_NOT_EMPTY)) ? 1u : 0u;

    #endif /* (LCD_SPIM_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: LCD_SPIM_GetTxBufferSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes/words of data currently held in the TX buffer.
*  If TX Software Buffer not used then function return 0 - if FIFO empty, 1 - if
*  FIFO not full, 4 - if FIFO full. In another case function return size of TX
*  Software Buffer.
*
* Parameters:
*  None.
*
* Return:
*  Integer count of the number of bytes/words in the TX buffer.
*
* Global variables:
*  LCD_SPIM_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer.
*  LCD_SPIM_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
uint8  LCD_SPIM_GetTxBufferSize(void) 
{
    uint8 size;

    #if(LCD_SPIM_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        LCD_SPIM_DisableTxInt();

        if(LCD_SPIM_txBufferRead == LCD_SPIM_txBufferWrite)
        {
            size = 0u;
        }
        else if(LCD_SPIM_txBufferRead < LCD_SPIM_txBufferWrite)
        {
            size = (LCD_SPIM_txBufferWrite - LCD_SPIM_txBufferRead);
        }
        else
        {
            size = (LCD_SPIM_TX_BUFFER_SIZE - LCD_SPIM_txBufferRead) + LCD_SPIM_txBufferWrite;
        }

        LCD_SPIM_EnableTxInt();

    #else

        size = LCD_SPIM_TX_STATUS_REG;

        if(0u != (size & LCD_SPIM_STS_TX_FIFO_EMPTY))
        {
            size = 0u;
        }
        else if(0u != (size & LCD_SPIM_STS_TX_FIFO_NOT_FULL))
        {
            size = 1u;
        }
        else
        {
            size = LCD_SPIM_FIFO_SIZE;
        }

    #endif /* (LCD_SPIM_TX_SOFTWARE_BUF_ENABLED) */

    return(size);
}


/*******************************************************************************
* Function Name: LCD_SPIM_ClearRxBuffer
********************************************************************************
*
* Summary:
*  Clear the RX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  LCD_SPIM_rxBufferWrite - used for the account of the bytes which
*  have been written down in the RX software buffer, modified every function
*  call - resets to zero.
*  LCD_SPIM_rxBufferRead - used for the account of the bytes which
*  have been read from the RX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any received data not read from the RAM buffer will be lost when overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LCD_SPIM_ClearRxBuffer(void) 
{
    /* Clear Hardware RX FIFO */
    while(0u !=(LCD_SPIM_RX_STATUS_REG & LCD_SPIM_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG16(LCD_SPIM_RXDATA_PTR);
    }

    #if(LCD_SPIM_RX_SOFTWARE_BUF_ENABLED)
        /* Disable RX interrupt to protect global veriables */
        LCD_SPIM_DisableRxInt();

        LCD_SPIM_rxBufferFull  = 0u;
        LCD_SPIM_rxBufferRead  = 0u;
        LCD_SPIM_rxBufferWrite = 0u;

        LCD_SPIM_EnableRxInt();
    #endif /* (LCD_SPIM_RX_SOFTWARE_BUF_ENABLED) */
}


/*******************************************************************************
* Function Name: LCD_SPIM_ClearTxBuffer
********************************************************************************
*
* Summary:
*  Clear the TX RAM buffer by setting the read and write pointers both to zero.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  LCD_SPIM_txBufferWrite - used for the account of the bytes which
*  have been written down in the TX software buffer, modified every function
*  call - resets to zero.
*  LCD_SPIM_txBufferRead - used for the account of the bytes which
*  have been read from the TX software buffer, modified every function call -
*  resets to zero.
*
* Theory:
*  Setting the pointers to zero makes the system believe there is no data to
*  read and writing will resume at address 0 overwriting any data that may have
*  remained in the RAM.
*
* Side Effects:
*  Any data not yet transmitted from the RAM buffer will be lost when
*  overwritten.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LCD_SPIM_ClearTxBuffer(void) 
{
    uint8 enableInterrupts;

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    LCD_SPIM_AUX_CONTROL_DP0_REG |= ((uint8)  LCD_SPIM_TX_FIFO_CLR);
    LCD_SPIM_AUX_CONTROL_DP0_REG &= ((uint8) ~LCD_SPIM_TX_FIFO_CLR);

    #if(LCD_SPIM_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        LCD_SPIM_AUX_CONTROL_DP1_REG |= ((uint8)  LCD_SPIM_TX_FIFO_CLR);
        LCD_SPIM_AUX_CONTROL_DP1_REG &= ((uint8) ~LCD_SPIM_TX_FIFO_CLR);
    #endif /* (LCD_SPIM_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);

    #if(LCD_SPIM_TX_SOFTWARE_BUF_ENABLED)
        /* Disable TX interrupt to protect global veriables */
        LCD_SPIM_DisableTxInt();

        LCD_SPIM_txBufferFull  = 0u;
        LCD_SPIM_txBufferRead  = 0u;
        LCD_SPIM_txBufferWrite = 0u;

        /* Buffer is EMPTY: disable TX FIFO NOT FULL interrupt */
        LCD_SPIM_TX_STATUS_MASK_REG &= ((uint8) ~LCD_SPIM_STS_TX_FIFO_NOT_FULL);

        LCD_SPIM_EnableTxInt();
    #endif /* (LCD_SPIM_TX_SOFTWARE_BUF_ENABLED) */
}


#if(0u != LCD_SPIM_BIDIRECTIONAL_MODE)
    /*******************************************************************************
    * Function Name: LCD_SPIM_TxEnable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to transmit.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void LCD_SPIM_TxEnable(void) 
    {
        LCD_SPIM_CONTROL_REG |= LCD_SPIM_CTRL_TX_SIGNAL_EN;
    }


    /*******************************************************************************
    * Function Name: LCD_SPIM_TxDisable
    ********************************************************************************
    *
    * Summary:
    *  If the SPI master is configured to use a single bi-directional pin then this
    *  will set the bi-directional pin to receive.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void LCD_SPIM_TxDisable(void) 
    {
        LCD_SPIM_CONTROL_REG &= ((uint8) ~LCD_SPIM_CTRL_TX_SIGNAL_EN);
    }

#endif /* (0u != LCD_SPIM_BIDIRECTIONAL_MODE) */


/*******************************************************************************
* Function Name: LCD_SPIM_PutArray
********************************************************************************
*
* Summary:
*  Write available data from ROM/RAM to the TX buffer while space is available
*  in the TX buffer. Keep trying until all data is passed to the TX buffer.
*
* Parameters:
*  *buffer: Pointer to the location in RAM containing the data to send
*  byteCount: The number of bytes to move to the transmit buffer.
*
* Return:
*  None.
*
* Side Effects:
*  Will stay in this routine until all data has been sent.  May get locked in
*  this loop if data is not being initiated by the master if there is not
*  enough room in the TX FIFO.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LCD_SPIM_PutArray(const uint16 buffer[], uint8 byteCount)
                                                                          
{
    uint8 bufIndex;

    bufIndex = 0u;

    while(byteCount > 0u)
    {
        LCD_SPIM_WriteTxData(buffer[bufIndex]);
        bufIndex++;
        byteCount--;
    }
}


/*******************************************************************************
* Function Name: LCD_SPIM_ClearFIFO
********************************************************************************
*
* Summary:
*  Clear the RX and TX FIFO's of all data for a fresh start.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  Clear status register of the component.
*
*******************************************************************************/
void LCD_SPIM_ClearFIFO(void) 
{
    uint8 enableInterrupts;

    /* Clear Hardware RX FIFO */
    while(0u !=(LCD_SPIM_RX_STATUS_REG & LCD_SPIM_STS_RX_FIFO_NOT_EMPTY))
    {
        (void) CY_GET_REG16(LCD_SPIM_RXDATA_PTR);
    }

    enableInterrupts = CyEnterCriticalSection();
    /* Clear TX FIFO */
    LCD_SPIM_AUX_CONTROL_DP0_REG |= ((uint8)  LCD_SPIM_TX_FIFO_CLR);
    LCD_SPIM_AUX_CONTROL_DP0_REG &= ((uint8) ~LCD_SPIM_TX_FIFO_CLR);

    #if(LCD_SPIM_USE_SECOND_DATAPATH)
        /* Clear TX FIFO for 2nd Datapath */
        LCD_SPIM_AUX_CONTROL_DP1_REG |= ((uint8)  LCD_SPIM_TX_FIFO_CLR);
        LCD_SPIM_AUX_CONTROL_DP1_REG &= ((uint8) ~LCD_SPIM_TX_FIFO_CLR);
    #endif /* (LCD_SPIM_USE_SECOND_DATAPATH) */
    CyExitCriticalSection(enableInterrupts);
}


/* Following functions are for version Compatibility, they are obsolete.
*  Please do not use it in new projects.
*/


/*******************************************************************************
* Function Name: LCD_SPIM_EnableInt
********************************************************************************
*
* Summary:
*  Enable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Enable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LCD_SPIM_EnableInt(void) 
{
    LCD_SPIM_EnableRxInt();
    LCD_SPIM_EnableTxInt();
}


/*******************************************************************************
* Function Name: LCD_SPIM_DisableInt
********************************************************************************
*
* Summary:
*  Disable internal interrupt generation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Theory:
*  Disable the internal interrupt output -or- the interrupt component itself.
*
*******************************************************************************/
void LCD_SPIM_DisableInt(void) 
{
    LCD_SPIM_DisableTxInt();
    LCD_SPIM_DisableRxInt();
}


/*******************************************************************************
* Function Name: LCD_SPIM_SetInterruptMode
********************************************************************************
*
* Summary:
*  Configure which status bits trigger an interrupt event.
*
* Parameters:
*  intSrc: An or'd combination of the desired status bit masks (defined in the
*  header file).
*
* Return:
*  None.
*
* Theory:
*  Enables the output of specific status bits to the interrupt controller.
*
*******************************************************************************/
void LCD_SPIM_SetInterruptMode(uint8 intSrc) 
{
    LCD_SPIM_TX_STATUS_MASK_REG  = (intSrc & ((uint8) ~LCD_SPIM_STS_SPI_IDLE));
    LCD_SPIM_RX_STATUS_MASK_REG  =  intSrc;
}


/*******************************************************************************
* Function Name: LCD_SPIM_ReadStatus
********************************************************************************
*
* Summary:
*  Read the status register for the component.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the status register.
*
* Global variables:
*  LCD_SPIM_swStatus - used to store in software status register,
*  modified every function call - resets to zero.
*
* Theory:
*  Allows the user and the API to read the status register for error detection
*  and flow control.
*
* Side Effects:
*  Clear status register of the component.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 LCD_SPIM_ReadStatus(void) 
{
    uint8 tmpStatus;

    #if(LCD_SPIM_TX_SOFTWARE_BUF_ENABLED || LCD_SPIM_RX_SOFTWARE_BUF_ENABLED)

        LCD_SPIM_DisableInt();

        tmpStatus  = LCD_SPIM_GET_STATUS_RX(LCD_SPIM_swStatusRx);
        tmpStatus |= LCD_SPIM_GET_STATUS_TX(LCD_SPIM_swStatusTx);
        tmpStatus &= ((uint8) ~LCD_SPIM_STS_SPI_IDLE);

        LCD_SPIM_swStatusTx = 0u;
        LCD_SPIM_swStatusRx = 0u;

        LCD_SPIM_EnableInt();

    #else

        tmpStatus  = LCD_SPIM_RX_STATUS_REG;
        tmpStatus |= LCD_SPIM_TX_STATUS_REG;
        tmpStatus &= ((uint8) ~LCD_SPIM_STS_SPI_IDLE);

    #endif /* (LCD_SPIM_TX_SOFTWARE_BUF_ENABLED || LCD_SPIM_RX_SOFTWARE_BUF_ENABLED) */

    return(tmpStatus);
}


/* [] END OF FILE */
