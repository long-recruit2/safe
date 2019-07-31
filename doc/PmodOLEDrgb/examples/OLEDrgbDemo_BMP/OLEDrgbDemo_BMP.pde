
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

uint8_t bmp[96*64*2];

void fillBmp()
{
  // generate a simple pattern into the bmp by incrementing the color 16 bits
  int i;
  uint16_t color = OLEDrgb.BuildRGB(  255,0, 0); // red pixel
  for (i = 0; i < 96*64*2; i+=2)
  {
    bmp[i] = color>>8;
    bmp[i+1] = color;
    color ++;
  }
}


void setup()
{
  pinMode(PIN_LED3, OUTPUT);
  pinMode(PIN_LED4, OUTPUT);
  
 // delay(1000);

  fillBmp();
  digitalWrite(PIN_LED3, HIGH);
  digitalWrite(PIN_LED4, LOW);
  
  OLEDrgb.begin();

  digitalWrite(PIN_LED3, LOW);
  digitalWrite(PIN_LED4, HIGH);

  
  OLEDrgb.DrawBitmap(0, 0, 95, 63,  bmp);

  
  digitalWrite(PIN_LED3, HIGH);
  digitalWrite(PIN_LED4, HIGH);


}

void loop()
{
}
