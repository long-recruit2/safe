/************************************************************************/
/*																		*/
/*	IOShieldOled.c	--	OLED Display Driver for Basic I/O Shield		*/
/*																		*/
/************************************************************************/
/*	Author: 	Cristian Fatu											*/
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
/*  Module Description: 												*/
/*																		*/
/*	This module contains the implementation of the object class that	*/
/*	forms the chipKIT interface to the graphics driver functions for	*/
/*	the OLED display on the Digilent Basic I/O Shield.					*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	07/20/2015(CristianF): created										*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "OLEDrgb.h"


#include <inttypes.h>
#include <DSPI.h>
// Spi 0 Controller object instantiation
//DSPI0 dspi;

/* Setting pins based on DSPI_SS pin plus offset to get to lower 4 pins
** on pmod connector
*/
const int DataCmd	= PIN_DSPI0_SS + 4;
const int Reset		= PIN_DSPI0_SS + 5;
const int VCCEn		= PIN_DSPI0_SS + 6;
const int PmodEn	= PIN_DSPI0_SS + 7; 
/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Oledrgb Definitions					*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/***	void Oledrgb::Oledrgb()
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Default constructor.
*/

Oledrgb::Oledrgb()
{
	
}

/* ------------------------------------------------------------ */
/***	void Oledrgb::begin(void)
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Initialize the OLED display controller and turn the display on.
*/
void Oledrgb::begin(void)
{
	OledrgbHostInit();
	OledrgbDevInit();
}

/* ------------------------------------------------------------ */
/***	void Oledrgb::end(void)
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Shut down the OLED display. This turns the power off to the
**		OLED display, and releases all of the PIC32 resources used
**		by the OLED display driver.
*/

void Oledrgb::end()
{
	OledrgbHostTerm();
}


/* ------------------------------------------------------------ */
/***	OledrgbHostInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Perform PIC32 device initialization to prepare for use
**		of the OLEDrgb display.

*/
void Oledrgb::OledrgbHostInit()
	{
	pdspi = new DSPI0();
	m_SSPin = PIN_DSPI0_SS;	// default SS pin for SPI0

	pdspi->begin(m_SSPin);	// this defines SS pin as output, sets SS high
	pdspi->setMode(DSPI_MODE3);
	pdspi->setSpeed(25000000);
	
	pinMode(DataCmd, OUTPUT);
	pinMode(Reset, OUTPUT);
	pinMode(VCCEn, OUTPUT);
	pinMode(PmodEn, OUTPUT);
	
	
	digitalWrite(DataCmd, LOW);
	digitalWrite(Reset, HIGH);
	digitalWrite(VCCEn, LOW);
	digitalWrite(PmodEn, HIGH);
	
	pdspi->setSelect(HIGH);

}

/* ------------------------------------------------------------ */
/***	OledrgbHostTerm
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Release processor resources used by the library
*/

void Oledrgb::OledrgbHostTerm()
	{

	// Make the signal pins be inputs.

	digitalWrite(DataCmd, HIGH);
	digitalWrite(Reset, HIGH);

	pinMode(DataCmd, INPUT);
	pinMode(Reset, INPUT);

	// Make power control pins be inputs. 

	digitalWrite(VCCEn, LOW);
	digitalWrite(PmodEn, HIGH);

	pinMode(VCCEn, INPUT);
	pinMode(PmodEn, INPUT);

	pdspi->end();
}


#define DELAY_TIME_MS	1000
void Oledrgb::OledrgbDevInit()
	{	
		/* 
		Bring PmodEn HIGH
	*/

	digitalWrite(PmodEn, HIGH);
//	delay(10);
	delay(DELAY_TIME_MS);
	digitalWrite(Reset, LOW); //Set Reset LOW
//	delay(100);
	delay(DELAY_TIME_MS);
	
	digitalWrite(Reset, HIGH); //Set Reset HIGH	
	/* command un-lock
	*/
	WriteSPI(0xFD);
    WriteSPI(0x12); 




	/* 5. Univision Initialization Steps
	*/

	// 5a) Set Display Off
	WriteSPI(CMD_DISPLAYOFF);
	
    // 5b) Set Remap and Data Format
	WriteSPI(CMD_SETREMAP, 0x72);
	// 5c) Set Display Start Line
	WriteSPI(CMD_SETDISPLAYSTARTLINE, 0x00); //start line is set at upper left corner

	// 5d) Set Display Offset
	WriteSPI(CMD_SETDISPLAYOFFSET, 0x00); //no offset

    // 5e)
    WriteSPI(CMD_NORMALDISPLAY);

	// 5f) Set Multiplex Ratio
	WriteSPI(CMD_SETMULTIPLEXRATIO, 0x3F); //64MUX    

    // 5g)Set Master Configuration
    WriteSPI(CMD_SETMASTERCONFIGURE, 0x8E);

    // 5h)Set Power Saving Mode
    WriteSPI(CMD_POWERSAVEMODE, 0x0B);    

	// 5i) Set Phase Length
	WriteSPI(CMD_PHASEPERIODADJUSTMENT, 0x31); //phase 2 = 14 DCLKs, phase 1 = 15 DCLKS            


	// 5j) Send Clock Divide Ratio and Oscillator Frequency
	WriteSPI(CMD_DISPLAYCLOCKDIV, 0xF0); //mid high oscillator frequency, DCLK = FpbCllk/2

	// 5k) Set Second Pre-charge Speed of Color A
	WriteSPI(CMD_SETPRECHARGESPEEDA, 0x64); //Set Second Pre-change Speed For ColorA

	// 5l) Set Set Second Pre-charge Speed of Color B
	WriteSPI(CMD_SETPRECHARGESPEEDB, 0x78); //Set Second Pre-change Speed For ColorB

	// 5m) Set Second Pre-charge Speed of Color C
	WriteSPI(CMD_SETPRECHARGESPEEDC, 0x64); //Set Second Pre-change Speed For ColorC

	// 5n) Set Pre-Charge Voltage
	WriteSPI(CMD_SETPRECHARGEVOLTAGE, 0x3A); // Pre-charge voltage =...Vcc    

	// 50) Set VCOMH Deselect Level
	WriteSPI(CMD_SETVVOLTAGE, 0x3E); // Vcomh = ...*Vcc

	// 5p) Set Master Current
	WriteSPI(CMD_MASTERCURRENTCONTROL, 0x06); 

	// 5q) Set Contrast for Color A
	WriteSPI(CMD_SETCONTRASTA, 0x91); //Set contrast for color A

	// 5r) Set Contrast for Color B
	WriteSPI(CMD_SETCONTRASTB, 0x50); //Set contrast for color B

	// 5s) Set Contrast for Color C
	WriteSPI(CMD_SETCONTRASTC, 0x7D); //Set contrast for color C

	WriteSPI(CMD_DEACTIVESCROLLING);   //disable scrolling
    // 5t) Set display ON


	// 5u) Clear Screen
	Clear();

	/* Turn on VCC and wait 100ms
	*/
	digitalWrite(VCCEn, HIGH);

	delay(DELAY_TIME_MS);

	/* Send Display On command
	*/
	WriteSPI(CMD_DISPLAYON);
	

	delay(300);
}


/* ------------------------------------------------------------ */
/***	void Oledrgb::clear(void)
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Clear the display. This clears the memory buffer and then
**		updates the display.
*/
void Oledrgb::Clear()
{
	cmds[0] = CMD_CLEARWINDOW; 		// Enter the “clear mode”
	cmds[1] = 0x00;					// Set the starting column coordinates
	cmds[2] = 0x00;					// Set the starting row coordinates
	cmds[3] = OLEDRGB_WIDTH - 1;	// Set the finishing column coordinates;
	cmds[4] = OLEDRGB_HEIGHT - 1;	// Set the finishing row coordinates;
	WriteSPI(cmds, 5, NULL);
	delay(5);
}



void Oledrgb::DrawRectangle(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2, uint16_t lineColor, bool bFill, uint16_t fillColor)
{
    cmds[0] = CMD_FILLWINDOW;		//fill window
    cmds[1] = (bFill ? ENABLE_FILL: DISABLE_FILL);
    cmds[2] = CMD_DRAWRECTANGLE;	//draw rectangle
	cmds[3] = c1;					// start column
	cmds[4] = r1;					// start row
	cmds[5] = c2;					// end column
	cmds[6] = r2;					//end row

	cmds[7] = ExtractRFromRGB(lineColor);	//R					
	cmds[8] = ExtractGFromRGB(lineColor);	//G
	cmds[9] = ExtractBFromRGB(lineColor);	//R


	if(bFill)
	{
		cmds[10] = ExtractRFromRGB(fillColor);	//R					
		cmds[11] = ExtractGFromRGB(fillColor);	//G
		cmds[12] = ExtractBFromRGB(fillColor);	//R
	}
	WriteSPI(cmds, bFill ? 13: 10, NULL);
	delay(5);
}

void Oledrgb::DrawLine(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2, uint16_t lineColor)
{
	cmds[0] = CMD_DRAWLINE; 		//draw line
	cmds[1] = c1;					// start column
	cmds[2] = r1;					// start row
	cmds[3] = c2;					// end column
	cmds[4] = r2;					//end row
	cmds[5] = ExtractRFromRGB(lineColor);	//R					
	cmds[6] = ExtractGFromRGB(lineColor);	//G
	cmds[7] = ExtractBFromRGB(lineColor);	//R
	
	WriteSPI(cmds, 8, NULL);
	delay(5);	
	
}


void Oledrgb::DrawPixel(uint8_t c, uint8_t r, uint16_t pixelColor)
{
/*
	cmds[0] = CMD_DRAWRECTANGLE;	//draw rectangle
	cmds[1] = c;					// start column
	cmds[2] = r;					// start row
	cmds[3] = c;					// end column
	cmds[4] = r;					//end row

	cmds[5] = ExtractRFromRGB(pixelColor);	//R					
	cmds[6] = ExtractGFromRGB(pixelColor);	//G
	cmds[7] = ExtractBFromRGB(pixelColor);	//R
	
	WriteSPI(cmds, 8, NULL);
	delay(5);
*/
	uint8_t data[2];
	//set column start and end
	cmds[0] = CMD_SETCOLUMNADDRESS; 		
	cmds[1] = c;					// Set the starting column coordinates
	cmds[2] = OLEDRGB_WIDTH - 1;					// Set the finishing column coordinates

	//set row start and end
	cmds[3] = CMD_SETROWADDRESS; 		
	cmds[4] = r;					// Set the starting row coordinates
	cmds[5] = OLEDRGB_HEIGHT - 1;					// Set the finishing row coordinates

	data[0] = pixelColor >> 8;
	data[1] = pixelColor;
	
	WriteSPI(cmds, 6, data, 2);
	delay(5);	
	
}

uint8_t Oledrgb::WriteSPI(uint8_t bVal)
{
	uint8_t	bRx = -1;
	
	if(pdspi != NULL)
	{
		// make SS active	
		pdspi->setSelect(LOW);
		
		// write to SPI
		bRx = pdspi->transfer(bVal);

//		delay(5);
		// make SS inactive
		pdspi->setSelect(HIGH);

	}
//	delay(5);

	return bRx;
}

uint8_t Oledrgb::WriteSPI(uint8_t bVal1, uint8_t bVal2)
{
	uint8_t	bRx = -1;
	
	if(pdspi != NULL)
	{
		// make SS active	
		pdspi->setSelect(LOW);
		
		// write to SPI
		bRx = pdspi->transfer(bVal1);
		
//		delay(5);
		// write to SPI
		bRx = pdspi->transfer(bVal2);

//		delay(5);
		// make SS inactive
		pdspi->setSelect(HIGH);

	}
//	delay(5);

	return bRx;
}
uint8_t Oledrgb::WriteSPI(uint8_t *pCmd, int nCmd, uint8_t *pData, int nData)
{
	uint8_t	bRx = -1;
	
	int ib;
//uint16_t color = BuildRGB(  255,0, 0); // red pixel	
	if(pdspi != NULL)
	{
		// make SS active	
		pdspi->setSelect(LOW);
		
		for(ib = 0; ib < nCmd; ib++)
		{
			// write to SPI
			bRx = pdspi->transfer(pCmd[ib]);
		}
		if(pData != NULL)
		{
			digitalWrite(DataCmd, HIGH);
			for(ib = 0; ib < nData; ib++)
//			for(ib = 0; ib < 96*64; ib++)
			{
				// write to SPI
				bRx = pdspi->transfer(pData[ib]);
				//pdspi->transfer(color >> 8);
				//pdspi->transfer(color);		
			}
			digitalWrite(DataCmd, LOW);
		}
		// make SS inactive
		pdspi->setSelect(HIGH);
	}
//	delay(5);

	return bRx;
}
void Oledrgb::DrawBitmap(uint8_t c1, uint8_t r1, uint8_t c2, uint8_t r2, uint8_t *pBmp)
{
	//set column start and end
	cmds[0] = CMD_SETCOLUMNADDRESS; 		
	cmds[1] = c1;					// Set the starting column coordinates
	cmds[2] = c2;					// Set the finishing column coordinates

	//set row start and end
	cmds[3] = CMD_SETROWADDRESS; 		
	cmds[4] = r1;					// Set the starting row coordinates
	cmds[5] = r2;					// Set the finishing row coordinates

	WriteSPI(cmds, 6, pBmp, (((c2 - c1 + 1)  * (r2 - r1 + 1)) << 1));
	delay(5);
}

/* ------------------------------------------------------------ */

/************************************************************************/

