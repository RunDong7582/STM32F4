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
#ifndef TRANSFORMS_HEADER
#define TRANSFORMS_HEADER

#include <device.h>
#include <math.h>
#include "Defines.h"

void topolar(float a, float b, float * r, float * theta);
void torect(float r, float theta, float * a, float * b);
void rotate(float * a, float * b, float theta);
void rotatex(float * ax, float * ay, float * az, float theta);
void rotatey(float * ax, float * ay, float * az, float theta);
void rotatez(float * ax, float * ay, float * az, float theta);
void project(float ax, float ay, float az, float * bx, float * by);

extern float X_OFFSET;
extern float Y_OFFSET;
extern float X_SCALE; 
extern float Y_SCALE;

extern float r; // z
extern float p; // x
extern float q; // y

#endif

//[] END OF FILE
