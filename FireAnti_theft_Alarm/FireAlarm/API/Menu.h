#ifndef __MENU_H
#define __MENU_H

#include "GUI.h"
#include "printf.h"

#define PAGE_MAX 40
#define uchar unsigned char
// LOOK UP TABLE
typedef struct {
    __IO short ax;
    __IO short ay;
    __IO short az;
    __IO short gx;
    __IO short gy;
    __IO short gz;
}MPUpacket;

typedef void (*current_page)(float temp, int update, MPUpacket mpu);
typedef struct
{
    uchar current_id;
    uchar up;           // roll up 
    uchar down;         // roll down 
    uchar left;         // page left
    uchar right;        // page right
    uchar enter;        // enter
    uchar back;         // back
    current_page display // current index to execute display function
}Book_Table;

extern MPUpacket mpu;
extern Book_Table MENU[PAGE_MAX];
extern uint16_t button;

void Book_Pageturn (uchar cur, float temp, int update, MPUpacket mpu);
void Boot (float temp , int update, MPUpacket mpu);
// void Main_menu(void); 

// First Subtree branch
void RT_Monitor_P1 (float temp , int update, MPUpacket mpu);
void RT_Monitor_P2 (float temp , int update, MPUpacket mpu);
void RT_Monitor_P3 (float temp , int update, MPUpacket mpu);

void DT_Curve_P1(void);
void RF_Msg_P1(void);
void ST_Para_P1(void);

#endif 