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
#ifndef MENU_HEADER
#define MENU_HEADER

#include <device.h>
#include "Defines.h"
#include "Transforms.h"

#define MENU_X 115
#define MENU_Y 1
#define LEVEL_HEIGHT 12

void IncreaseMenuLevel(int32 val);
void DecreaseMenuLevel(int32 val);
void ShowMenuLevel(int32 del);
void IncreaseMenuOption(int32 val);
void DecreaseMenuOption(int32 val);
void ResetMenuOption(int32 val);

#endif
//[] END OF FILE
