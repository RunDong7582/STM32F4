#ifndef __MENU_H
#define __MENU_H

#include "GUI.h"
#include "printf.h"

#define PAGE_MAX 40
#define uchar unsigned char
// LOOK UP TABLE

typedef void (*current_page)(float temp, int update);
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


extern Book_Table MENU[PAGE_MAX];


void Book_Pageturn (uchar cur, float temp, int update);
void Boot (float temp , int update);
// void Main_menu(void); 

// First Subtree branch
void RT_Monitor_P1 (float temp , int update);
void RT_Monitor_P2(void);
void RT_Monitor_P3(void);

void DT_Curve_P1(void);
void RF_Msg_P1(void);
void ST_Para_P1(void);

#endif 