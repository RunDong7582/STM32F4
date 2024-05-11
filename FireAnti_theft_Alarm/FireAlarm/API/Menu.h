#ifndef __MENU_H
#define __MENU_H

#define TEMP_FIRST 0b0001
#define  MPU_FIRST 0b0010
#define TEMP_CURVE 0b0100
#define  MPU_PITCH 0b1000
#define   MPU_ROLL 0b1001
#define    MPU_YAW 0b1010
#define   WIFI_ESP 0b1100
#define   PARA_SET 0b1101
#define  TEMP_WARN 0b1111
#define   MPU_WARN 0b1110

#define  AX  4000
#define  AY  4000
#define  AZ  4000 

#define  GX   100
#define  GY   200
#define  GZ   100

#include "GUI.h"
#include "printf.h"
#include "ESP01.h"
#include "rotate_cube.h"

extern const GUI_FONT GUI_FontHZ_YouYuan_24;
extern const GUI_FONT GUI_FontHZ_KaiTi_32;
extern const GUI_FONT GUI_FontHZ_KaiTi_20;
extern const GUI_FONT GUI_FontHZ_KaiTi_12;
extern const GUI_FONT GUI_FontHZ_Zhongyuan_Hz_24;
extern const GUI_FONT GUI_FontHZ_Zhongyuan_HZ_16;
extern GUI_CONST_STORAGE GUI_BITMAP bmcxr;
extern GUI_CONST_STORAGE GUI_BITMAP bmhdu;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarmbw;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm30;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm60;
extern GUI_CONST_STORAGE GUI_BITMAP bmfirealarm82;
#define Len      120

#define PAGE_MAX 40
#define uchar unsigned char
#define __IO volatile

#define tPLOT_NUM 160
#define pPLOT_NUM 160
#define rPLOT_NUM 160
#define yPLOT_NUM 160

typedef struct {
    uchar cur;
    uchar last;
    uint16_t button;
    uint16_t Priority;
    uint16_t Prev_Priority;
    uint16_t update;
    uint16_t gpara;
    uint16_t set;
    uint16_t killbeep;
}Page_t;

typedef struct {
    float new;
    float old;
    __IO int   update;
    __IO int   warn;
    __IO int   cnt;
    __IO float thresh;
}Temperature;

typedef struct {
    __IO short ax;
    __IO short ay;
    __IO short az;
    __IO short gx;
    __IO short gy;
    __IO short gz;
    __IO float fAX;
    __IO float fAY;
    __IO float fAZ;
    __IO int   ok;
    __IO int   warn;
    __IO int   update;
    __IO int   cnt;
}MPUpacket;

typedef struct 
{
    uint16_t button;
    float interval;
    char buf[Len];
}WifiPacket;

typedef struct 
{
    int sensitivity;
    int alarm;
  float upload;
  float maxtemp;

} PARA_list;

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
}MENU_Table;

extern MPUpacket   mpu;
extern MENU_Table  MENU[PAGE_MAX];
extern Page_t      Book;
extern Temperature gtemp;
extern WifiPacket  SensorPack;
extern PARA_list   para;


void Book_Priorswitch (uchar cur);
void Book_Pageturn    (uchar cur, 
                    Temperature temp, MPUpacket mpu);
void Boot          (Temperature temp, MPUpacket mpu);

void RT_Monitor_P1 (Temperature temp , MPUpacket mpu);
void RT_Monitor_P2 (Temperature temp , MPUpacket mpu);
void RT_Monitor_P3 (Temperature temp , MPUpacket mpu);

void DT_Curve_P1   (Temperature temp , MPUpacket mpu);
void DT_Curve_P2   (Temperature temp , MPUpacket mpu);
void DT_Curve_P3   (Temperature temp , MPUpacket mpu);
void DT_Curve_P4   (Temperature temp , MPUpacket mpu);
void DT_Curve_3D   (Temperature temp , MPUpacket mpu);

void RF_Msg_P1     (Temperature temp , MPUpacket mpu);
void RF_Msg_P2     (Temperature temp , MPUpacket mpu);
void ST_Para_P1    (void);

void GUI_TempDrawCurve  (uint8_t margin);
void GUI_PitchDrawCurve (uint8_t margin);
void GUI_RollDrawCurve  (uint8_t margin);
void GUI_YawDrawCurve   (uint8_t margin);

#endif 