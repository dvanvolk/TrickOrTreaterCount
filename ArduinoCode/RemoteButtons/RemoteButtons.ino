//=================================================================================================
/// Arduino Code to read button presses and send them to the Receiver 
///
/// (C) 2023 Daniel VanVolkinburg 
/// Released under GNU Public License (GPL)
/// email dvanvolk@ieee.org
//=================================================================================================
#include <Button.h>
#include "DVLED.h"

// -- Hardware Configuration ---
// Buttons
#define RED_BUTTON_PIN A1
Button mCRedButton = Button(RED_BUTTON_PIN,PULLUP);
#define UP_BUTTON_PIN A2
Button mCUpButton = Button(UP_BUTTON_PIN,PULLUP);
#define DOWN_BUTTON_PIN A3
Button mCDownButton = Button(DOWN_BUTTON_PIN,PULLUP);

// LEDs
#define BUILT_IN_LED 13
CDVLED mCBuiltInLed = CDVLED( BUILT_IN_LED, HIGH );
#define RED_POWER_LED 6
CDVLED mCPowerLed = CDVLED(RED_POWER_LED, LOW );
#define RED_STATUS_LED 12
CDVLED mCRedStatusLed = CDVLED(RED_STATUS_LED, LOW );
#define YELLOW_STATUS_LED 11
CDVLED mCYellowStatusLed = CDVLED(YELLOW_STATUS_LED, LOW );
#define GREEN_STATUS_LED 10
CDVLED mCGreenStatusLed = CDVLED(GREEN_STATUS_LED, LOW );

//-------------------------------------------------
// Default Arduino Setup Function
//-------------------------------------------------
void setup() 
{
    mCBuiltInLed.Off();
    mCPowerLed.Off();
    mCRedStatusLed.Off();
    mCYellowStatusLed.Off();
    mCGreenStatusLed.Off();

}

//-------------------------------------------------
// Default Arduino Loop Function
//-------------------------------------------------
void loop() 
{
  if(mCRedButton.isPressed())
  {
    mCRedStatusLed.On();
  }
  else
  {
    mCRedStatusLed.Off();
  }

  if(mCUpButton.isPressed())
  {
    mCYellowStatusLed.On();
  }
  else
  {
    mCYellowStatusLed.Off();
  }

  if(mCDownButton.isPressed())
  {
    mCGreenStatusLed.On();
  }
  else
  {
    mCGreenStatusLed.Off();
  }

  // mCPowerLed.blink(500);  
  // mCRedStatusLed.blink(500);
  // mCYellowStatusLed.blink(500);
  // mCGreenStatusLed.blink(500);
}
