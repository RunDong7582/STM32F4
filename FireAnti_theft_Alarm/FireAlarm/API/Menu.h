#ifndef __MENU_H
#define __MENU_H

#include "GUI.h"
#include "printf.h"

extern const GUI_FONT GUI_FontHZ_YouYuan_24;
extern const GUI_FONT GUI_FontHZ_KaiTi_32;
extern const GUI_FONT GUI_FontHZ_KaiTi_20;
extern const GUI_FONT GUI_FontHZ_KaiTi_12;
extern const GUI_FONT GUI_FontHZ_Zhongyuan_HZ_24;
extern GUI_CONST_STORAGE GUI_BITMAP bmhdu;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarmbw;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm30;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm60;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm82;

#define PAGE_MAX 40
#define uchar unsigned char
#define __IO volatile

typedef struct {
    uchar cur;
    uchar last;
    uint16_t button;
}Page_t;

typedef struct {
    float new;
    float old;
    __IO int update;
}Temperature;

typedef struct {
    __IO short ax;
    __IO short ay;
    __IO short az;
    __IO short gx;
    __IO short gy;
    __IO short gz;
    __IO int   ok;
    __IO int   warn;
    __IO int   update;
}MPUpacket;

typedef void (*current_page) ( Temperature temp, MPUpacket mpu );
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
extern Page_t Book;
extern Temperature gtemp;

void Book_Pageturn (uchar cur, 
                    Temperature temp, MPUpacket mpu);
void Boot          (Temperature temp, MPUpacket mpu);

void RT_Monitor_P1 (Temperature temp , MPUpacket mpu);
void RT_Monitor_P2 (Temperature temp , MPUpacket mpu);
void RT_Monitor_P3 (Temperature temp , MPUpacket mpu);

void DT_Curve_P1(void);
void RF_Msg_P1(void);
void ST_Para_P1(void);

#endif 