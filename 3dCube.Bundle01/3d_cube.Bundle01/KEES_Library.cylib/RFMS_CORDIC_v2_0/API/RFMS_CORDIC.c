/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file contains the setup, control and status commands for the RFMS_CORDIC
*  component.
*
* Note:
*
*******************************************************************************
* Copyright 2011-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`.h"
#include "`$INSTANCE_NAME`_DMA_Angle_dma.h"

/*******************************************************************************
* CORDIC_v1 component internal variable.
*******************************************************************************/

static void `$INSTANCE_NAME`_Reinitialize(void) `=ReentrantKeil($INSTANCE_NAME . "_Reinitialize")`;
static void `$INSTANCE_NAME`_DMA_Init(void)     `=ReentrantKeil($INSTANCE_NAME . "_DMA_Init")`;
static uint8 `$INSTANCE_NAME`_ReadStatus(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadStatus")`;

uint8 `$INSTANCE_NAME`_initVar = 0u;

static uint8 dma_angle_td;
static uint8 dma_angle_channel;

/* Angles held in the LUT */
static const uint16 `$INSTANCE_NAME`_AngleLUT[] = {0x1A91,      /* 26.565 degrees */
					                        0x0E09,      /* 14.036 degrees */
											0x0720,      /* 7.125 degrees  */
											0x0394,      /* 3.576 degrees  */
											0x01CA,      /* 1.790 degrees  */
											0x00E5,      /* 0.895 degrees  */
											0x0073,      /* 0.448 degrees  */
											0x0039,      /* 0.224 degrees  */
											0x001C,      /* 0.112 degrees  */
											0x000E,      /* 0.056 degrees  */
											0x0007,      /* 0.028 degrees  */
											0x0004,      /* 0.014 degrees  */
											0x0002,      /* 0.007 degrees  */
											0x0001,      /* 0.003 degrees  */
											0x0000,      /* dummy data     */
											0x0000};     /* dummy data     */
											
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Enable
********************************************************************************
*
* Summary:
*  Enables the CORDIC hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Enable(void) `=ReentrantKeil($INSTANCE_NAME . "_Enable")`
{
    `$INSTANCE_NAME`_CONTROL_REG |= `$INSTANCE_NAME`_EN;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Init
********************************************************************************
*
* Summary:
*  Initializes the component and sets up the DMA.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Init(void) `=ReentrantKeil($INSTANCE_NAME . "_Init")`
{
    /* Set the Counter limit. This determines how many Angles from the LUT will be used */
    CY_SET_REG16(`$INSTANCE_NAME`_COUNTER_LIM_PTR, (uint16) `$INSTANCE_NAME`_COUNTER_LIM);
	
#if `$CORDIC_MODE`
#else
	 /* Set the positive/negative threshold for detecting the sign change */
    CY_SET_REG16(`$INSTANCE_NAME`_SIGN_PTR, (uint16) `$INSTANCE_NAME`_SIGN);
#endif

	`$INSTANCE_NAME`_DMA_Init(); /* Initilize the DMA settings */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
*
* Summary:
*  Enables the component. Calls the Init() API if the component has not been
*  initialized before. Calls the Enable() API.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  `$INSTANCE_NAME`_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Start(uint16 angle, uint16 x, uint16 y) `=ReentrantKeil($INSTANCE_NAME . "_Start")`
{
	uint8 rst_status;
	
	rst_status = 0;
	
    if(0u == `$INSTANCE_NAME`_initVar)
    {
        `$INSTANCE_NAME`_Init();
        `$INSTANCE_NAME`_initVar = 1u;
    }
	`$INSTANCE_NAME`_CONTROL_REG &= ((uint8) ~`$INSTANCE_NAME`_EN);
	`$INSTANCE_NAME`_CONTROL_REG |= `$INSTANCE_NAME`_RST;
	
	/* wait for the signal to arrive */
	while(!rst_status)
	{
		rst_status = `$INSTANCE_NAME`_ReadStatus();
	}
	
	`$INSTANCE_NAME`_CONTROL_REG &= ((uint8) ~`$INSTANCE_NAME`_RST);
	
	/* wait for the signal to arrive */
	while(rst_status)
	{
		rst_status = `$INSTANCE_NAME`_ReadStatus();
	}
	
	`$INSTANCE_NAME`_Reinitialize();
	
#if `$CORDIC_MODE`
    /* Send the desired target angle */
    CY_SET_REG16(`$INSTANCE_NAME`_FINAL_ANGLE_PTR, (uint16)angle);
	rst_status = x+y; /* To get rid of the warning */
#else	
	/* Send the x value */
    CY_SET_REG16(`$INSTANCE_NAME`_COSINE_FIFO_PTR, (uint16)x);
    CY_SET_REG16(`$INSTANCE_NAME`_SINE_FIFO_PTR, (uint16)x);
	/* Send the y value twice */
	CY_SET_REG16(`$INSTANCE_NAME`_COSINE_FIFO_PTR, (uint16)y);
    CY_SET_REG16(`$INSTANCE_NAME`_SINE_FIFO_PTR, (uint16)y);
	CY_SET_REG16(`$INSTANCE_NAME`_COSINE_FIFO_PTR, (uint16)y);
    CY_SET_REG16(`$INSTANCE_NAME`_SINE_FIFO_PTR, (uint16)y);
	rst_status = angle; /* To get rid of the warning */
#endif

    `$INSTANCE_NAME`_Enable();
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Stop
********************************************************************************
*
* Summary:
*  Disables the CORDIC computation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Stop(void) `=ReentrantKeil($INSTANCE_NAME . "_Stop")`
{
	`$INSTANCE_NAME`_CONTROL_REG &= ((uint8) ~`$INSTANCE_NAME`_EN);
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Reinitialize
********************************************************************************
* Summary:
*  Reinitializes the registers for the next CORDIC computation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
********************************************************************************/
void `$INSTANCE_NAME`_Reinitialize(void) `=ReentrantKeil($INSTANCE_NAME . "_Reinitialize")`
{
    /* Reinitialize the starting value of CORDIC */
    CY_SET_REG16(`$INSTANCE_NAME`_SEED_VALUE_PTR, (uint16) `$INSTANCE_NAME`_SEED_VALUE);
	
	/* Reinitialize the initial Angle value */
    CY_SET_REG16(`$INSTANCE_NAME`_INIT_ANGLE_PTR, (uint16) `$INSTANCE_NAME`_INIT_ANGLE);
}

/********************************************************************************
* Function Name: `$INSTANCE_NAME`_DMA_Init
*******************************************************************************
* Summary:
*  This function performs the DNA_Angle initialization.
*
* Parameters:
*	void
*
* Return:
*	void
*
*******************************************************************************/
void `$INSTANCE_NAME`_DMA_Init(void) `=ReentrantKeil($INSTANCE_NAME . "_DMA_Init")`
{		
    dma_angle_td = CyDmaTdAllocate();
    #if (CY_PSOC3)
    dma_angle_channel = `$INSTANCE_NAME`_DMA_Angle_DmaInitialize(2u, 1u, 
	                                    HI16(CYDEV_SRAM_BASE), HI16(CYDEV_PERIPH_BASE));
    CyDmaTdSetConfiguration(dma_angle_td, sizeof(`$INSTANCE_NAME`_AngleLUT), dma_angle_td, 
	                         (`$INSTANCE_NAME`_DMA_Angle__TD_TERMOUT_EN |TD_SWAP_EN | TD_INC_SRC_ADR));
    #elif (CY_PSOC5LP)
    dma_angle_channel = `$INSTANCE_NAME`_DMA_Angle_DmaInitialize(2u, 1u,
	                                    HI16((uint32)&`$INSTANCE_NAME`_AngleLUT), HI16((uint32)`$INSTANCE_NAME`_FIFO0_PTR));
    CyDmaTdSetConfiguration(dma_angle_td, sizeof(`$INSTANCE_NAME`_AngleLUT), dma_angle_td, 
	                         (`$INSTANCE_NAME`_DMA_Angle__TD_TERMOUT_EN | TD_INC_SRC_ADR));
    #endif
	CyDmaTdSetAddress(dma_angle_td,LO16((uint32)`$INSTANCE_NAME`_AngleLUT), LO16((uint32)`$INSTANCE_NAME`_FIFO0_PTR));
	CyDmaChSetInitialTd(dma_angle_channel, dma_angle_td);
	CyDmaChEnable(dma_angle_channel, 1u);
}

/********************************************************************************
* Function Name: `$INSTANCE_NAME`_GetData
*******************************************************************************
* Summary:
*  Retrieves the sine and cosine results from the CoreDP A0 and A1 registers 
*
* Parameters:
*	int16 *sin_data, int16 *cos_data
*
* Return:
*	void
*
*******************************************************************************/
void `$INSTANCE_NAME`_GetData(int16 *result_0, int16 *result_1) `=ReentrantKeil($INSTANCE_NAME . "_GetData")`
{
#if `$CORDIC_MODE`
    /* Get the sine result */
    *result_0 = CY_GET_REG16(`$INSTANCE_NAME`_SINE_PTR);
	
	/* Get the cosine result */
    *result_1 = CY_GET_REG16(`$INSTANCE_NAME`_COSINE_PTR);
#else
    /* Get the arctan2 result */
    *result_0 = CY_GET_REG16(`$INSTANCE_NAME`_CURRENT_ANGLE_PTR);
	
	/* Get the hypotenuse result */
    *result_1 = CY_GET_REG16(`$INSTANCE_NAME`_COSINE_PTR);
#endif
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReadStatus
********************************************************************************
*
* Summary:
*  Returns state in the status register.
*
* Parameters:
*  None
*
* Return:
*  State of the reset signal asserted by the CPU. This is connected to 
*  StatusReg[0].
*
* Side Effects: 
*  None
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_ReadStatus(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadStatus")`
{
    return (`$INSTANCE_NAME`_STATUS_REG);
}

/* [] END OF FILE */
