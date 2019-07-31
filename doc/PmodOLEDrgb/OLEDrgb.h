/************************************************************************/
/*																		*/
/*	OLEDrgb.h	--	Interface Declarations for IOShieldOled.cpp		*/
/*																		*/
/************************************************************************/
/*	Author:		Cristian Fatu											*/
/*	Copyright 2015, Digilent Inc.										*/
/************************************************************************/
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*	This header file contains the object class declarations and other	*/
/*	interface declarations need to use the OLED graphics display driver	*/
/*	for the Digilent Basic I/O Shield.									*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	07/20/2015(CristianF): created										*/
/*																		*/
/************************************************************************/

#if !defined(OLED_H)
#define OLED_H

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#include <inttypes.h>
#include <DSPI.h>

#define BYTE uint8_t

#define OLEDRGB_WIDTH                      96
#define OLEDRGB_HEIGHT                     64

#define CMD_DRAWLINE                       0x21
#define CMD_DRAWRECTANGLE                  0x22
#define CMD_COPYWINDOW                     0x23
#define CMD_DIMWINDOW                      0x24
#define CMD_CLEARWINDOW                    0x25
#define CMD_FILLWINDOW                     0x26
    #define DISABLE_FILL    0x00
    #define ENABLE_FILL     0x01
#define CMD_CONTINUOUSSCROLLINGSETUP      0x27
#define CMD_DEACTIVESCROLLING              0x2E
#define CMD_ACTIVESCROLLING                0x2F

#define CMD_SETCOLUMNADDRESS              	0x15
#define CMD_SETROWADDRESS                 	0x75
#define CMD_SETCONTRASTA                  	0x81
#define CMD_SETCONTRASTB                  	0x82
#define CMD_SETCONTRASTC                  	0x83
#define CMD_MASTERCURRENTCONTROL          	0x87
#define CMD_SETPRECHARGESPEEDA           	0x8A
#define CMD_SETPRECHARGESPEEDB           	0x8B
#define CMD_SETPRECHARGESPEEDC           	0x8C
#define CMD_SETREMAP                       	0xA0
#define CMD_SETDISPLAYSTARTLINE          	0xA1
#define CMD_SETDISPLAYOFFSET              	0xA2
#define CMD_NORMALDISPLAY                  	0xA4
#define CMD_ENTIREDISPLAYON              	0xA5
#define CMD_ENTIREDISPLAYOFF              	0xA6
#define CMD_INVERSEDISPLAY                 	0xA7
#define CMD_SETMULTIPLEXRATIO             	0xA8
#define CMD_DIMMODESETTING                	0xAB
#define CMD_SETMASTERCONFIGURE            	0xAD
#define CMD_DIMMODEDISPLAYON             	0xAC
#define CMD_DISPLAYOFF                     	0xAE
#define CMD_DISPLAYON    					0xAF
#define CMD_POWERSAVEMODE                 	0xB0
#define CMD_PHASEPERIODADJUSTMENT         	0xB1
#define CMD_DISPLAYCLOCKDIV               	0xB3
#define CMD_SETGRAySCALETABLE            	0xB8
#define CMD_ENABLELINEARGRAYSCALETABLE  	0xB9
#define CMD_SETPRECHARGEVOLTAGE           	0xBB
#define CMD_SETVVOLTAGE                   	0xBE



/* ------------------------------------------------------------ */
/*					Global Variable Declarations				*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class Oledrgb
{
 private: 
	DSPI *pdspi;
	uint8_t m_SSPin;
	uint8_t cmds[13];
	private:
	uint8_t WriteSPI(uint8_t bVal);
	uint8_t WriteSPI(uint8_t bVal1, uint8_t bVal2);	
	uint8_t WriteSPI(uint8_t *pCmd, int nCmd, uint8_t *pData, int nData = -1);
	
	void OledrgbDevInit();
	void OledrgbHostTerm();
	void OledrgbHostInit();
	void OledrgbDevInit1();
	void OledrgbDevInit2();
	
	//uint16_t m_rgwPixelMem[OLEDRGB_WIDTH * OLEDRGB_HEIGHT];
	 public:

	uint16_t BuildRGB(uint8_t R,uint8_t G,uint8_t B){return ((R>>3)<<11) | ((G>>2)<<5) | (B>>3);};
	uint8_t ExtractRFromRGB(uint16_t wRGB){return (uint8_t)((wRGB>>11)&0x1F);};
	uint8_t ExtractGFromRGB(uint16_t wRGB){return (uint8_t)((wRGB>>5)&0x3F);};	
	uint8_t ExtractBFromRGB(uint16_t wRGB){return (uint8_t)(wRGB&0x1F);};	
    Oledrgb();

	/* Basic device control functions.
	*/
    void begin(void);
	void end(void);
	

	
	void Clear();

	void DrawRectangle(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2, uint16_t lineColor, bool bFill, uint16_t fillColor);
	void DrawLine(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2, uint16_t lineColor);
	void DrawPixel(uint8_t x, uint8_t y, uint16_t pixelColor);
	void DrawBitmap(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2, uint8_t *pBmp);
	void DrawBitmap1(uint8_t x, uint8_t y, uint16_t noPixels, uint16_t pixelColor);
};

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
