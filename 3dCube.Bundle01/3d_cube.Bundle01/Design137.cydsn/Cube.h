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
#ifndef CUBE_HEADER
#define CUBE_HEADER

#include <device.h>
#include "Defines.h"
#include "Transforms.h"

void InitializeCube(void);
void RotateCubeX(float angle);
void RotateCubeY(float angle);
void RotateCubeZ(float angle);
void SaveOldLinesCube(void);
void ProjectCube(void);
void DrawCube(void);
void ScaleDistort(void);
void OffsetBounce(void);

#endif
//[] END OF FILE
