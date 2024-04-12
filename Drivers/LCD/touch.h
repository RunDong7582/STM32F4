//*************************************************
//作者：XING
//功能描述：触屏函数
//建立时间：2010/11/08  
//最后修改时间：
//*************************************************
/*
  修改历史:
  1,2010/11/23, 10:01,加入软件模拟SPI功能

*/
#ifndef  __TOUCH_H__
#define  __TOUCH_H__

#include "main.h"

// A/D 通道选择命令字和工作寄存器
// #define	CHX 	0xd0 	//通道Y+的选择控制字	
// #define	CHY 	0x90	//通道X+的选择控制字
#define	CHX 	0x90 	//通道Y+的选择控制字	
#define	CHY 	0xd0	//通道X+的选择控制字

#define TP_CS(a) \
		do { \
			if (a) { \
				HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_SET); \
			} else { \
				HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_RESET); \
			} \
		} while (0)

void Touch_GPIO_Config(void);

int GUI_TOUCH_X_MeasureX(void); 
int GUI_TOUCH_X_MeasureY(void);

unsigned int TOUCH_X(void); 
unsigned int TOUCH_Y(void); 
void TP_GetAdXY(unsigned int *x,unsigned int *y);

char IsPressed(uint16_t x, uint16_t y,uint16_t x0, uint16_t y0, uint16_t lenth, uint16_t width,char* pressed);
char IsPressed_V2(uint16_t x, uint16_t y,uint16_t x0, uint16_t y0, uint16_t lenth, uint16_t width,char* pressed);
char IsPressed_V3(uint16_t x, uint16_t y,uint16_t x0, uint16_t y0, uint16_t lenth, uint16_t width,char* pressed);
uint8_t Touch_Poll(int32_t *x,int32_t *y);
void TouchProcess(void);


#endif                                     
