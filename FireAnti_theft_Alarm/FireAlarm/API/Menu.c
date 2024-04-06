#include "Menu.h"
#include "GUI.h"


extern GUI_CONST_STORAGE GUI_BITMAP bmhdu;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarmbw;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm30;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm60;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm82;

extern const GUI_FONT GUI_FontHZ_KaiTi_20;
extern const GUI_FONT GUI_FontHZ_KaiTi_32;
extern const GUI_FONT GUI_FontHZ_YouYuan_24;
extern const GUI_FONT GUI_FontHZ_Zhongyuan_HZ_24;

Book_Table MENU[PAGE_MAX]=
{
	//启动界面
	{0,0,0,0,0,1,0,(*Boot)},  // press K6, BACK into BOOT

	//主界面 - 默认为实时监测
    {1,4,2,1,5,1, 0,(*RT_Monitor_P1)},
	{2,1,3,2,7,2, 0,(*DT_Curve_P1)},
	{3,2,4,3,10,3,0,(*RF_Msg_P1)},
	{4,3,1,4,11,4,0,(*ST_Para_P1)},
  
    {5,5,5,1,6,5,0,(*RT_Monitor_P2)},
    {6,6,6,5,1,6,0,(*RT_Monitor_P3)},

};                                          

/**
 * 	轮询
 * 
*/
 
void Book_Pageturn (uchar cur, float temp, int update, MPUpacket mpu)
{  
    for(int i = 0; i < PAGE_MAX; i++ ){
        if ( MENU[i].current_id == cur) {
			if(!update)	GUI_Clear();
            MENU[i].display( temp , update , mpu);
            break;
        } 
    }
}

/**
 * 	Boot 
 * 
*/
void Boot (float temp , int update , MPUpacket mpu)
{
		GUI_SetBkColor(GUI_BLACK);
		GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
		for(int i = 0; i < 61; i++) {
		if(button) {GUI_Clear();GUI_SetBkColor(GUI_GRAY_D0);GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());break;}
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
		if( i == 60 ){
			GUI_SetColor(GUI_BROWN);
			GUI_SetFont(GUI_FONT_20_ASCII);
			GUI_DispStringAt("100%", 132, 204);
			osDelay(500);
		}
		osDelay(20);
		}
		if (!button) {
		GUI_Clear();
		GUI_SetBkColor(GUI_GRAY_D0);
		GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
		GUI_SetFont(&GUI_FontHZ_YouYuan_24);
		GUI_SetColor(GUI_RED);	
		GUI_DrawBitmap(&bmhdu, 0, 0);
		GUI_DispStringAt("防火防盗监测器", 76, 96);
		osDelay(1000);
		GUI_Clear();
		GUI_DispStringAt("21032311", 106, 96);
		GUI_DispStringAt("陈炫润", 114, 126);
		osDelay(2000);
		}
		// Beep(5,100);
}

/*
    Real Time Monitoring P1
*/

void RT_Monitor_P1 (float temp , int update, MPUpacket mpu)
{
	if (!update) {
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
	
	if (!update) GUI_ClearRect(150, 20, 310,220);
	GUI_SetColor(GUI_LIGHTBLUE);
	if (update)  GUI_ClearRect(156, 170, 284,194);
	GUI_AA_FillRoundedRect(150, 20, 310,220,5);
	GUI_SetFont(&GUI_FontComic24B_ASCII);
	GUI_SetColor(GUI_BLACK);
	GUI_SetBkColor(GUI_LIGHTBLUE);
	GUI_SetFont(&GUI_FontHZ_Zhongyuan_HZ_24);
	char buf[10];
	sprintf(buf,"%.1f", temp);
	GUI_DispStringAt(buf, 210 ,170);
	GUI_SetColor(GUI_RED);
	GUI_DispStringAt("温度", 156 ,170);
	GUI_DispStringAt("℃", 260 ,172);
	GUI_SetColor(GUI_DARKMAGENTA);
	GUI_DispStringAt("震动报警", 156 ,52);
	GUI_DispStringAt(gtw ? "有":"无", 208,108);

}

void RT_Monitor_P2(float temp , int update, MPUpacket mpu)
{
   	if (!update) {
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
	
	// if (!update) GUI_ClearRect(150, 20, 310,220);
	GUI_SetColor(GUI_LIGHTBLUE);
	// if (update)  GUI_ClearRect(170, 170, 284,194);
	GUI_AA_FillRoundedRect(150, 20, 310,220,5);
	GUI_SetFont(&GUI_FontComic24B_ASCII);
	// GUI_SetBkColor(GUI_LIGHTBLUE);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font20_ASCII);
	char buf[128];
	sprintf(buf,"%5d",mpu.ax);
	GUI_DispStringAt(buf, 170 ,160);

}

void RT_Monitor_P3(float temp , int update, MPUpacket mpu)
{
	if (!update) {
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
	
	// if (!update) GUI_ClearRect(150, 20, 310,220);
	GUI_SetColor(GUI_LIGHTBLUE);
	// if (update)  GUI_ClearRect(156, 170, 284,194);
	GUI_AA_FillRoundedRect(150, 20, 310,220,5);
	GUI_SetFont(&GUI_FontComic24B_ASCII);
	GUI_SetColor(GUI_BLACK);
	// GUI_SetBkColor(GUI_LIGHTBLUE);
	GUI_SetFont(&GUI_Font10_ASCII);
	char buf[128];
	sprintf(buf,"%5d %5d %5d",mpu.gx,mpu.gy,mpu.gz);
	GUI_DispStringAt(buf, 170 ,160);
}

void DT_Curve_P1(void)
{

}

void RF_Msg_P1(void)
{

}

void ST_Para_P1(void)
{

}