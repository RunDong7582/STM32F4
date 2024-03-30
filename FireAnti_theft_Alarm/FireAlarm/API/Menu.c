#include "Menu.h"
#include "GUI.h"

extern const GUI_FONT GUI_FontHZ_KaiTi_20;
extern const GUI_FONT GUI_FontHZ_KaiTi_32;
extern float g_tmp;
KEY_TABLE table[30]=
{
	//主界面
    {0,0,0,1,(*fun_0)},

    //实时检测
    {1,4,2, 5,(*fun_a1)},  
    {2,1,3, 9,(*fun_b1)},
    {3,2,4,13,(*fun_c1)},
    {4,3,1, 0,(*fun_d1)}, //no actual function or content,just return the last

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
        //                                               index_3_son1|2|3
        //                                              |----[17]  //information for[5]                                                   
        //                                              |----[18]  //information for[6]
        //                                              |----[19]  //information for[7]
        //                                              |
        //                            index_2_son1------> 
        //                              [5]             index_3_son4|5|6
        //                        |---> [6]             |----[20]
        //                        |     [7]             |----[21]
        //                        |     [8]             |----[22]
        //              index_1   |   index_2_son2------>
        // index_0        [1]     |     [9]             index_3_son7|8|9
        //     |0|------->[2]-------->  [10]            |----[23]
        //                [3]     |     [11]            |----[24]
        //                [4]     |     [12]            |----[25]
        //                        |   index_2_son3------>
        //                        |     [13]
        //                        |---> [14]
        //                              [15]
        //                              [16]

/*
    ***** index_1 *****
*/
void fun_a1()
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
}
void fun_b1()
{
	GUI_SetColor(GUI_BLUE);
	GUI_DrawRoundedFrame(168,16,268,216,8,8);
  	GUI_SetFont(&GUI_FontHZ_KaiTi_32);
	char buf[10];
	sprintf(buf,"%.0f℃", g_tmp);
	GUI_DispStringAt(buf, 20 , 0);
    
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

/*
    ***** index_0 *****
*/

void fun_0()
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
