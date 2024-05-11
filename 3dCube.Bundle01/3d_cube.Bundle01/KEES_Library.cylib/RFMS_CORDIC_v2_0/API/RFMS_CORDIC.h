/*******************************************************************************
* File Name: `$INSTANCE_NAME`.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides constants and parameter values and API definition for the
*  RFMS_CORDIC Component
*
* Note:
*
********************************************************************************
* Copyright 2011-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(`$INSTANCE_NAME`_H) /* CORDIC_v1 Header File */
#define `$INSTANCE_NAME`_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#ifndef CY_PSOC5A
    #error Component `$CY_COMPONENT_NAME` requires cy_boot v3.0 or later
#endif /* CY_PSOC5A */


/***************************************
*        Function Prototypes
***************************************/

void  `$INSTANCE_NAME`_Start(uint16 angle, uint16 x, uint16 y)  `=ReentrantKeil($INSTANCE_NAME . "_Start")`;
void  `$INSTANCE_NAME`_Init(void)                    `=ReentrantKeil($INSTANCE_NAME . "_Init")`;
void  `$INSTANCE_NAME`_Enable(void)                  `=ReentrantKeil($INSTANCE_NAME . "_Enable")`;
void  `$INSTANCE_NAME`_Stop(void)                    `=ReentrantKeil($INSTANCE_NAME . "_Stop")`;
void `$INSTANCE_NAME`_GetData(int16 *result_0, int16 *result_1) `=ReentrantKeil($INSTANCE_NAME . "_GetData")`;

/***************************************
*   Variable with external linkage
***************************************/

extern uint8 `$INSTANCE_NAME`_initVar;
						  
/***************************************
*           API Constants
***************************************/
#if `$CORDIC_MODE`
	#define `$INSTANCE_NAME`_COUNTER_LIM        (0x000Fu)
#else
	#define `$INSTANCE_NAME`_COUNTER_LIM        (0x000Eu)
#endif

#define `$INSTANCE_NAME`_SEED_VALUE         (0x009Bu)
#define `$INSTANCE_NAME`_INIT_ANGLE         (0x2D00u)
#define `$INSTANCE_NAME`_SIGN               (0x8000u)

/***************************************
*    Enumerated Types and Parameters
***************************************/

/***************************************
*    Initial Parameter Constants
***************************************/

/***************************************
*             Registers
***************************************/

/* Control register */
#define `$INSTANCE_NAME`_CONTROL_REG (* (reg8 *) `$INSTANCE_NAME`_bCORDIC_CtlReg__CONTROL_REG)
#define `$INSTANCE_NAME`_CONTROL_PTR (  (reg8 *) `$INSTANCE_NAME`_bCORDIC_CtlReg__CONTROL_REG)

/* Status register */
#define `$INSTANCE_NAME`_STATUS_REG       (* (reg8 *) `$INSTANCE_NAME`_bCORDIC_StsReg__STATUS_REG)
#define `$INSTANCE_NAME`_STATUS_MASK_REG  (* (reg8 *) `$INSTANCE_NAME`_bCORDIC_StsReg__MASK_REG)

/* Datapath auxillary control register */
#define `$INSTANCE_NAME`_CORE_HI_AUX_CTL (* (reg8 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__DP_AUX_CTL_REG)
#define `$INSTANCE_NAME`_CORE_LO_AUX_CTL (* (reg8 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u1__DP_AUX_CTL_REG)

#if(CY_PSOC3)/* 8-bit UDB register access */

	/* Counter limit register */
	#define `$INSTANCE_NAME`_COUNTER_LIM_PTR         (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CountDP_u0__D0_REG)

	/* Desired result magnitude register */
	#define `$INSTANCE_NAME`_SEED_VALUE_PTR          (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__A0_REG)
	
    /* Initial value of the Angle calculation */
    #define `$INSTANCE_NAME`_INIT_ANGLE_PTR          (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_AngleDP_u0__A0_REG)
	
	/* Current Angle register */
    #define `$INSTANCE_NAME`_CURRENT_ANGLE_PTR       (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_AngleDP_u0__A1_REG)
	
	/* Register to hold the desired target Angle */
    #define `$INSTANCE_NAME`_FINAL_ANGLE_PTR         (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_AngleDP_u0__D1_REG)
	
	/* FIFO to feed in the Angles in the LUT (16-bits because it is used by the DMA) */
    #define `$INSTANCE_NAME`_FIFO0_PTR               (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_AngleDP_u0__16BIT_F0_REG)
	
	/* Cosine result register */
	#define `$INSTANCE_NAME`_COSINE_PTR                (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__A0_REG)
	
	/* Sine result register */
	#define `$INSTANCE_NAME`_SINE_PTR              (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__A1_REG)
	
	/* FIFO 0 of CoreDP */
    #define `$INSTANCE_NAME`_COSINE_FIFO_PTR           (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__F0_REG)
	
	/* FIFO 1 of CoreDP */
    #define `$INSTANCE_NAME`_SINE_FIFO_PTR         (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__F1_REG)
	
	/* Positive/Negative threshold register */
	#define `$INSTANCE_NAME`_SIGN_PTR         (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__D1_REG)

#else /* 16-bit UDB register access */

	/* Counter limit register */
	#define `$INSTANCE_NAME`_COUNTER_LIM_PTR         (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CountDP_u0__16BIT_D0_REG)

	/* Desired result magnitude register */
	#define `$INSTANCE_NAME`_SEED_VALUE_PTR          (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__16BIT_A0_REG)
	
    /* Initial value of the Angle calculation */
    #define `$INSTANCE_NAME`_INIT_ANGLE_PTR          (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_AngleDP_u0__16BIT_A0_REG)

	/* Current Angle register */
    #define `$INSTANCE_NAME`_CURRENT_ANGLE_PTR       (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_AngleDP_u0__16BIT_A1_REG)
	
	/* Register to hold the desired target Angle */
    #define `$INSTANCE_NAME`_FINAL_ANGLE_PTR         (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_AngleDP_u0__16BIT_D1_REG)

	/* FIFO to feed in the Angles in the LUT */
    #define `$INSTANCE_NAME`_FIFO0_PTR               (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_AngleDP_u0__16BIT_F0_REG)

	/* Cosine result register */
	#define `$INSTANCE_NAME`_COSINE_PTR                (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__16BIT_A0_REG)
	
	/* Sine result register */
	#define `$INSTANCE_NAME`_SINE_PTR              (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__16BIT_A1_REG)
	
	/* FIFO 0 of CoreDP */
    #define `$INSTANCE_NAME`_SINE_FIFO_PTR           (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__16BIT_F0_REG)
	
	/* FIFO 1 of CoreDP */
    #define `$INSTANCE_NAME`_COSINE_FIFO_PTR         (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__16BIT_F1_REG)
	
	/* Positive/Negative threshold register */
	#define `$INSTANCE_NAME`_SIGN_PTR         (  (reg16 *) `$INSTANCE_NAME`_bCORDIC_CoreDP_u0__16BIT_D1_REG)

#endif /* End (CY_PSOC3) */

/***************************************
*       Register Constants
***************************************/

/* Enabling and interrupt generation */
#define `$INSTANCE_NAME`_EN             (0x01u)
#define `$INSTANCE_NAME`_RST            (0x02u)

#endif /* End of CORDIC_v1 Header File */


/* [] END OF FILE */
