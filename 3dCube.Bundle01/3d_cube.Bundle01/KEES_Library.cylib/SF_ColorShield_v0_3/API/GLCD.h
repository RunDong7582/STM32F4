
/*******************************************************************************
* File Name: `$INSTANCE_NAME`.h  
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
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


#define `$INSTANCE_NAME`_Controller     `$Controller_Type`
#define `$INSTANCE_NAME`_PHILIPS         1u
#define `$INSTANCE_NAME`_EPSON           0u

#define `$INSTANCE_NAME`_SPIM            `$SPIM_Name`
#define `$INSTANCE_NAME`_SPIM_TYPE       `$SPI_Type`
#define `$INSTANCE_NAME`_SPIM_UDB_TYPE    0u 
#define `$INSTANCE_NAME`_SPIM_SCB_TYPE    1u 


/*******************************************************
*				Function Prototypes                    
********************************************************/


void `$INSTANCE_NAME`_Start(void);
void `$INSTANCE_NAME`_Stop(void);
void `$INSTANCE_NAME`_Clear(int32 color);
void `$INSTANCE_NAME`_Contrast(int32 setting);

void `$INSTANCE_NAME`_Pixel(int32 x, int32 y, int32 color );
void `$INSTANCE_NAME`_DrawCircle (int32 x, int32 y, int32 radius, int32 color);

void `$INSTANCE_NAME`_PrintChar(uint8 c, int32 x, int32 y, int32 fColor, int32 bColor);
void `$INSTANCE_NAME`_PrintString(char *pString, int32 x, int32 y, int32 fColor, int32 bColor);

void `$INSTANCE_NAME`_DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, int32 color);
void `$INSTANCE_NAME`_DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, int32 fill, int32 color);

/* Low level commands used by the normal commands.  */
void  `$INSTANCE_NAME`_Command(uint32 datacmd);
void  `$INSTANCE_NAME`_Data(uint32 datadt);
int32 `$INSTANCE_NAME`_SwapColors(int32 in);
void  `$INSTANCE_NAME`_On(void);
void  `$INSTANCE_NAME`_Off(void);


const uint8 `$INSTANCE_NAME`_FONT8x16[97][16];


/********************************************************************
*                                                                  
*					LCD Dimension Definitions                       
*                                                                  
*********************************************************************/
#define `$INSTANCE_NAME`_ROW_LENGTH  132
#define `$INSTANCE_NAME`_COL_HEIGHT  132
#define `$INSTANCE_NAME`_ENDPAGE     132
#define `$INSTANCE_NAME`_ENDCOL      130

#define `$INSTANCE_NAME`_MINX        0u
#define `$INSTANCE_NAME`_MAXX        131u
#define `$INSTANCE_NAME`_MINY        0u
#define `$INSTANCE_NAME`_MAXY        131u




#if (`$INSTANCE_NAME`_Controller == `$INSTANCE_NAME`_EPSON)
#define `$INSTANCE_NAME`_DISPON       0xAF	/* Display on         */
#define `$INSTANCE_NAME`_DISPOFF      0xAE	/* Display off        */
#define `$INSTANCE_NAME`_SLEEPIN      0x95	/* Sleep in           */
#define `$INSTANCE_NAME`_SLEEPOUT     0x94	/* Sleep out          */
#define `$INSTANCE_NAME`_DINVOFF      0xA6	/* Normal display     */
#define `$INSTANCE_NAME`_DINVON       0xA7	/* Inverse display    */
#define `$INSTANCE_NAME`_PAGEADDR     0x75	/* Page address set   */
#define `$INSTANCE_NAME`_COLADDR      0x15	/* Column address set */
#define `$INSTANCE_NAME`_RAMWR        0x5C	/* Writing to memory  */
#define `$INSTANCE_NAME`_SETCONST     0x81	/* Electronic volume control (Contrast) */
#define `$INSTANCE_NAME`_NOP          0x25	/* No op              */

#define	`$INSTANCE_NAME`_CONST_MASK	   0x3F	/* Contrast Mask      */
#define	`$INSTANCE_NAME`_CONST_DEFAULT 0x20	/* Default Contrast   */
#endif

#if (`$INSTANCE_NAME`_Controller == `$INSTANCE_NAME`_PHILIPS)
#define `$INSTANCE_NAME`_DISPON      0x29	/* Display on            */
#define `$INSTANCE_NAME`_DISPOFF     0x28	/* Display off           */
#define `$INSTANCE_NAME`_SLEEPIN     0x10	/* Sleep in              */ 
#define	`$INSTANCE_NAME`_SLEEPOUT	 0x11	/* Sleep out             */
#define	`$INSTANCE_NAME`_DINVOFF	 0x20	/* Display inversion off */
#define `$INSTANCE_NAME`_DINVON      0x21	/* Display inversion ona */
#define `$INSTANCE_NAME`_PAGEADDR    0x2B	/* Page address set      */
#define `$INSTANCE_NAME`_COLADDR     0x2A	/* Column address set    */
#define `$INSTANCE_NAME`_RAMWR       0x2C	/* Memory write          */
#define	`$INSTANCE_NAME`_SETCONST	 0x25	/* Set contrast          */
#define	`$INSTANCE_NAME`_NOP  		 0x00	/* No operation          */
 
#define	`$INSTANCE_NAME`_CONST_MASK	   0x7F	/* Contrast Mask         */
#define	`$INSTANCE_NAME`_CONST_DEFAULT 0x30	/* Default Contrast      */

#endif

/* Command constants  */

/********************************************************************
*
*					EPSON Controller Definitions
*
*********************************************************************/

#define `$INSTANCE_NAME`_COMSCN      0xBB	/* Common scan direction      */
#define `$INSTANCE_NAME`_DISCTL      0xCA	/* Display control            */
#define `$INSTANCE_NAME`_DATCTL      0xBC	/* Data scan direction, etc.  */
#define `$INSTANCE_NAME`_RGBSET8     0xCE	/* 256-color position set     */
#define `$INSTANCE_NAME`_RAMRD       0x5D	/* Reading from memory        */
#define `$INSTANCE_NAME`_PTLIN       0xA8	/* Partial display in         */
#define `$INSTANCE_NAME`_PTLOUT      0xA9	/* Partial display out        */
#define `$INSTANCE_NAME`_RMWIN       0xE0	/* Read and modify write      */
#define `$INSTANCE_NAME`_RMWOUT      0xEE	/* End                        */
#define `$INSTANCE_NAME`_ASCSET      0xAA	/* Area scroll set            */
#define `$INSTANCE_NAME`_SCSTART     0xAB	/* Scroll start set           */
#define `$INSTANCE_NAME`_OSCON       0xD1	/* Internal oscillation on    */
#define `$INSTANCE_NAME`_OSCOFF      0xD2	/* Internal osciallation off  */

#define `$INSTANCE_NAME`_PWRCTR      0x20	/* Power control              */
#define `$INSTANCE_NAME`_VOLUP       0xD6	/* Increment electronic control by 1 */
#define `$INSTANCE_NAME`_VOLDOWN     0xD7	/* Decrement electronic control by 1 */
#define `$INSTANCE_NAME`_TMPGRD      0x82	/* Temperature gradient set   */
#define `$INSTANCE_NAME`_EPCTIN      0xCD	/* Control EEPROM             */
#define `$INSTANCE_NAME`_EPCOUT      0xCC	/* Cancel EEPROM control      */
#define `$INSTANCE_NAME`_EPMWR       0xFC	/* Write into EEPROM          */
#define `$INSTANCE_NAME`_EPMRD       0xFD	/* Read from EEPROM           */
#define `$INSTANCE_NAME`_EPSRRD1     0x7C	/* Read register 1            */
#define `$INSTANCE_NAME`_EPSRRD2     0x7D	/* Read register 2            */

/********************************************************************
*
*			PHILLIPS Controller Definitions
*
*********************************************************************/

#define	`$INSTANCE_NAME`_BSTRON		0x03	/* Booster voltage on              */
#define	`$INSTANCE_NAME`_NORON		0x13	/* Normal display mode on          */
#define `$INSTANCE_NAME`_RGBSET	    0x2D	/* Color set                       */
#define	`$INSTANCE_NAME`_MADCTL		0x36	/* Memory data access control      */
#define	`$INSTANCE_NAME`_COLMOD		0x3A	/* Interface pixel format          */
#define `$INSTANCE_NAME`_DISCTR     0xB9	/* Super frame inversion           */
#define	`$INSTANCE_NAME`_EC			0xC0	/* Internal or external oscillator */

/*******************************************************
*				12-Bit Color Definitions
********************************************************/

#define `$INSTANCE_NAME`_BLACK		0x000
#define `$INSTANCE_NAME`_NAVY 		0x008
#define `$INSTANCE_NAME`_BLUE		0x00F
#define `$INSTANCE_NAME`_TEAL 		0x088
#define `$INSTANCE_NAME`_EMERALD	0x0C5
#define	`$INSTANCE_NAME`_GREEN		0x0F0
#define `$INSTANCE_NAME`_CYAN		0x0FF
#define `$INSTANCE_NAME`_SLATE 		0x244
#define `$INSTANCE_NAME`_INDIGO  	0x408
#define `$INSTANCE_NAME`_TURQUOISE	0x4ED
#define `$INSTANCE_NAME`_OLIVE 		0x682
#define `$INSTANCE_NAME`_MAROON 	0x800
#define `$INSTANCE_NAME`_PURPLE 	0x808
#define `$INSTANCE_NAME`_GRAY 		0x888
#define `$INSTANCE_NAME`_SKYBLUE	0x8CE
#define `$INSTANCE_NAME`_BROWN		0xB22
#define `$INSTANCE_NAME`_CRIMSON 	0xD13
#define `$INSTANCE_NAME`_ORCHID 	0xD7D
#define `$INSTANCE_NAME`_RED		0xF00
#define `$INSTANCE_NAME`_MAGENTA	0xF0F
#define `$INSTANCE_NAME`_ORANGE 	0xF40
#define `$INSTANCE_NAME`_PINK		0xF6A
#define `$INSTANCE_NAME`_CORAL 		0xF75
#define `$INSTANCE_NAME`_SALMON 	0xF87
#define `$INSTANCE_NAME`_GOLD 		0xFD0
#define `$INSTANCE_NAME`_YELLOW		0xFF0
#define `$INSTANCE_NAME`_WHITE		0xFFF



/*******************************************************
*                   Circle Definitions
********************************************************/
#define `$INSTANCE_NAME`_FULLCIRCLE 1
#define `$INSTANCE_NAME`_OPENSOUTH  2
#define `$INSTANCE_NAME`_OPENNORTH  3
#define `$INSTANCE_NAME`_OPENEAST   4
#define `$INSTANCE_NAME`_OPENWEST   5
#define `$INSTANCE_NAME`_OPENNORTHEAST 6
#define `$INSTANCE_NAME`_OPENNORTHWEST 7
#define `$INSTANCE_NAME`_OPENSOUTHEAST 8
#define `$INSTANCE_NAME`_OPENSOUTHWEST 9
















//[] END OF FILE
