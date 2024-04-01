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

extern float g_tmp;

KEY_TABLE table[30]=
{
	//启动界面
	{0,0,0,1,(*Boot)},  // press K6, BACK into BOOT

	//主界面
    {1,1,1,2,(*Main_menu)},

    //实时检测
    {2,5,3, 6,(*RT_Monitor)},  
    {3,2,4,10,(*fun_b1)},
    {4,3,5,11,(*fun_c1)},
    {5,4,2, 1,(*fun_d1)}, //no actual function or content ,just return the last

    //the index_2
    {5,8,6,17,(*fun_a21)},                  
    {6,5,7,18,(*fun_a22)},
    {7,6,8,19,(*fun_a23)},                                          
    {8,7,5, 1,(*fun_a24)},
    
    { 9,12,10,20,(*fun_b21)},                   
    {10, 9,11,21,(*fun_b22)},
    {11,10,12,22,(*fun_b23)},                                           
    {12,11, 9, 2,(*fun_b24)},
    
    {13,16,14,23,(*fun_c21)},                   
    {14,13,15,24,(*fun_c22)},                                   
    {15,14,16,25,(*fun_c23)},                                   
    {16,15,13, 3,(*fun_c24)},
    
    //the index_3
    {17,17,17,5,(*fun_a31)},                                
    {18,18,18,6,(*fun_a32)},                            
    {19,19,19,7,(*fun_a33)},
    
    {20,20,20, 9,(*fun_b31)},                                   
    {21,21,21,10,(*fun_b32)},                               
    {22,22,22,11,(*fun_b33)},

    {23,23,23,13,(*fun_c31)},                               
    {24,24,24,14,(*fun_c32)},                           
    {25,25,25,15,(*fun_c33)},               

};                                          

/**
 * 	Boot 
 * 
*/
void Boot()
{
    GUI_SetBkColor(GUI_BLACK);
	GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
    for(int i = 0; i < 61; i++) {
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
	//Beep(5,100);
}

/*
    Main Menu
*/

void Main_menu()
{
	GUI_SetBkColor(GUI_GRAY_D0);
	GUI_ClearRect(0, 0, LCD_GetXSize(), LCD_GetYSize());
    GUI_SetColor(GUI_BROWN);
    GUI_SetFont(&GUI_FontHZ_KaiTi_20);
	GUI_DrawRoundedFrame(8,4,108,62,4,6);
	GUI_DrawRoundedFrame(8,62,108,120,4,6);
	GUI_DrawRoundedFrame(8,120,108,178,4,6);
	GUI_DrawRoundedFrame(8,178,108,236,4,6);

	GUI_SetColor(GUI_BLACK);
	GUI_DispStringAt("实时监测",16,24);
	GUI_DispStringAt("数据曲线",16,82);
	GUI_DispStringAt("无线通信",16,140);
	GUI_DispStringAt("参数设置",16,198);
}



/*
    Real Time Monitoring
*/
void RT_Monitor()
{
 	// GUI_SetBkColor(GUI_GRAY_D0);
    GUI_SetColor(GUI_BROWN);
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
	char buf[80];
	sprintf(buf,"%.0f℃", g_tmp);
	GUI_DispStringAt(buf, 200 , 160);
}
void fun_b1()
{
	GUI_SetColor(GUI_BLUE);
	GUI_DrawRoundedFrame(168,16,268,216,8,8);
  	GUI_SetFont(&GUI_FontHZ_KaiTi_32);
    
}
void fun_c1()
{

	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);		
    
}
void fun_d1()
{
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);	
}

/*
    ***** index_2 *****
*/

void fun_a21()     
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																					
}

void fun_a22()      
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_a23()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_a24()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_b21()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_b22()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																								
}

void fun_b23()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_b24()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																				
}

void fun_c21()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																					
}

void fun_c22()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_c23()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																						
}

void fun_c24()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																				
}

/*
    ***** index_3 *****
*/

void fun_a31()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																						
}

void fun_a32()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																					
}

void fun_a33()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_b31()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_b32()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_b33()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																								
}

void fun_c31()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																							
}

void fun_c32()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																						
}

void fun_c33()    
{	
	GUI_DispStringAt("STM32",0,16);
	GUI_DispStringAt("Multi Menu Test",0,40);
	GUI_DispStringAt("",0,64);
	GUI_DispStringAt("Enter to Test",0,88);																					
}

