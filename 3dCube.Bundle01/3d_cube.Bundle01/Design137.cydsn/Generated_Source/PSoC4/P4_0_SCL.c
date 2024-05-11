/*******************************************************************************
* File Name: P4_0_SCL.c  
* Version 1.90
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "P4_0_SCL.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        P4_0_SCL_PC =   (P4_0_SCL_PC & \
                                (uint32)(~(uint32)(P4_0_SCL_DRIVE_MODE_IND_MASK << (P4_0_SCL_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (P4_0_SCL_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: P4_0_SCL_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void P4_0_SCL_Write(uint8 value) 
{
    uint8 drVal = (uint8)(P4_0_SCL_DR & (uint8)(~P4_0_SCL_MASK));
    drVal = (drVal | ((uint8)(value << P4_0_SCL_SHIFT) & P4_0_SCL_MASK));
    P4_0_SCL_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: P4_0_SCL_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to this drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void P4_0_SCL_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(P4_0_SCL__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: P4_0_SCL_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro P4_0_SCL_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 P4_0_SCL_Read(void) 
{
    return (uint8)((P4_0_SCL_PS & P4_0_SCL_MASK) >> P4_0_SCL_SHIFT);
}


/*******************************************************************************
* Function Name: P4_0_SCL_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 P4_0_SCL_ReadDataReg(void) 
{
    return (uint8)((P4_0_SCL_DR & P4_0_SCL_MASK) >> P4_0_SCL_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(P4_0_SCL_INTSTAT) 

    /*******************************************************************************
    * Function Name: P4_0_SCL_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 P4_0_SCL_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(P4_0_SCL_INTSTAT & P4_0_SCL_MASK);
		P4_0_SCL_INTSTAT = maskedStatus;
        return maskedStatus >> P4_0_SCL_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
