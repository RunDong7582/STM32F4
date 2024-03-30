#ifndef __MENU_H
#define __MENU_H

#include "GUI.h"

#define uchar unsigned char
// LOOK UP TABLE

typedef struct
{
    uchar current_id;
    uchar up;           // roll up to check index
    uchar down;         // roll down to check index
    uchar enter;        // ensure the index
    void (*current_operation)() // current index to execute display function
} KEY_TABLE;

extern KEY_TABLE table[30];

extern void fun_a1(void); 
extern void fun_b1(void);  
extern void fun_c1(void);  
extern void fun_d1(void);  

extern void fun_a21(void); 
extern void fun_a22(void);  
extern void fun_a23(void);  
extern void fun_a24(void);  

extern void fun_b21(void); 
extern void fun_b22(void);  
extern void fun_b23(void);  
extern void fun_b24(void); 

extern void fun_c21(void); 
extern void fun_c22(void);  
extern void fun_c23(void);  
extern void fun_c24(void);  

extern void fun_a31(void); 
extern void fun_a32(void);  
extern void fun_a33(void);  

extern void fun_b31(void); 
extern void fun_b32(void);  
extern void fun_b33(void); 

extern void fun_c31(void); 
extern void fun_c32(void);  
extern void fun_c33(void); 
  
extern void fun_0(void);   

#endif 