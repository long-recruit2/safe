
/************************************************************************/
/*									*/
/*  OLEDrgbDemo  --  OLEDrgb Display demo                                     */
/*									*/
/************************************************************************/
/*  Author: 	Cristian Fatu						*/
/*  Copyright 2015, Digilent Inc.					*/
/************************************************************************/

/************************************************************************/
/*  Revision History:							*/
/*									*/
/*  07/20/2015(CristianF): created						*/
/*									*/
/************************************************************************/

/************************************************************************/
/*  Board Support:							*/
/*									*/
/*  chipKit Uno with Pmod Shield:     Header JC	                	*/
/*   (Note: To use this, download and deploy the Board Variant for      */
/*          chipKIT UNO32 W/Pmod Shield from the Digilent website)      */              
/*  Cerebot Mx3ck:                    Header JE                         */
/*  Cerebot Mx4ck:                    Header JB                         */
/*                   (make sure JP3 is on RB15)                         */
/*  Cerebot Mx7ck:                    Header JD                         */
/************************************************************************/

#include <DSPI.h>
#include <OLEDrgb.h>

Oledrgb OLEDrgb;



void setup()
{
  pinMode(PIN_LED3, OUTPUT);
  pinMode(PIN_LED4, OUTPUT);
  

  digitalWrite(PIN_LED3, HIGH);
  digitalWrite(PIN_LED4, LOW);
  
  OLEDrgb.begin();

  digitalWrite(PIN_LED3, LOW);
  digitalWrite(PIN_LED4, HIGH);

  
  OLEDrgb.DrawRectangle(10, 20, 80, 60, OLEDrgb.BuildRGB(  0,255,  0), true, OLEDrgb.BuildRGB(  0,  0,255) ); // rectngle green line, blue fill
  OLEDrgb.DrawLine(10, 20, 45, 5, OLEDrgb.BuildRGB(  223,140,  20)); // orange line
  OLEDrgb.DrawPixel(50, 15, OLEDrgb.BuildRGB(  255 ,0,  0)); // red pixel

  
  digitalWrite(PIN_LED3, HIGH);
  digitalWrite(PIN_LED4, HIGH);


}

void loop()
{

}
