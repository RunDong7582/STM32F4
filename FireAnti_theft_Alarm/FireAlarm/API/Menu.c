#include "Menu.h"

extern float vPitch[pPLOT_NUM];
extern float  vTemp[tPLOT_NUM];
extern float  vRoll[rPLOT_NUM];
extern float   vYaw[yPLOT_NUM];

MENU_Table MENU[PAGE_MAX]=
{
	//启动界面
	{0,0,0,0,0,1,0,(*Boot)},  

	//主界面 - 默认为实时监测
    {1,4,2,1,5,1, 0,(*RT_Monitor_P1)},
	{2,1,3,2,7,2, 0,(*DT_Curve_P1)},
	{3,2,4,3,10,3,0,(*RF_Msg_P1)},
	{4,3,1,4,11,4,0,(*ST_Para_P1)},
  
	//子菜单 - 实时监测 
    {5,5,5,1,6,5,0,(*RT_Monitor_P2)},
    {6,6,6,5,1,6,0,(*RT_Monitor_P3)},

	//子菜单 - 数据曲线
	{7,7,7,2,8,7, 0,(*DT_Curve_P2)},
	{8,8,8,9,9,8, 0,(*DT_Curve_P3)},
	{9,9,9,7,2,9, 0,(*DT_Curve_P4)},
};                                          

void Book_Priorswitch ( uchar cur ) 
{
	Book.Prev_Priority = Book.Priority;
	switch (cur)
	{
		case 0:
			Book.Priority = TEMP_FIRST;
		break;
		case 1:
			Book.Priority = TEMP_FIRST;
		break;
		case 2:
			Book.Priority = TEMP_CURVE;
		break;
		case 5:
			Book.Priority = MPU_FIRST;
		break;
		case 6:
			Book.Priority = MPU_FIRST;
		break;
		case 7:
			Book.Priority = MPU_PITCH;
		break;
		case 8:
			Book.Priority =  MPU_ROLL;
		break;
		case 9:
			Book.Priority =   MPU_YAW;
		break;
		default:
		break;
	}
}


/**
 * 	轮询
 * 
*/
 
void Book_Pageturn (uchar cur, Temperature temp , MPUpacket mpu)
{  
    for (int i = 0; i < PAGE_MAX; i++ ) {

        if ( MENU[i].current_id == cur) 
		{
			if ( !temp.update || !mpu.update ) 
					GUI_Clear();

            MENU[i].display ( temp , mpu );
            break;
        } 

    }
}

/**
 * 	Boot 
 * 
*/
void Boot ( Temperature temp , MPUpacket mpu )
{
		GUI_SetBkColor(GUI_BLACK);
		GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
		for (int i = 0; i < 61; i++) {

			if(Book.button) 
			{	
				GUI_Clear();
				GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
				GUI_SetDefaultBkColor(GUI_GRAY_D0);
				break;
			}

			if ( i % 5 == 0 && i!=60 )
				GUI_ClearRect(88,48,216,176);
			else if ( i % 5 == 1)
				GUI_DrawBitmap(&bmfirealarm30, 88, 48);
			else if ( i % 5 == 2)
				GUI_DrawBitmap(&bmfirealarm60, 88, 48);
			else if ( i % 5 == 3)
				GUI_DrawBitmap(&bmfirealarm82, 88, 48);
			else if ( i % 5 == 4)
				GUI_DrawBitmap(&bmfirealarm, 88, 48);

			GUI_SetColor(GUI_YELLOW);
			GUI_FillRect(40,200,4*i+40,220);
			GUI_DrawRect(40,200,280,220);

			if ( i == 60 ) 
			{
				GUI_SetColor(GUI_BROWN);
				GUI_SetFont(GUI_FONT_20_ASCII);
				GUI_DispStringAt("100%", 132, 204);
				osDelay(500);
			}

			osDelay(20);
		}

		if (!Book.button) 
		{
			GUI_Clear();
			GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
			GUI_SetBkColor(GUI_GRAY_D0);
			GUI_SetFont(&GUI_FontHZ_YouYuan_24);
			GUI_SetColor(GUI_RED);	
			GUI_DrawBitmap(&bmhdu, 0, 0);
			GUI_DispStringAt("防火防盗监测器", 76, 96);
			if (!Book.button) 
        		osDelay(1000);
			GUI_Clear();
			GUI_DispStringAt("21032311", 106, 96);
			GUI_DispStringAt("陈炫润", 114, 126);
			if (!Book.button) 
        		osDelay(2000);
		}
		// Beep(5,100);
}

/*
    Real Time Monitoring P1
*/

void RT_Monitor_P1 ( Temperature temp, MPUpacket mpu )
{
	if ( !temp.update || Book.update ) 
	{
		GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
	}

	GUI_SetBkColor(GUI_GRAY_D0);
    GUI_SetColor(GUI_BROWN);
	GUI_SetFont(&GUI_FontHZ_KaiTi_20);
	GUI_DrawRoundedFrame(8,62,108,120,4,6);
	GUI_DrawRoundedFrame(8,120,108,178,4,6);
	GUI_DrawRoundedFrame(8,178,108,236,4,6);
	GUI_SetColor(GUI_RED);
	GUI_DrawRoundedFrame(8,4,108,62,4,6);

	GUI_SetColor(GUI_BLACK);
	GUI_DispStringAt("实时监测",16,24);
	GUI_DispStringAt("数据曲线",16,82);
	GUI_DispStringAt("无线通信",16,140);
	GUI_DispStringAt("参数设置",16,198); 
	
	if (!temp.update || Book.update) 
		GUI_ClearRect(150, 20, 310,220);

	GUI_SetColor(GUI_LIGHTBLUE);

	if (temp.update)  
		GUI_ClearRect(156, 170, 284,194);
	
	GUI_AA_FillRoundedRect(150, 20, 310,220,5);
	GUI_SetFont(&GUI_FontComic24B_ASCII);
	GUI_SetColor(GUI_BLACK);
	if ( temp.update ) GUI_SetBkColor(GUI_LIGHTBLUE);
	GUI_SetFont(&GUI_FontHZ_Zhongyuan_HZ_24);

	char buf[10];
	sprintf ( buf, "%.1f" , temp.update?temp.new:temp.old);

	GUI_DispStringAt(buf, 210 ,170);
	GUI_SetColor(GUI_RED);
	GUI_DispStringAt("温度", 156 ,170);
	GUI_DispStringAt("℃", 260 ,172);
	GUI_SetColor(GUI_DARKMAGENTA);
	GUI_DispStringAt("震动报警", 156 ,52);
	GUI_DispStringAt(mpu.warn ? "有":"无", 208,108);

}

void RT_Monitor_P2 ( Temperature temp, MPUpacket mpu )
{
   	if (!mpu.update || Book.update) 
	{
		GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
	}
		GUI_SetBkColor(GUI_GRAY_D0);
		GUI_SetColor(GUI_BROWN);
		GUI_SetFont(&GUI_FontHZ_KaiTi_20);
		GUI_DrawRoundedFrame(8,62,108,120,4,6);
		GUI_DrawRoundedFrame(8,120,108,178,4,6);
		GUI_DrawRoundedFrame(8,178,108,236,4,6);
		GUI_SetColor(GUI_RED);
		GUI_DrawRoundedFrame(8,4,108,62,4,6);

		GUI_SetColor(GUI_BLACK);
		GUI_DispStringAt("实时监测",16,24);
		GUI_DispStringAt("数据曲线",16,82);
		GUI_DispStringAt("无线通信",16,140);
		GUI_DispStringAt("参数设置",16,198); 
	
	if (!mpu.update) 
		GUI_ClearRect(150, 20, 310,220);

	GUI_SetColor(GUI_LIGHTBLUE);
	
	if (mpu.update) 
		GUI_ClearRect(170,100,240,190);

	GUI_AA_FillRoundedRect(150, 20, 310, 220, 5);
	GUI_SetFont(&GUI_FontComic24B_ASCII);
	GUI_SetColor(GUI_DARKYELLOW);
	// GUI_SetBkColor(GUI_LIGHTBLUE);

	char bufA[40];
	sprintf(bufA,"ax: %5d",mpu.ax);
	GUI_DispStringAt(bufA, 170 ,100);
	char bufB[40];
	sprintf(bufB,"ay: %5d",mpu.ay);
	GUI_DispStringAt(bufB, 170 ,130);
	char bufC[40];
	sprintf(bufC,"az: %5d",mpu.az);
	GUI_DispStringAt(bufC, 170 ,160);
}

void RT_Monitor_P3 ( Temperature temp, MPUpacket mpu )
{
   	if (!mpu.update) 
	{
		GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
	}
		GUI_SetBkColor(GUI_GRAY_D0);
		GUI_SetColor(GUI_BROWN);
		GUI_SetFont(&GUI_FontHZ_KaiTi_20);
		GUI_DrawRoundedFrame(8,62,108,120,4,6);
		GUI_DrawRoundedFrame(8,120,108,178,4,6);
		GUI_DrawRoundedFrame(8,178,108,236,4,6);
		GUI_SetColor(GUI_RED);
		GUI_DrawRoundedFrame(8,4,108,62,4,6);

		GUI_SetColor(GUI_BLACK);
		GUI_DispStringAt("实时监测",16,24);
		GUI_DispStringAt("数据曲线",16,82);
		GUI_DispStringAt("无线通信",16,140);
		GUI_DispStringAt("参数设置",16,198); 
	
	if (!mpu.update) 
		GUI_ClearRect(150, 20, 310,220);

	GUI_SetColor(GUI_LIGHTBLUE);
	
	if (mpu.update) 
		GUI_ClearRect(170,100,240,190);

	GUI_AA_FillRoundedRect(150, 20, 310, 220, 5);
	GUI_SetFont(&GUI_FontComic24B_ASCII);
	GUI_SetColor(GUI_DARKYELLOW);

	char bufA[40];
	sprintf(bufA,"gx: %5d",mpu.gx);
	GUI_DispStringAt(bufA, 170 ,100);
	char bufB[40];
	sprintf(bufB,"gy: %5d",mpu.gy);
	GUI_DispStringAt(bufB, 170 ,130);
	char bufC[40];
	sprintf(bufC,"gz: %5d",mpu.gz);
	GUI_DispStringAt(bufC, 170 ,160);
}

void DT_Curve_P1 ( Temperature temp, MPUpacket mpu )
{
	// GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
	GUI_SetBkColor(GUI_GRAY_D0);
    GUI_SetColor(GUI_BROWN);
	GUI_SetFont(&GUI_FontHZ_KaiTi_20);
	GUI_DrawRoundedFrame(8,4,108,62,4,6);
	GUI_DrawRoundedFrame(8,120,108,178,4,6);
	GUI_DrawRoundedFrame(8,178,108,236,4,6);
	GUI_SetColor(GUI_RED);
	GUI_DrawRoundedFrame(8,62,108,120,4,6);

	GUI_SetColor(GUI_BLACK);
	GUI_DispStringAt("实时监测",16,24);
	GUI_DispStringAt("数据曲线",16,82);
	GUI_DispStringAt("无线通信",16,140);
	GUI_DispStringAt("参数设置",16,198); 

	GUI_SetColor(GUI_BROWN);
	GUI_DrawRoundedFrame(170, 70, 290, 210,5,6);

	char buf[36];
	GUI_SetFont(&GUI_FontHZ_Zhongyuan_HZ_24);
	GUI_SetColor(GUI_DARKRED);
	sprintf(buf, "温度:%.1f℃",temp.new);
	GUI_DispStringAt(buf, 180, 30);
	GUI_TempDrawCurve(10);
}

void DT_Curve_P2 ( Temperature temp, MPUpacket mpu )
{
	GUI_SetBkColor(GUI_GRAY_D0);
    GUI_SetColor(GUI_BROWN);
	GUI_SetFont(&GUI_FontHZ_KaiTi_20);
	GUI_DrawRoundedFrame(8,4,108,62,4,6);
	GUI_DrawRoundedFrame(8,120,108,178,4,6);
	GUI_DrawRoundedFrame(8,178,108,236,4,6);
	GUI_SetColor(GUI_RED);
	GUI_DrawRoundedFrame(8,62,108,120,4,6);

	GUI_SetColor(GUI_BLACK);
	GUI_DispStringAt("实时监测",16,24);
	GUI_DispStringAt("数据曲线",16,82);
	GUI_DispStringAt("无线通信",16,140);
	GUI_DispStringAt("参数设置",16,198); 

	GUI_SetColor(GUI_DARKCYAN);
	GUI_DrawRoundedFrame(120, 70, 310, 230, 5, 6);
	char buf[36];
	GUI_SetFont(&GUI_FontHZ_Zhongyuan_HZ_24);
	GUI_SetColor(GUI_BLUE);
	sprintf(buf, "俯仰角:%.1f℃", mpu.fAX);
	GUI_DispStringAt(buf, 160, 20);
	GUI_PitchDrawCurve(45);
}

void DT_Curve_P3 ( Temperature temp, MPUpacket mpu )
{
	GUI_SetBkColor(GUI_GRAY_D0);
    GUI_SetColor(GUI_BROWN);
	GUI_SetFont(&GUI_FontHZ_KaiTi_20);
	GUI_DrawRoundedFrame(8,4,108,62,4,6);
	GUI_DrawRoundedFrame(8,120,108,178,4,6);
	GUI_DrawRoundedFrame(8,178,108,236,4,6);
	GUI_SetColor(GUI_RED);
	GUI_DrawRoundedFrame(8,62,108,120,4,6);

	GUI_SetColor(GUI_BLACK);
	GUI_DispStringAt("实时监测",16,24);
	GUI_DispStringAt("数据曲线",16,82);
	GUI_DispStringAt("无线通信",16,140);
	GUI_DispStringAt("参数设置",16,198); 

	GUI_SetColor(GUI_DARKCYAN);
	GUI_DrawRoundedFrame(120, 50, 290, 230, 5, 6);

	char buf[36];
	GUI_SetFont(&GUI_FontHZ_Zhongyuan_HZ_24);
	GUI_SetColor(GUI_BLUE);
	sprintf(buf, "滚转角:%.1f℃", mpu.fAY);
	GUI_DispStringAt(buf, 160, 20);
	GUI_RollDrawCurve(40);
}

void DT_Curve_P4 ( Temperature temp, MPUpacket mpu )
{
	GUI_SetBkColor(GUI_GRAY_D0);
    GUI_SetColor(GUI_BROWN);
	GUI_SetFont(&GUI_FontHZ_KaiTi_20);
	GUI_DrawRoundedFrame(8,4,108,62,4,6);
	GUI_DrawRoundedFrame(8,120,108,178,4,6);
	GUI_DrawRoundedFrame(8,178,108,236,4,6);
	GUI_SetColor(GUI_RED);
	GUI_DrawRoundedFrame(8,62,108,120,4,6);

	GUI_SetColor(GUI_BLACK);
	GUI_DispStringAt("实时监测",16,24);
	GUI_DispStringAt("数据曲线",16,82);
	GUI_DispStringAt("无线通信",16,140);
	GUI_DispStringAt("参数设置",16,198); 

	GUI_SetColor(GUI_DARKCYAN);
	GUI_DrawRoundedFrame(120, 70, 290, 230, 5, 6);

	char buf[36];
	GUI_SetFont(&GUI_FontHZ_Zhongyuan_HZ_24);
	GUI_SetColor(GUI_BLUE);
	sprintf(buf, "偏航角:%.1f℃", mpu.fAZ);
	GUI_DispStringAt(buf, 160, 20);
	GUI_YawDrawCurve(50);
}


void RF_Msg_P1(void)
{

}

void ST_Para_P1(void)
{

}

void GUI_TempDrawCurve (uint8_t margin)
{
	/// @brief Draw temparture curve on the LCD 

	static int i;

	float tmpMin = 20.0f;
	float tmpMax = 35.0f;
	float tmpThresh = 30.0f;
	int sh = LCD_GetYSize() - 2 * margin;
	int sw = LCD_GetXSize() - 170;
	int ox = 170;
	int oy = margin + sh;

    float coef = sh / (tmpMax - tmpMin);   

	for ( i = 0; i < tPLOT_NUM; i += 10 )
	{
		GUI_DrawHLine(oy - (tmpThresh - tmpMin) * coef , ox + i, ox + i + 5);
	}
		
	for ( i = 1; i < gtemp.cnt && i < tPLOT_NUM; ++i ) 
	{
		GUI_DrawLine(ox + i - 1, oy - (vTemp[i-1] - tmpMin) * coef, ox + i, oy - (vTemp[i] - tmpMin) * coef);
	}

}

void GUI_PitchDrawCurve (uint8_t margin)
{
	/// @brief Draw temparture curve on the LCD 

	static int i;
	float angmin = -90.0f;
	float angmax =  90.0f;

	int sh = LCD_GetYSize() - 2 * margin;
	int sw = LCD_GetXSize() - 130;
	int ox = 130;
	int oy = margin + sh;

    float coef = sh / (angmax - angmin);   
	GUI_DrawHLine(oy + angmin * coef, ox, ox + sw);

	for ( i = 1; i < mpu.cnt && i < pPLOT_NUM; ++i ) 
	{
		GUI_DrawLine(ox + i - 1, oy - (vPitch[i-1] - angmin) * coef, ox + i, oy - (vPitch[i] - angmin) * coef);
	}

}

void GUI_RollDrawCurve (uint8_t margin)
{
	/// @brief Draw temparture curve on the LCD 

	static int i;
	float angmin = -60.0f;
	float angmax =  60.0f;

	int sh = LCD_GetYSize() - 2 * margin;
	int sw = LCD_GetXSize() - 130;
	int ox = 130;
	int oy = margin + sh;

    float coef = sh / (angmax - angmin);   
	GUI_DrawHLine(oy + angmin * coef, ox, ox + sw);

	for ( i = 1; i < mpu.cnt && i < rPLOT_NUM ; ++i )
	{
		GUI_DrawLine(ox + i - 1, oy - (vRoll[i-1] - angmin) * coef, ox + i, oy - (vRoll[i] - angmin) * coef);
	}

}

void GUI_YawDrawCurve (uint8_t margin)
{
	/// @brief Draw temparture curve on the LCD 

	static int i;
	float angmin = -60.0f;
	float angmax =  60.0f;

	int sh = LCD_GetYSize() - 2 * margin;
	int sw = LCD_GetXSize() - 130;
	int ox = 130;
	int oy = margin + sh;

    float coef = sh / (angmax - angmin);   
	GUI_DrawHLine(oy + angmin * coef, ox, ox + sw);

	for (i = 1; i < mpu.cnt && i < yPLOT_NUM; ++i) 
	{
		GUI_DrawLine(ox + i - 1, oy - (vYaw[i-1] - angmin) * coef, ox + i, oy - (vYaw[i] - angmin) * coef);
	}
}