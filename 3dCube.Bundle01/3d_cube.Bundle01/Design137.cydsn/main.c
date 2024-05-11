/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <device.h>
#include "Defines.h"
#include "Cube.h"
#include "Menu.h"

void CheckButtons(void);

float x_rot, y_rot, z_rot;

void main()
{
	
	X_OFFSET = 65.0;
	Y_OFFSET = 65.0;
	X_SCALE = 30.0;
	Y_SCALE = 30.0;
	
	x_rot = 0.02;
	y_rot = 0.035;
	z_rot = 0.055;
	
	InitializeCube(); // in Cube.c
	
	LCD_Start(); // in LCD.c
	LCD_On(); // in LCD.c
	LCD_Contrast(60); // in LCD.c
	LCD_Clear(BACKGROUND); // in LCD.c
	
	ShowMenuLevel(0);

    for(;;)
    {
		// rotate the cube points around the defined axis
		RotateCubeX(x_rot); // in Cube.c
		RotateCubeY(y_rot); // in Cube.c
		RotateCubeZ(z_rot); // in Cube.c
		
		// save the current points
		SaveOldLinesCube(); // in Cube.c
		
		// perform a 2D project of the 3D object onto the X,Y plane
		// with Z perspective
		ProjectCube(); // in Cube.c
		
		// Draw the cube on the screen
		DrawCube(); // in Cube.c
		
		// check the buttons for adjusting different parameters
		CheckButtons(); // in main.c
    }
}

void CheckButtons(void)
{
	// if D5 button is pressed
	if(D5_Read() == 0)
	{
		IncreaseMenuOption(1);  // in Menu.c
	}
	else
	{
		IncreaseMenuOption(0);  // in Menu.c
	}
	
	// if D4 button is pressed
	if(D4_Read() == 0)
	{
		DecreaseMenuOption(1);  // in Menu.c
	}
	else
	{
		DecreaseMenuOption(0);  // in Menu.c
	}
	
	// if D3 button is pressed
	if(D3_Read() == 0)
	{
		ResetMenuOption(1);  // in Menu.c
	}
	else
	{
		ResetMenuOption(0);  // in Menu.c
	}
	
	// if the Pioneer kit button is pressed
	if(P0_7_SWITCH_Read() == 0)
	{
		IncreaseMenuLevel(1);  // in Menu.c
	}
	else
	{
		IncreaseMenuLevel(0);  // in Menu.c
	}
	
	return;
}

/* [] END OF FILE */
