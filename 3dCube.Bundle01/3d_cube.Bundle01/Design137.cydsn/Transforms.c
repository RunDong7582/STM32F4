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

#include "Transforms.h"

float r = 0.2; // z
float p = 0; // x
float q = 0; // y

float X_OFFSET;
float Y_OFFSET;
float X_SCALE; 
float Y_SCALE;

void topolar(float a, float b, float * r, float * theta)
{
	*theta = atan2(b, a);
	*r = sqrt(pow(a,2) + pow(b,2));
	
	return;
}

void torect(float r, float theta, float * a, float * b)
{
	*a = r*cos(theta);
	*b = r*sin(theta);
	
	return;
}

void rotate(float * a, float * b, float theta)
{
	float r, theta_new;
	
	topolar(*a, *b, &r, &theta_new);
	theta_new = theta_new + theta;
	torect(r, theta_new, a, b);
	
	return;
}

void rotatex(float * ax, float * ay, float * az, float theta)
{
	rotate(ay, az, theta);
	
	return;
}

void rotatey(float * ax, float * ay, float * az, float theta)
{
	rotate(az, ax, theta);
	
	return;
}

void rotatez(float * ax, float * ay, float * az, float theta)
{
	rotate(ax, ay, theta);
	
	return;
}

void project(float ax, float ay, float az, float * bx, float * by)
{	
	
	// three point persepctive
	*bx = ax/(p*ax+q*ay+r*az+1)*X_SCALE + X_OFFSET;
	*by = ay/(p*ax+q*ay+r*az+1)*Y_SCALE + Y_OFFSET;
	
	// two point persepctive
//	*bx = ax/(p*ax+q*ay+1)*X_SCALE + X_OFFSET;
//	*by = ay/(p*ax+q*ay+1)*Y_SCALE + Y_OFFSET;
	
	// single point persepctive
//	*bx = ax/(r*az+1)*X_SCALE + X_OFFSET;
//	*by = ay/(r*az+1)*Y_SCALE + Y_OFFSET;
	
	// no perspective
//	*bx = ax*X_SCALE + X_OFFSET;
//	*by = ay*Y_SCALE + Y_OFFSET;
	
	// simple protection to prevent display freakout
	
	if(*bx < 0)
	{
		*bx = 0;
	}
	
	if(*by < 0)
	{
		*by = 0;
	}
	
	return;
}

/* [] END OF FILE */
