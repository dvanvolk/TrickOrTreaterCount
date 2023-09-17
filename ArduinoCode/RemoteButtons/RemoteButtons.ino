/* -----------------------------------------------------------
* Arduino Code to read button presses and send them to the 
*  Receiver 
*  
* (C) 2023 Daniel VanVolkinburg 
* Released under GNU Public License (GPL)
* email dvanvolk@ieee.org
* -----------------------------------------------------------*/
#include <Button.h>
#include <LED.h>

// Hardware Configuration
#define RED_BUTTON_PIN A1
Button mCRedButton = Button(RED_BUTTON_PIN,PULLUP);
#define UP_BUTTON_PIN A2
Button mCUpButton = Button(UP_BUTTON_PIN,PULLUP);
#define DOWN_BUTTON_PIN A3
Button mCDownButton = Button(DOWN_BUTTON_PIN,PULLUP);

#define BUILT_IN_LED 13
LED mCBuiltInLed = LED(BUILT_IN_LED);
#define RED_POWER_LED 6
LED mCPowerLed = LED(RED_POWER_LED);
#define RED_STATUS_LED 12
LED mCRedStatusLed = LED(RED_STATUS_LED);
#define YELLOW_STATUS_LED 11
LED mCYellowStatusLed = LED(YELLOW_STATUS_LED);
#define GREEN_STATUS_LED 10
LED mCGreenStatusLed = LED(GREEN_STATUS_LED);

//-------------------------------------------------
// Default Arduino Setup Function
//-------------------------------------------------
void setup() 
{
    mCBuiltInLed.off();
    mCPowerLed.off();
    mCRedStatusLed.off();
    mCYellowStatusLed.off();
    mCGreenStatusLed.off();

}

//-------------------------------------------------
// Default Arduino Loop Function
//-------------------------------------------------
void loop() 
{
  if(mCRedButton.isPressed())
  {
    mCRedStatusLed.on();
  }
  else
  {
    mCRedStatusLed.off();
  }

  if(mCUpButton.isPressed())
  {
    mCYellowStatusLed.on();
  }
  else
  {
    mCYellowStatusLed.off();
  }

  if(mCDownButton.isPressed())
  {
    mCGreenStatusLed.on();
  }
  else
  {
    mCGreenStatusLed.off();
  }

  mCPowerLed.blink(500);  
  // mCRedStatusLed.blink(500);
  // mCYellowStatusLed.blink(500);
  // mCGreenStatusLed.blink(500);
}
