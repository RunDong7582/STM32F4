/*******************************************************************************
* File Name: LCD.c  
* Version 0.3
*
*  Description:
*    This file contains all the API for the SparkFun Color Graphics
*    Arduino Shield board which uses a Nokia 6100 LCD.
*
* Note:  This code has been tested with the Philips PCF8833 controller, but not
*        the EPSON S115D10 controller as of release 0.3.
*
*        Code has been optimized for a 32-bit processor such as the Cortex
*        M0 or M3.
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
#include "cytypes.h"
#include "LCD.h"
#include "LCD_SPIM_UDB.h"
#include "LCD_Reset.h"
#include "cyfitter.h"

#if (LCD_SPIM_TYPE == LCD_SPIM_SCB_TYPE)
#include "LCD_SPIM_UDB_SPI_UART.h"
#endif

uint8 LCD_driver;
				
static int32 LCD_x_offset = 0;
static int32 LCD_y_offset = 0;

/*******************************************************************************
* Function Name: LCD_Command
********************************************************************************
*
* Summary:
*  This function sends a command to the LCD controller.  
*
* Parameters:  
*  cmdData:  This is the command to be sent to the LCD controller  
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_Command(uint32 cmdData)
{
#if (LCD_SPIM_TYPE == LCD_SPIM_UDB_TYPE)
  while((LCD_SPIM_UDB_ReadTxStatus() & (LCD_SPIM_UDB_STS_SPI_DONE | LCD_SPIM_UDB_STS_SPI_IDLE) )== 0);
  LCD_SPIM_UDB_WriteTxData((uint16)cmdData);
#endif

#if (LCD_SPIM_TYPE == LCD_SPIM_SCB_TYPE)
    LCD_SPIM_UDB_SpiUartWriteTxData(cmdData);
#endif
}

/*******************************************************************************
* Function Name: LCD_Data
********************************************************************************
*
* Summary:
*  This function sends raw data to the controller.  The 9th bit is set to signal
*  the controller that it information is data and not a command.
*
* Parameters:  
*  rawData:  The data to be sent to the controller. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_Data(uint32 rawData)
{	
#if (LCD_SPIM_TYPE == LCD_SPIM_UDB_TYPE)
    uint16 data16;
    data16 = (uint16)rawData | 0x0100;
	
    while((LCD_SPIM_UDB_ReadTxStatus() & (LCD_SPIM_UDB_STS_SPI_DONE | LCD_SPIM_UDB_STS_SPI_IDLE) )== 0);
    LCD_SPIM_UDB_WriteTxData(data16);
#endif

#if (LCD_SPIM_TYPE == LCD_SPIM_SCB_TYPE)

    rawData = rawData | 0x00000100;
    LCD_SPIM_UDB_SpiUartWriteTxData(rawData);
#endif

}

/*******************************************************************************
* Function Name: LCD_Start
********************************************************************************
*
* Summary:
*  This function initializes the SPI interface and the LCD controller.  
*
* Parameters:  
*  None:   
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_Start(void)
{
	
	LCD_SPIM_UDB_Start();
#if (LCD_SPIM_TYPE == LCD_SPIM_SCB_TYPE)
    LCD_SPIM_UDB_SpiSetActiveSlaveSelect(3u);
#endif

	CyDelayUs(20);			            /* 20us delay    */
	LCD_Reset_Write(0x00);	/* Display Reset */
	CyDelay(200);			            /* 200ms delay   */
	LCD_Reset_Write(0xFF);	/* Release Reset */
	CyDelay(200);			            /* 200ms delay   */
	
#if(LCD_Controller == LCD_EPSON)
	LCD_Command(LCD_DISCTL);	/*  Display control (0xCA)         */
	LCD_Data(0x0C);		/* 12 = 1100 - CL dividing ratio [don't divide] switching period 8H (default) */
	LCD_Data(0x20);		/* nlines/4 - 1 = 132/4 - 1 = 32 duty              */
	LCD_Data(0x00);		/* No inversely highlighted lines                  */
	
	LCD_Command(LCD_COMSCN);	  /* common scanning direction     */
	LCD_Data(0x01);		                  /* 1->68, 132<-69 scan direction */
	
	LCD_Command(LCD_OSCON);	  /* internal oscialltor ON        */
	LCD_Command(LCD_SLEEPOUT);  /* sleep out                     */    
	
	LCD_Command(LCD_PWRCTR);	  /* Power ctrl                    */
	LCD_Data(0x0F);	                      /* everything on, no external reference  resistors  */
	
	LCD_Command(LCD_DINVON);	  /* invert display mode           */
	
	LCD_Command(LCD_DATCTL);	  /* data control                  */
	LCD_Data(0x03);		/* Inverse page address, reverse rotation column address, column scan-direction	*/
	LCD_Data(0x00);		                  /* normal RGB arrangement                 */
	LCD_Data(0x02);		                  /* 16-bit Grayscale Type A (12-bit color) */
	
	LCD_Command(LCD_SETCONST);	/* Electronic volume, this is the contrast/brightness        */
	LCD_Data(LCD_CONST_DEFAULT);  /* Volume (contrast) setting - fine tuning, original (0-63)  */
	LCD_Data(3);			                    /* Internal resistor ratio - coarse adjustment (0-7)         */
	
	LCD_Command(LCD_NOP);	        /* nop */

	CyDelay(100);

	LCD_Command(LCD_DISPON);	     /*  Display on */
#endif

#if(LCD_Controller == LCD_PHILIPS) /* Driver is Philips */

	LCD_Command(LCD_SLEEPOUT);	 /* Sleep Out          */
	LCD_Command(LCD_BSTRON);       /* Booster voltage on */
	LCD_Command(LCD_DISPON);		 /* Display on         */
	
	/* 12-bit color pixel format:  */
	LCD_Command(LCD_COLMOD);		 /* Color interface format     */
	LCD_Data(0x03);			                 /* 0b011 is 12-bit/pixel mode */
	
	LCD_Command(LCD_MADCTL);		 /* Memory Access Control      */
	LCD_Command(LCD_SETCONST);     /* Set Contrast               */          
	LCD_Data(LCD_CONST_DEFAULT);
	
	LCD_Command(LCD_NOP);	
#endif

  	LCD_Contrast(60);                           /* Set default contrast  */
  	LCD_Clear(LCD_BLACK);          /* Clear screen to black */
}

/*******************************************************************************
* Function Name: LCD_Stop
********************************************************************************
*
* Summary:
*  This function clears the LCD to black and turns off the display.  
*
* Parameters:  
*   None:   
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_Stop(void)
{
   LCD_Clear(LCD_BLACK);
   LCD_Command(LCD_DISPOFF);
}

/*******************************************************************************
* Function Name: LCD_Clear
********************************************************************************
*
* Summary:
*  Clear the display.  
*
* Parameters:  
*  color: Color to clear display with. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_Clear(int32 color)
{
    int32 i;

	LCD_Command(LCD_PAGEADDR);
	LCD_Data(LCD_MINX);
	LCD_Data(LCD_MAXX);

	LCD_Command(LCD_COLADDR);
	LCD_Data(LCD_MINY);
	LCD_Data(LCD_MAXY);

	LCD_Command(LCD_RAMWR);


	for( i=0; i < (LCD_MAXX*LCD_MAXY)/2; i++)
	{
		LCD_Data((color>>4)&0x00FF);
		LCD_Data(((color&0x0F)<<4)|(color>>8));
		LCD_Data(color&0x0FF);
	}

	LCD_x_offset = 0;
	LCD_y_offset = 0;
}


/*******************************************************************************
* Function Name: LCD_Contrast
********************************************************************************
*
* Summary:
*  Set color contrast  
*
* Parameters:  
*  setting: Contrast value.  
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_Contrast(int32 setting)
{
    setting &= LCD_CONST_MASK;	                /* Mask off contrast value */
    LCD_Command(LCD_SETCONST);	/* Set Contrast            */
    LCD_Data(setting);	                    

#if(LCD_Controller == LCD_EPSON)  /* Only required for Epson */
    LCD_Data(3);			
#endif

}

/*******************************************************************************
* Function Name: LCD_SwapColors
********************************************************************************
*
* Summary:
*   Swaps colors between Epson and Philips mode.  
*
* Parameters:  
*  prtValue:  The value  
*
* Return: 
*  None 
*  Added by Steve Sparks @ Big Nerd Ranch.
*  This swaps the Epson RGB order into the Philips RGB order. (Or, vice versa, I suppose.)
*******************************************************************************/
int32 LCD_SwapColors(int32 in)
{
    return ((in & 0x000F)<<8)|(in & 0x00F0)|((in & 0x0F00)>>8);
}
/*******************************************************************************
* Function Name: LCD_Pixel
********************************************************************************
*
* Summary:
*  Draw Pixel  
*
* Parameters:  
*  x,y:    Location to draw the pixel
*  color:  Color of the pixel
*
* Return: 
*  None 
*******************************************************************************/
void LCD_Pixel(int32 x, int32 y, int32 color)
{
    y =	(LCD_COL_HEIGHT - 1) - y;
    x = (LCD_ROW_LENGTH - 1) - x;

#if (LCD_Controller == LCD_EPSON) /* EPSON     */
	LCD_Command(LCD_PAGEADDR);    /* Set page  */
	LCD_Data(x);
	LCD_Data(LCD_ENDPAGE);

	LCD_Command(LCD_COLADDR);     /* Set column */
	LCD_Data(y);
	LCD_Data(LCD_ENDCOL);

	LCD_Command(LCD_RAMWR);       /* Write color data */
	LCD_Data((color>>4)&0x00FF);
	LCD_Data(((color&0x0F)<<4)|(color>>8));
	LCD_Data(color&0x0FF);
#endif

#if (LCD_Controller == LCD_PHILIPS) /* PHILIPS   */
	LCD_Command(LCD_PAGEADDR);      /* Set page  */
	LCD_Data((uint8)x);
	LCD_Data((uint8)x);

	LCD_Command(LCD_COLADDR);       /* Set column */
	LCD_Data((uint8)y);
	LCD_Data((uint8)y);

	LCD_Command(LCD_RAMWR);         /* Write color data */
	LCD_Data((uint8)((color>>4)&0x00FF));
	LCD_Data((uint8)(((color&0x0F)<<4)|0x00));
#endif
}

/*******************************************************************************
* Function Name: LCD_DrawCircle
********************************************************************************
*
* Summary:
*  Draw a circle on the display given a start point and radius.  
*
*  This code uses Bresenham's Circle Algorithm. 
*
* Parameters:  
*  x0, y0: Center of circle
*  radius: Radius of circle
*  color:  Color of circle
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_DrawCircle (int32 x0, int32 y0, int32 radius, int32 color)
{
	int32 f = 1 - radius;
	int32 ddF_x = 0;
	int32 ddF_y = -2 * radius;
	int32 x = 0;
	int32 y = radius;

	LCD_Pixel(x0, y0 + radius, color);
	LCD_Pixel(x0, y0 - radius, color);
	LCD_Pixel( x0 + radius, y0, color);
	LCD_Pixel( x0 - radius, y0, color);

	while(x < y)
	{
		if(f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		LCD_Pixel(x0 + x, y0 + y, color);
		LCD_Pixel(x0 - x, y0 + y, color);
		LCD_Pixel( x0 + x, y0 - y, color);
		LCD_Pixel( x0 - x, y0 - y, color);
		LCD_Pixel( x0 + y, y0 + x, color);
		LCD_Pixel( x0 - y, y0 + x, color);
		LCD_Pixel( x0 + y, y0 - x, color);
		LCD_Pixel( x0 - y, y0 - x, color);
	}
}

/*******************************************************************************
* Function Name: LCD_PrintChar
********************************************************************************
*
* Summary:
*  Print a character on the display.  
*
* Parameters:  
*  c:       ASCII character to be printed
*  x,y:     Location where character will be printed.
*  fColor:  Foreground color
*  bColor:  Background color
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_PrintChar(uint8 c, int32 x, int32 y, int32 fColor, int32 bColor)
{
	y	=	(LCD_COL_HEIGHT - 1) - y; /* make display "right" side up */
	x	=	(LCD_ROW_LENGTH - 2) - x;

	int32             i,j;
	int32    nCols;
	int32    nRows;
	int32    nBytes;
	uint8    PixelRow;
	uint8    Mask;
	int32    Word0;
	int32    Word1;
	uint8   *pFont;
	uint8   *pChar;

	/* Get pointer to the beginning of the selected font table  */
	pFont = (uint8 *)LCD_FONT8x16;
	
	/* Get the nColumns, nRows and nBytes.                      */
	nCols = *pFont;
	nRows = *(pFont + 1);
	nBytes = *(pFont + 2);
	
	/* Get pointer to the last byte of the desired character    */
	pChar = pFont + (nBytes * (c - 0x1F)) + nBytes - 1;

#if (LCD_Controller == LCD_EPSON)
	/* Row address set */
	LCD_Command(LCD_PAGEADDR);
	LCD_Data(x);
	LCD_Data((x + nRows - 1));
	/* Column address set */
	LCD_Command(LCD_COLADDR);
	LCD_Data(y);
	LCD_Data((y + nCols - 1));

	/* Write Memory */
	LCD_Command(LCD_RAMWR);
	/* Loop on each row, working backwards from the bottom to the top  */
	for (i = nRows - 1; i >= 0; i--) {
		/* Copy pixel row from font table and then decrement row       */
		PixelRow = *pChar++;
		/* Loop on each pixel in the row (left to right)               */
		/* Note: we do two pixels each loop                            */
		Mask = 0x80;
		for (j = 0; j < nCols; j += 2) 
		{
			/* If pixel bit set, use foreground color; else use the background color. */
			/* Now get the pixel color for two successive pixels                      */
			if ((PixelRow & Mask) == 0)
			{
				Word0 = bColor;
			}
			else
			{
				Word0 = fColor;
			}
			
			Mask = Mask >> 1;
			if ((PixelRow & Mask) == 0)
			{
				Word1 = bColor;
			}
			else
			{
				Word1 = fColor;
			}
			Mask = Mask >> 1;
			/* Use this information to output three data bytes  */
			LCD_Data((uint8)((Word0 >> 4) & 0xFF));
			LCD_Data((uint8)(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF)));
			LCD_Data((uint8)(Word1 & 0xFF));
		}
	}
#endif

#if (LCD_Controller == LCD_PHILIPS)

	/* Set row address     */
	LCD_Command(LCD_PAGEADDR);
	LCD_Data((uint8)x);
	LCD_Data((uint8)(x + nRows - 1));
    /* Set column address  */
	LCD_Command(LCD_COLADDR);
	LCD_Data((uint8)y);
	LCD_Data((uint8)(y + nCols - 1));

	/* Write to display memory */
	LCD_Command(LCD_RAMWR);
	/* loop on each row, working backwards from the bottom to the top.  */
	pChar+=nBytes-1;  /* Stick pChar at the end of the row, need to reverse print on phillips */
	for (i = nRows - 1; i >= 0; i--) {
		/* copy pixel row from font table and then decrement row.  */
		PixelRow = *pChar--;
		/* Loop on each pixel in the row (left to right)  */
		/* Note: we do two pixels each loop               */
		Mask = 0x01;  
		for (j = 0; j < nCols; j += 2) 
		{
			/* If pixel bit set, use foreground color; else use the background color  */
			/* Get the pixel color for two successive pixels                          */
			if ((PixelRow & Mask) == 0)
				Word0 = bColor;
			else
				Word0 = fColor;
			Mask = Mask << 1; 
			if ((PixelRow & Mask) == 0)
				Word1 = bColor;
			else
				Word1 = fColor;
			Mask = Mask << 1; 
			/* use this information to output three data bytes */
			LCD_Data((Word0 >> 4) & 0xFF);
			LCD_Data(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
			LCD_Data(Word1 & 0xFF);
		}
	}
#endif
}

/*******************************************************************************
* Function Name: LCD_PrintString
********************************************************************************
*
* Summary:
*  Print a string horizontally on the display.  
*
* Parameters:  
*  pString:  Pointer to null terminated string to be displayed.
*  x,y:      Location where string is to be printed
*  fColor:   Forground Color
*  bColor:   Background Color
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_PrintString(char *pString, int32 x, int32 y, int32 fColor, int32 bColor)
{
	x = x + 16;
	y = y + 8;
    int32 originalY = y;

	/* Loop until null-terminator is found */
	while (*pString != 0x00) 
	{
		/* Draw the character        */
		LCD_PrintChar(*pString++, x, y, fColor, bColor);
		/* Advance the y position    */
		y = y + 8;
		/* Bail out if y exceeds 131 */
		if (y > 131) 
		{
            x = x + 16;
            y = originalY;
        }
        if (x > 131) break;
	}
}

/*******************************************************************************
* Function Name: LCD_DrawLine
********************************************************************************
*
* Summary:
*  Draw a line on the display.  
*
* Parameters:  
*  x0, y0:  The beginning endpoint
*  x1, y1:  The end endpoint.
*  color:   Color of the line.
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, int32 color)
{
	int32 dy = y1 - y0; /* Difference between y0 and y1 */
	int32 dx = x1 - x0; /* Difference between x0 and x1 */
	int32 stepx, stepy;

	if (dy < 0)
	{
		dy = -dy;
		stepy = -1;
	}
	else
	{
		stepy = 1;
	}

	if (dx < 0)
	{
		dx = -dx;
		stepx = -1;
	}
	else
	{
		stepx = 1;
	}

	dy <<= 1; /* dy is now 2*dy  */
	dx <<= 1; /* dx is now 2*dx  */
	LCD_Pixel(x0, y0, color);

	if (dx > dy) 
	{
		int fraction = dy - (dx >> 1);
		while (x0 != x1)
		{
			if (fraction >= 0)
			{
				y0 += stepy;
				fraction -= dx;
			}
			x0 += stepx;
			fraction += dy;
			LCD_Pixel(x0, y0, color);
		}
	}
	else
	{
		int fraction = dx - (dy >> 1);
		while (y0 != y1)
		{
			if (fraction >= 0)
			{
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;
			LCD_Pixel( x0, y0, color);
		}
	}
}

/*******************************************************************************
* Function Name: LCD_DrawRect
********************************************************************************
*
* Summary:
*  Draw a rectangle, filled or not.  
*
* Parameters:  
*  x0, y0:  The upper lefthand corner.
*  x1, y1:  The lower right corner.
*  fill:    Non-Zero if retangle is to be filled.
*  color:   Color for rectangle, border and fill.
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, int32 fill, int32 color)
{	
     int xDiff;
	/* Check if the rectangle is to be filled    */
	if (fill != 0)
	{	
        /* Find the difference between the x vars */
		if(x0 > x1)
		{
			xDiff = x0 - x1; 
		}
		else
		{
			xDiff = x1 - x0;
		}
	
	    /* Fill it with lines  */
		while(xDiff >= 0)
		{
			LCD_DrawLine(x0, y0, x0, y1, color);
		
			if(x0 > x1)
				x0--;
			else
				x0++;
		
			xDiff--;
		}

	}
	else 
	{
		/* Draw the four sides of the rectangle */
		LCD_DrawLine(x0, y0, x1, y0, color);
		LCD_DrawLine(x0, y1, x1, y1, color);
		LCD_DrawLine(x0, y0, x0, y1, color);
		LCD_DrawLine(x1, y0, x1, y1, color);
	}
}


/*******************************************************************************
* Function Name: LCD_Off
********************************************************************************
*
* Summary:
*  Turn off display
*
* Parameters:  
*  None 
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_Off(void)		/* Turn off the display  */
{
    LCD_Command(LCD_DISPOFF);
}

/*******************************************************************************
* Function Name: LCD_On
********************************************************************************
*
* Summary:
*  Turn on display
*
* Parameters:  
*  None 
*
* Return: 
*  None 
*  
*******************************************************************************/
void LCD_On(void)		/* Turn on the display */
{
    LCD_Command(LCD_DISPON);
}



/* [] END OF FILE */
