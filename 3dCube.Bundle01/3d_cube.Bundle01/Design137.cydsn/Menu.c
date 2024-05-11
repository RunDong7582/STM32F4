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

#include "Menu.h"

int32 menu_level = 0;
int32 menu_option = 0;

float foo;

extern float x_rot, y_rot, z_rot;

char * CYCODE menu[] = {"", "X rotation", "Y rotation", "Z rotation", "X perspective", "Y perspective", "Z perspective", "X scale", "Y scale"};
float * param[] = {&foo, &x_rot, &y_rot, &z_rot, &p, &q, &r, &X_SCALE, &Y_SCALE};
float param_step[] = {0, 0.005, 0.005, 0.005, 0.02, 0.02, 0.02, 3, 3};
#define MENU_SIZE 9

void IncreaseMenuLevel(int32 val)
{
	static int32 val_old = 0;
	
	if(val_old == 0 && val > 0)
	{
		ShowMenuLevel(1);  // in Menu.c
		menu_level++;
		menu_level %= MENU_SIZE;
	}
	
	ShowMenuLevel(0);  // in Menu.c
	
	val_old = val;
	
	return;
}

void DecreaseMenuLevel(int32 val)
{
	static int32 val_old = 0;
	
	if(val_old == 0 && val > 0)
	{
		ShowMenuLevel(1);  // in Menu.c
		menu_level--;
		menu_level %= MENU_SIZE;
	}
	
	ShowMenuLevel(0);  // in Menu.c
	
	val_old = val;
	
	return;
}

void ShowMenuLevel(int32 del)
{
	if(del > 0)
	{
		LCD_PrintString(menu[menu_level], 115, 0, BACKGROUND, BACKGROUND);  // in LCD.c
	}
	else
	{
		LCD_PrintString(menu[menu_level], 115, 0, FOREGROUND, BACKGROUND);  // in LCD.c
	}
	
	return;
}

void IncreaseMenuOption(int32 val)
{
	static int32 val_old = 0;
	
	if(val_old == 0 && val > 0)
	{
		(*param[menu_level]) += param_step[menu_level];
	}
	
	val_old = val;
	
	return;
}

void DecreaseMenuOption(int32 val)
{
	static int32 val_old = 0;
	
	if(val_old == 0 && val > 0)
	{
		(*param[menu_level]) -= param_step[menu_level];
	}
	
	val_old = val;
	
	return;
}

void ResetMenuOption(int32 val)
{
	static int32 val_old = 0;
	
	if(val_old == 0 && val > 0)
	{
		(*param[menu_level]) = 0;
	}
	
	val_old = val;
	
	return;
}

/* [] END OF FILE */
