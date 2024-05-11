#ifndef __ROTATE_CUBE_H
#define __ROTATE_CUBE_H

void matconv1(float a[3],float b[3][3],float* out);
void rotation_matrix(float cube[8][3],float x,float y,float z,float cube_dis[8][3]);
void Draw_Cube(float x, float y, float z);


#endif