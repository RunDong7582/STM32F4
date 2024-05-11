
/*******************************************************************************
* File Name: LCD.h  
* Version 0.3
*
*  Description:
*    This file contains all the API function prototypes for the SparkFun Color 
*    Graphics Arduino Shield board which uses a Nokia 6100 LCD.
*
* Note:  This code has been tested with the Philips PCF8833 controller, but not
*        the EPSON S115D10 controller as of release 0.3.
*******************************************************************************
* The following firmware was developed by Cypress Semiconductor
* This work is licensed under a Creative Commons Attribution 3.0 Unported License.
*
* http://creativecommons.org/licenses/by/3.0/deed.en_US
* 
* You are free to:
* -To Share — to copy, distribute and transmit the work 
* -To Remix — to adapt the work 
* -To make commercial use of the work
*
* Much of this code was ported from the LCD_driver code by Mark Sproul
* and Peter DavenPort.  Also the tutorial by James P. Lynch
* on how the controller works was invaluable.
********************************************************************************/


#define LCD_Controller     1
#define LCD_PHILIPS         1u
#define LCD_EPSON           0u

#define LCD_SPIM            SPIM_UDB
#define LCD_SPIM_TYPE       0
#define LCD_SPIM_UDB_TYPE    0u 
#define LCD_SPIM_SCB_TYPE    1u 


/*******************************************************
*				Function Prototypes                    
********************************************************/


void LCD_Start(void);
void LCD_Stop(void);
void LCD_Clear(int32 color);
void LCD_Contrast(int32 setting);

void LCD_Pixel(int32 x, int32 y, int32 color );
void LCD_DrawCircle (int32 x, int32 y, int32 radius, int32 color);

void LCD_PrintChar(uint8 c, int32 x, int32 y, int32 fColor, int32 bColor);
void LCD_PrintString(char *pString, int32 x, int32 y, int32 fColor, int32 bColor);

void LCD_DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, int32 color);
void LCD_DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, int32 fill, int32 color);

/* Low level commands used by the normal commands.  */
void  LCD_Command(uint32 datacmd);
void  LCD_Data(uint32 datadt);
int32 LCD_SwapColors(int32 in);
void  LCD_On(void);
void  LCD_Off(void);


const uint8 LCD_FONT8x16[97][16];


/********************************************************************
*                                                                  
*					LCD Dimension Definitions                       
*                                                                  
*********************************************************************/
#define LCD_ROW_LENGTH  132
#define LCD_COL_HEIGHT  132
#define LCD_ENDPAGE     132
#define LCD_ENDCOL      130

#define LCD_MINX        0u
#define LCD_MAXX        131u
#define LCD_MINY        0u
#define LCD_MAXY        131u




#if (LCD_Controller == LCD_EPSON)
#define LCD_DISPON       0xAF	/* Display on         */
#define LCD_DISPOFF      0xAE	/* Display off        */
#define LCD_SLEEPIN      0x95	/* Sleep in           */
#define LCD_SLEEPOUT     0x94	/* Sleep out          */
#define LCD_DINVOFF      0xA6	/* Normal display     */
#define LCD_DINVON       0xA7	/* Inverse display    */
#define LCD_PAGEADDR     0x75	/* Page address set   */
#define LCD_COLADDR      0x15	/* Column address set */
#define LCD_RAMWR        0x5C	/* Writing to memory  */
#define LCD_SETCONST     0x81	/* Electronic volume control (Contrast) */
#define LCD_NOP          0x25	/* No op              */

#define	LCD_CONST_MASK	   0x3F	/* Contrast Mask      */
#define	LCD_CONST_DEFAULT 0x20	/* Default Contrast   */
#endif

#if (LCD_Controller == LCD_PHILIPS)
#define LCD_DISPON      0x29	/* Display on            */
#define LCD_DISPOFF     0x28	/* Display off           */
#define LCD_SLEEPIN     0x10	/* Sleep in              */ 
#define	LCD_SLEEPOUT	 0x11	/* Sleep out             */
#define	LCD_DINVOFF	 0x20	/* Display inversion off */
#define LCD_DINVON      0x21	/* Display inversion ona */
#define LCD_PAGEADDR    0x2B	/* Page address set      */
#define LCD_COLADDR     0x2A	/* Column address set    */
#define LCD_RAMWR       0x2C	/* Memory write          */
#define	LCD_SETCONST	 0x25	/* Set contrast          */
#define	LCD_NOP  		 0x00	/* No operation          */
 
#define	LCD_CONST_MASK	   0x7F	/* Contrast Mask         */
#define	LCD_CONST_DEFAULT 0x30	/* Default Contrast      */

#endif

/* Command constants  */

/********************************************************************
*
*					EPSON Controller Definitions
*
*********************************************************************/

#define LCD_COMSCN      0xBB	/* Common scan direction      */
#define LCD_DISCTL      0xCA	/* Display control            */
#define LCD_DATCTL      0xBC	/* Data scan direction, etc.  */
#define LCD_RGBSET8     0xCE	/* 256-color position set     */
#define LCD_RAMRD       0x5D	/* Reading from memory        */
#define LCD_PTLIN       0xA8	/* Partial display in         */
#define LCD_PTLOUT      0xA9	/* Partial display out        */
#define LCD_RMWIN       0xE0	/* Read and modify write      */
#define LCD_RMWOUT      0xEE	/* End                        */
#define LCD_ASCSET      0xAA	/* Area scroll set            */
#define LCD_SCSTART     0xAB	/* Scroll start set           */
#define LCD_OSCON       0xD1	/* Internal oscillation on    */
#define LCD_OSCOFF      0xD2	/* Internal osciallation off  */

#define LCD_PWRCTR      0x20	/* Power control              */
#define LCD_VOLUP       0xD6	/* Increment electronic control by 1 */
#define LCD_VOLDOWN     0xD7	/* Decrement electronic control by 1 */
#define LCD_TMPGRD      0x82	/* Temperature gradient set   */
#define LCD_EPCTIN      0xCD	/* Control EEPROM             */
#define LCD_EPCOUT      0xCC	/* Cancel EEPROM control      */
#define LCD_EPMWR       0xFC	/* Write into EEPROM          */
#define LCD_EPMRD       0xFD	/* Read from EEPROM           */
#define LCD_EPSRRD1     0x7C	/* Read register 1            */
#define LCD_EPSRRD2     0x7D	/* Read register 2            */

/********************************************************************
*
*			PHILLIPS Controller Definitions
*
*********************************************************************/

#define	LCD_BSTRON		0x03	/* Booster voltage on              */
#define	LCD_NORON		0x13	/* Normal display mode on          */
#define LCD_RGBSET	    0x2D	/* Color set                       */
#define	LCD_MADCTL		0x36	/* Memory data access control      */
#define	LCD_COLMOD		0x3A	/* Interface pixel format          */
#define LCD_DISCTR     0xB9	/* Super frame inversion           */
#define	LCD_EC			0xC0	/* Internal or external oscillator */

/*******************************************************
*				12-Bit Color Definitions
********************************************************/

#define LCD_BLACK		0x000
#define LCD_NAVY 		0x008
#define LCD_BLUE		0x00F
#define LCD_TEAL 		0x088
#define LCD_EMERALD	0x0C5
#define	LCD_GREEN		0x0F0
#define LCD_CYAN		0x0FF
#define LCD_SLATE 		0x244
#define LCD_INDIGO  	0x408
#define LCD_TURQUOISE	0x4ED
#define LCD_OLIVE 		0x682
#define LCD_MAROON 	0x800
#define LCD_PURPLE 	0x808
#define LCD_GRAY 		0x888
#define LCD_SKYBLUE	0x8CE
#define LCD_BROWN		0xB22
#define LCD_CRIMSON 	0xD13
#define LCD_ORCHID 	0xD7D
#define LCD_RED		0xF00
#define LCD_MAGENTA	0xF0F
#define LCD_ORANGE 	0xF40
#define LCD_PINK		0xF6A
#define LCD_CORAL 		0xF75
#define LCD_SALMON 	0xF87
#define LCD_GOLD 		0xFD0
#define LCD_YELLOW		0xFF0
#define LCD_WHITE		0xFFF



/*******************************************************
*                   Circle Definitions
********************************************************/
#define LCD_FULLCIRCLE 1
#define LCD_OPENSOUTH  2
#define LCD_OPENNORTH  3
#define LCD_OPENEAST   4
#define LCD_OPENWEST   5
#define LCD_OPENNORTHEAST 6
#define LCD_OPENNORTHWEST 7
#define LCD_OPENSOUTHEAST 8
#define LCD_OPENSOUTHWEST 9
















//[] END OF FILE
