/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c  
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
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
#include "`$INSTANCE_NAME`.h"
#include "`$INSTANCE_NAME`_`$SPIM_Name`.h"
#include "`$INSTANCE_NAME`_Reset.h"
#include "cyfitter.h"

#if (`$INSTANCE_NAME`_SPIM_TYPE == `$INSTANCE_NAME`_SPIM_SCB_TYPE)
#include "`$INSTANCE_NAME`_`$SPIM_Name`_SPI_UART.h"
#endif

uint8 `$INSTANCE_NAME`_driver;
				
static int32 `$INSTANCE_NAME`_x_offset = 0;
static int32 `$INSTANCE_NAME`_y_offset = 0;

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Command
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
void `$INSTANCE_NAME`_Command(uint32 cmdData)
{
#if (`$INSTANCE_NAME`_SPIM_TYPE == `$INSTANCE_NAME`_SPIM_UDB_TYPE)
  while((`$INSTANCE_NAME`_`$SPIM_Name`_ReadTxStatus() & (`$INSTANCE_NAME`_`$SPIM_Name`_STS_SPI_DONE | `$INSTANCE_NAME`_`$SPIM_Name`_STS_SPI_IDLE) )== 0);
  `$INSTANCE_NAME`_`$SPIM_Name`_WriteTxData((uint16)cmdData);
#endif

#if (`$INSTANCE_NAME`_SPIM_TYPE == `$INSTANCE_NAME`_SPIM_SCB_TYPE)
    `$INSTANCE_NAME`_`$SPIM_Name`_SpiUartWriteTxData(cmdData);
#endif
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Data
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
void `$INSTANCE_NAME`_Data(uint32 rawData)
{	
#if (`$INSTANCE_NAME`_SPIM_TYPE == `$INSTANCE_NAME`_SPIM_UDB_TYPE)
    uint16 data16;
    data16 = (uint16)rawData | 0x0100;
	
    while((`$INSTANCE_NAME`_`$SPIM_Name`_ReadTxStatus() & (`$INSTANCE_NAME`_`$SPIM_Name`_STS_SPI_DONE | `$INSTANCE_NAME`_`$SPIM_Name`_STS_SPI_IDLE) )== 0);
    `$INSTANCE_NAME`_`$SPIM_Name`_WriteTxData(data16);
#endif

#if (`$INSTANCE_NAME`_SPIM_TYPE == `$INSTANCE_NAME`_SPIM_SCB_TYPE)

    rawData = rawData | 0x00000100;
    `$INSTANCE_NAME`_`$SPIM_Name`_SpiUartWriteTxData(rawData);
#endif

}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
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
void `$INSTANCE_NAME`_Start(void)
{
	
	`$INSTANCE_NAME`_`$SPIM_Name`_Start();
#if (`$INSTANCE_NAME`_SPIM_TYPE == `$INSTANCE_NAME`_SPIM_SCB_TYPE)
    `$INSTANCE_NAME`_`$SPIM_Name`_SpiSetActiveSlaveSelect(3u);
#endif

	CyDelayUs(20);			            /* 20us delay    */
	`$INSTANCE_NAME`_Reset_Write(0x00);	/* Display Reset */
	CyDelay(200);			            /* 200ms delay   */
	`$INSTANCE_NAME`_Reset_Write(0xFF);	/* Release Reset */
	CyDelay(200);			            /* 200ms delay   */
	
#if(`$INSTANCE_NAME`_Controller == `$INSTANCE_NAME`_EPSON)
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_DISCTL);	/*  Display control (0xCA)         */
	`$INSTANCE_NAME`_Data(0x0C);		/* 12 = 1100 - CL dividing ratio [don't divide] switching period 8H (default) */
	`$INSTANCE_NAME`_Data(0x20);		/* nlines/4 - 1 = 132/4 - 1 = 32 duty              */
	`$INSTANCE_NAME`_Data(0x00);		/* No inversely highlighted lines                  */
	
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_COMSCN);	  /* common scanning direction     */
	`$INSTANCE_NAME`_Data(0x01);		                  /* 1->68, 132<-69 scan direction */
	
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_OSCON);	  /* internal oscialltor ON        */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_SLEEPOUT);  /* sleep out                     */    
	
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_PWRCTR);	  /* Power ctrl                    */
	`$INSTANCE_NAME`_Data(0x0F);	                      /* everything on, no external reference  resistors  */
	
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_DINVON);	  /* invert display mode           */
	
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_DATCTL);	  /* data control                  */
	`$INSTANCE_NAME`_Data(0x03);		/* Inverse page address, reverse rotation column address, column scan-direction	*/
	`$INSTANCE_NAME`_Data(0x00);		                  /* normal RGB arrangement                 */
	`$INSTANCE_NAME`_Data(0x02);		                  /* 16-bit Grayscale Type A (12-bit color) */
	
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_SETCONST);	/* Electronic volume, this is the contrast/brightness        */
	`$INSTANCE_NAME`_Data(`$INSTANCE_NAME`_CONST_DEFAULT);  /* Volume (contrast) setting - fine tuning, original (0-63)  */
	`$INSTANCE_NAME`_Data(3);			                    /* Internal resistor ratio - coarse adjustment (0-7)         */
	
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_NOP);	        /* nop */

	CyDelay(100);

	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_DISPON);	     /*  Display on */
#endif

#if(`$INSTANCE_NAME`_Controller == `$INSTANCE_NAME`_PHILIPS) /* Driver is Philips */

	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_SLEEPOUT);	 /* Sleep Out          */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_BSTRON);       /* Booster voltage on */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_DISPON);		 /* Display on         */
	
	/* 12-bit color pixel format:  */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_COLMOD);		 /* Color interface format     */
	`$INSTANCE_NAME`_Data(0x03);			                 /* 0b011 is 12-bit/pixel mode */
	
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_MADCTL);		 /* Memory Access Control      */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_SETCONST);     /* Set Contrast               */          
	`$INSTANCE_NAME`_Data(`$INSTANCE_NAME`_CONST_DEFAULT);
	
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_NOP);	
#endif

  	`$INSTANCE_NAME`_Contrast(60);                           /* Set default contrast  */
  	`$INSTANCE_NAME`_Clear(`$INSTANCE_NAME`_BLACK);          /* Clear screen to black */
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Stop
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
void `$INSTANCE_NAME`_Stop(void)
{
   `$INSTANCE_NAME`_Clear(`$INSTANCE_NAME`_BLACK);
   `$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_DISPOFF);
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Clear
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
void `$INSTANCE_NAME`_Clear(int32 color)
{
    int32 i;

	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_PAGEADDR);
	`$INSTANCE_NAME`_Data(`$INSTANCE_NAME`_MINX);
	`$INSTANCE_NAME`_Data(`$INSTANCE_NAME`_MAXX);

	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_COLADDR);
	`$INSTANCE_NAME`_Data(`$INSTANCE_NAME`_MINY);
	`$INSTANCE_NAME`_Data(`$INSTANCE_NAME`_MAXY);

	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_RAMWR);


	for( i=0; i < (`$INSTANCE_NAME`_MAXX*`$INSTANCE_NAME`_MAXY)/2; i++)
	{
		`$INSTANCE_NAME`_Data((color>>4)&0x00FF);
		`$INSTANCE_NAME`_Data(((color&0x0F)<<4)|(color>>8));
		`$INSTANCE_NAME`_Data(color&0x0FF);
	}

	`$INSTANCE_NAME`_x_offset = 0;
	`$INSTANCE_NAME`_y_offset = 0;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Contrast
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
void `$INSTANCE_NAME`_Contrast(int32 setting)
{
    setting &= `$INSTANCE_NAME`_CONST_MASK;	                /* Mask off contrast value */
    `$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_SETCONST);	/* Set Contrast            */
    `$INSTANCE_NAME`_Data(setting);	                    

#if(`$INSTANCE_NAME`_Controller == `$INSTANCE_NAME`_EPSON)  /* Only required for Epson */
    `$INSTANCE_NAME`_Data(3);			
#endif

}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SwapColors
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
int32 `$INSTANCE_NAME`_SwapColors(int32 in)
{
    return ((in & 0x000F)<<8)|(in & 0x00F0)|((in & 0x0F00)>>8);
}
/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Pixel
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
void `$INSTANCE_NAME`_Pixel(int32 x, int32 y, int32 color)
{
    y =	(`$INSTANCE_NAME`_COL_HEIGHT - 1) - y;
    x = (`$INSTANCE_NAME`_ROW_LENGTH - 1) - x;

#if (`$INSTANCE_NAME`_Controller == `$INSTANCE_NAME`_EPSON) /* EPSON     */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_PAGEADDR);    /* Set page  */
	`$INSTANCE_NAME`_Data(x);
	`$INSTANCE_NAME`_Data(`$INSTANCE_NAME`_ENDPAGE);

	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_COLADDR);     /* Set column */
	`$INSTANCE_NAME`_Data(y);
	`$INSTANCE_NAME`_Data(`$INSTANCE_NAME`_ENDCOL);

	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_RAMWR);       /* Write color data */
	`$INSTANCE_NAME`_Data((color>>4)&0x00FF);
	`$INSTANCE_NAME`_Data(((color&0x0F)<<4)|(color>>8));
	`$INSTANCE_NAME`_Data(color&0x0FF);
#endif

#if (`$INSTANCE_NAME`_Controller == `$INSTANCE_NAME`_PHILIPS) /* PHILIPS   */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_PAGEADDR);      /* Set page  */
	`$INSTANCE_NAME`_Data((uint8)x);
	`$INSTANCE_NAME`_Data((uint8)x);

	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_COLADDR);       /* Set column */
	`$INSTANCE_NAME`_Data((uint8)y);
	`$INSTANCE_NAME`_Data((uint8)y);

	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_RAMWR);         /* Write color data */
	`$INSTANCE_NAME`_Data((uint8)((color>>4)&0x00FF));
	`$INSTANCE_NAME`_Data((uint8)(((color&0x0F)<<4)|0x00));
#endif
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DrawCircle
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
void `$INSTANCE_NAME`_DrawCircle (int32 x0, int32 y0, int32 radius, int32 color)
{
	int32 f = 1 - radius;
	int32 ddF_x = 0;
	int32 ddF_y = -2 * radius;
	int32 x = 0;
	int32 y = radius;

	`$INSTANCE_NAME`_Pixel(x0, y0 + radius, color);
	`$INSTANCE_NAME`_Pixel(x0, y0 - radius, color);
	`$INSTANCE_NAME`_Pixel( x0 + radius, y0, color);
	`$INSTANCE_NAME`_Pixel( x0 - radius, y0, color);

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

		`$INSTANCE_NAME`_Pixel(x0 + x, y0 + y, color);
		`$INSTANCE_NAME`_Pixel(x0 - x, y0 + y, color);
		`$INSTANCE_NAME`_Pixel( x0 + x, y0 - y, color);
		`$INSTANCE_NAME`_Pixel( x0 - x, y0 - y, color);
		`$INSTANCE_NAME`_Pixel( x0 + y, y0 + x, color);
		`$INSTANCE_NAME`_Pixel( x0 - y, y0 + x, color);
		`$INSTANCE_NAME`_Pixel( x0 + y, y0 - x, color);
		`$INSTANCE_NAME`_Pixel( x0 - y, y0 - x, color);
	}
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_PrintChar
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
void `$INSTANCE_NAME`_PrintChar(uint8 c, int32 x, int32 y, int32 fColor, int32 bColor)
{
	y	=	(`$INSTANCE_NAME`_COL_HEIGHT - 1) - y; /* make display "right" side up */
	x	=	(`$INSTANCE_NAME`_ROW_LENGTH - 2) - x;

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
	pFont = (uint8 *)`$INSTANCE_NAME`_FONT8x16;
	
	/* Get the nColumns, nRows and nBytes.                      */
	nCols = *pFont;
	nRows = *(pFont + 1);
	nBytes = *(pFont + 2);
	
	/* Get pointer to the last byte of the desired character    */
	pChar = pFont + (nBytes * (c - 0x1F)) + nBytes - 1;

#if (`$INSTANCE_NAME`_Controller == `$INSTANCE_NAME`_EPSON)
	/* Row address set */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_PAGEADDR);
	`$INSTANCE_NAME`_Data(x);
	`$INSTANCE_NAME`_Data((x + nRows - 1));
	/* Column address set */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_COLADDR);
	`$INSTANCE_NAME`_Data(y);
	`$INSTANCE_NAME`_Data((y + nCols - 1));

	/* Write Memory */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_RAMWR);
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
			`$INSTANCE_NAME`_Data((uint8)((Word0 >> 4) & 0xFF));
			`$INSTANCE_NAME`_Data((uint8)(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF)));
			`$INSTANCE_NAME`_Data((uint8)(Word1 & 0xFF));
		}
	}
#endif

#if (`$INSTANCE_NAME`_Controller == `$INSTANCE_NAME`_PHILIPS)

	/* Set row address     */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_PAGEADDR);
	`$INSTANCE_NAME`_Data((uint8)x);
	`$INSTANCE_NAME`_Data((uint8)(x + nRows - 1));
    /* Set column address  */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_COLADDR);
	`$INSTANCE_NAME`_Data((uint8)y);
	`$INSTANCE_NAME`_Data((uint8)(y + nCols - 1));

	/* Write to display memory */
	`$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_RAMWR);
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
			`$INSTANCE_NAME`_Data((Word0 >> 4) & 0xFF);
			`$INSTANCE_NAME`_Data(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF));
			`$INSTANCE_NAME`_Data(Word1 & 0xFF);
		}
	}
#endif
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_PrintString
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
void `$INSTANCE_NAME`_PrintString(char *pString, int32 x, int32 y, int32 fColor, int32 bColor)
{
	x = x + 16;
	y = y + 8;
    int32 originalY = y;

	/* Loop until null-terminator is found */
	while (*pString != 0x00) 
	{
		/* Draw the character        */
		`$INSTANCE_NAME`_PrintChar(*pString++, x, y, fColor, bColor);
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
* Function Name: `$INSTANCE_NAME`_DrawLine
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
void `$INSTANCE_NAME`_DrawLine(int32 x0, int32 y0, int32 x1, int32 y1, int32 color)
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
	`$INSTANCE_NAME`_Pixel(x0, y0, color);

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
			`$INSTANCE_NAME`_Pixel(x0, y0, color);
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
			`$INSTANCE_NAME`_Pixel( x0, y0, color);
		}
	}
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DrawRect
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
void `$INSTANCE_NAME`_DrawRect(int32 x0, int32 y0, int32 x1, int32 y1, int32 fill, int32 color)
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
			`$INSTANCE_NAME`_DrawLine(x0, y0, x0, y1, color);
		
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
		`$INSTANCE_NAME`_DrawLine(x0, y0, x1, y0, color);
		`$INSTANCE_NAME`_DrawLine(x0, y1, x1, y1, color);
		`$INSTANCE_NAME`_DrawLine(x0, y0, x0, y1, color);
		`$INSTANCE_NAME`_DrawLine(x1, y0, x1, y1, color);
	}
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Off
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
void `$INSTANCE_NAME`_Off(void)		/* Turn off the display  */
{
    `$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_DISPOFF);
}

/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_On
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
void `$INSTANCE_NAME`_On(void)		/* Turn on the display */
{
    `$INSTANCE_NAME`_Command(`$INSTANCE_NAME`_DISPON);
}



/* [] END OF FILE */
