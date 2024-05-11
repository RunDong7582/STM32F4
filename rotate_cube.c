#define ARM_MATH_CM4
#include "rotate_cube.h"
#include "GUI.h"
#include "arm_math.h"//ARM官方的DSP库
#define PI 3.1415926
#define XX 0.05
#define YY 0.05
#define ZZ 0.01

float Cube3D[8][3] = {
    {-80,-80,-80},
    {-80, 80,-80},
    { 80, 80,-80},
    { 80,-80,-80},
    {-80,-80, 80},
    {-80, 80, 80},
    { 80, 80, 80},
    { 80,-80, 80}
};


float box_dis[8][3] = {
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0}
};			

int lineid[]= {1,2,2,3,3,4,4,1,5,6,6,7,7,8,8,5,8,4,7,3,6,2,5,1};

void matconv1(float a[3],float b[3][3],float* out)
{
	float res0,res1,res2,aa[3],bb0[3],bb1[3],bb2[3];
	float bbb[3];
	for(int i=0;i<3;i++)aa[i]=a[i];
	for(int i=0;i<3;i++)bb0[i]=b[0][i];
	for(int i=0;i<3;i++)bb1[i]=b[1][i];
	for(int i=0;i<3;i++)bb2[i]=b[2][i];
	arm_dot_prod_f32(aa, bb0, 3, &res0);
	arm_dot_prod_f32(aa, bb1, 3, &res1);
	arm_dot_prod_f32(aa, bb2, 3, &res2);
	out[0]=res0;out[1]=res1;out[2]=res2;
}

/*
arm_cos_f32(float32_t x)
arm_sin_f32(float32_t x)
*/
void rotation_matrix(float cube[8][3],float x,float y,float z,float cube_dis[8][3])
{
	x/=PI;y/=PI;z/=PI;
	float cube_m[8][3];
	float point [8][3];
	float p[3];
	
	float rz[3][3]={{arm_cos_f32(z),-arm_sin_f32(z), 0},
					{arm_sin_f32(z), arm_cos_f32(z), 0},
					{ 0,  0 , 1} };
	
	float rx[3][3]={{ 1,  0 , 0 },
					{0,arm_cos_f32(x),-arm_sin_f32(x)},
					{0,arm_sin_f32(x), arm_cos_f32(x)}};
	
	float ry[3][3]={{arm_cos_f32(y),0,arm_sin_f32(y)},
					{ 0,  1 , 0 },
					{-arm_sin_f32(y),0,arm_cos_f32(y)}};

	for ( int i = 0 ; i < 8 ; i++ )
	{
		
		matconv1( cube[i] , rx , p );
		matconv1(    p    , ry , p );
		matconv1(	 p    , rz , p );

		for ( int j = 0 ; j < 3 ; j++ )
		{
			point[i][j] = p[j];
		}

	}

	for ( int i = 0 ; i < 8 ; i++ )
	{
		for ( int j = 0 ; j < 3 ; j++ )
		{
			cube_dis[i][j] = point[i][j];
		}
	}
}
void Draw_Cube(void)
{
	float x, y, z;
	x = 0;
	y = 0;
	z = 0;
	GUI_Clear();
	GUI_SetBkColor(GUI_BLACK);

	while(1)
	{
		x = x + XX;
		y = y + YY;
		z = z + ZZ;

		rotation_matrix ( Cube3D , x , y , z , box_dis );
		GUI_SetColor    ( GUI_WHITE );
		GUI_FillRect(100,20,300,300);

		for ( int i = 0; i < 24 ; i+=2 )
		{   
			GUI_DrawLine(240+box_dis[lineid[i]-1][0],160+box_dis[lineid[i]-1][1],
						 240+box_dis[lineid[i+1]-1][0],160+box_dis[lineid[i+1]-1][1]);
		}
		GUI_Delay(10);
	}
}


