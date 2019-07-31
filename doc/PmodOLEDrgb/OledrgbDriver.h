/************************************************************************/
/*																		*/
/*	OledDriver.h -- Interface Declarations for OLED Display Driver 		*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2011, Digilent Inc.										*/
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
/*	Interface to OledDriver.c											*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	04/29/2011(GeneA): created											*/
/*																		*/
/************************************************************************/

#if !defined(OLEDDRIVER_INC)
#define	OLEDDRIVER_INC

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#define	cbOledDispMax	512		//max number of bytes in display buffer

#define	ccolOledMax		128		//number of display columns
#define	crowOledMax		32		//number of display rows
#define	cpagOledMax		4		//number of display memory pages

#define	cbOledChar		8		//font glyph definitions is 8 bytes long
#define	chOledUserMax	0x20	//number of character defs in user font table
#define	cbOledFontUser	(chOledUserMax*cbOledChar)

/* Graphics drawing modes.
*/
#define	modOledSet		0
#define	modOledOr		1
#define	modOledAnd		2
#define	modOledXor		3

#define RGBOLED_WIDTH                      96
#define RGBOLED_HEIGHT                     64

#define RGB(R,G,B)                  (((R>>3)<<11) | ((G>>2)<<5) | (B>>3))

#define cmdSetContrastColorA	0x81    //
#define cmdSetContrastColorB	0x82    //
#define cmdSetContrastColorC	0x83    //
#define cmdSetMasterCurrent		0x87    //
#define cmdSetSpeedColorA		0x8A    //
#define cmdSetSpeedColorB		0x8B    //
#define cmdSetSpeedColorC		0x8C    //
#define cmdSetRemap				0xA0
#define cmdSetDisplayStartLine	0xA1
#define cmdSetDisplayOffset		0xA2	//A[6:0] sets vertical scroll
#define cmdSetDisplayModeOff	0xA4	//all pixels off at GS level 0
#define cmdSetMuxRatio      	0xA8	//A[6:0] start address, B[6:0] end address
#define cmdMasterConfig 		0xAD	//A[0]=0 => select external Vdd, A[0] = 1 => Enable internal Vdd regulator [reset]
#define cmdOledDisplayOff		0xAE	//display off
#define cmdOledDisplayOn		0xAF	//display on
#define cmdPowerSavingMode		0xB0
#define cmdPhaseLength			0xB1	//A[3:0] is phase 1 period, A[7:4] is phase 2 period
#define cmdClkDividerOscFreq	0xB3	//A[3:0] divide  DCLK, A[7:4] set oscillator frequency
#define cmdSetPreChargeVoltage	0xBB	//A[4:0] sets precharge level as ( .2 (00000) to .6 (11111) )*Vcc
#define cmdSetVcomh				0xBE	//A[2:0] set Vcomh as ( .72 (000) to .86 (111) )*Vcc
#define cmdDrawLine             0x21
#define cmdDrawRectangle        0x22
#define cmdClearWindow          0x25
#define cmdFillWindow           0x26
#define cmdDeactiveScrolling    0x2E

#define CMD_DRAW_LINE                       0x21
#define CMD_DRAW_RECTANGLE                  0x22
#define CMD_COPY_WINDOW                     0x23
#define CMD_DIM_WINDOW                      0x24
#define CMD_CLEAR_WINDOW                    0x25
#define CMD_FILL_WINDOW                     0x26
    #define DISABLE_FILL    0x00
    #define ENABLE_FILL     0x01
#define CMD_CONTINUOUS_SCROLLING_SETUP      0x27
#define CMD_DEACTIVE_SCROLLING              0x2E
#define CMD_ACTIVE_SCROLLING                0x2F

#define CMD_SET_COLUMN_ADDRESS              0x15
#define CMD_SET_ROW_ADDRESS                 0x75
#define CMD_SET_CONTRAST_A                  0x81
#define CMD_SET_CONTRAST_B                  0x82
#define CMD_SET_CONTRAST_C                  0x83
#define CMD_MASTER_CURRENT_CONTROL          0x87
#define CMD_SET_PRECHARGE_SPEED_A           0x8A
#define CMD_SET_PRECHARGE_SPEED_B           0x8B
#define CMD_SET_PRECHARGE_SPEED_C           0x8C
#define CMD_SET_REMAP                       0xA0
#define CMD_SET_DISPLAY_START_LINE          0xA1
#define CMD_SET_DISPLAY_OFFSET              0xA2
#define CMD_NORMAL_DISPLAY                  0xA4
#define CMD_ENTIRE_DISPLAY_ON               0xA5
#define CMD_ENTIRE_DISPLAY_OFF              0xA6
#define CMD_INVERSE_DISPLAY                 0xA7
#define CMD_SET_MULTIPLEX_RATIO             0xA8
#define CMD_DIM_MODE_SETTING                0xAB
#define CMD_SET_MASTER_CONFIGURE            0xAD
#define CMD_DIM_MODE_DISPLAY_ON             0xAC
#define CMD_DISPLAY_OFF                     0xAE
#define CMD_NORMAL_BRIGHTNESS_DISPLAY_ON    0xAF
#define CMD_POWER_SAVE_MODE                 0xB0
#define CMD_PHASE_PERIOD_ADJUSTMENT         0xB1
#define CMD_DISPLAY_CLOCK_DIV               0xB3
#define CMD_SET_GRAy_SCALE_TABLE            0xB8
#define CMD_ENABLE_LINEAR_GRAY_SCALE_TABLE  0xB9
#define CMD_SET_PRECHARGE_VOLTAGE           0xBB
#define CMD_SET_V_VOLTAGE                   0xBE


#define disableFill    0x00
#define enableFill     0x01

/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

void	OledInit();
void	OledTerm();
void	OledDisplayOn();
void	OledDisplayOff();
void	OledClear();
void	OledClearBuffer();
void	OledUpdate();
void 	OledrgbDrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t outColor, uint16_t fillColor);

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
