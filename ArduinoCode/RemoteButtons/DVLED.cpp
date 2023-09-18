//=================================================================================================
/// @file  DVLED.cpp
///
/// Creates an LED Class to manage LED illumination without the use of the delay
///
/// (C) 2023 Daniel VanVolkinburg 
/// Released under GNU Public License (GPL)
/// email dvanvolk@ieee.org
//=================================================================================================
#include "DVLED.h"

//-------------------------------------------------------------------------------------------------
/// Constructor
///  Automatically calls the pin initialization function
///
/// @param   nLedPin        GPIO pin the led is connected to
/// @param   nOnValue		HIGH = a write of HIGH will enable the LED, the LED is active HIGH
///						     LOW = a write of LOW will enable the LED, the LED is active LOW
//-------------------------------------------------------------------------------------------------
CDVLED::CDVLED( uint8_t nLedPin, uint8_t nOnValue ) 
  : mnPreviousMillis( 0 )
  , mnSequenceTotal( 0 )
  , mnCurrentIndex( 0 )
  , mbEnableFlashSequence( false )
  , mbStatus( false )
  , mnPin( nLedPin )
  , mnOnValue( nOnValue )
{
	Init( nLedPin, nOnValue );
}

//-------------------------------------------------------------------------------------------------
/// Returns the current LED state
///
/// @return  bool      LED State, true = LED is ON, false = LED is OFF
//-------------------------------------------------------------------------------------------------
bool CDVLED::GetState( void )
{
	return mbStatus;
}

//-------------------------------------------------------------------------------------------------
/// Turns on the LED
//-------------------------------------------------------------------------------------------------
void CDVLED::On( void )
{
	digitalWrite( mnPin, mnOnValue );
	mbStatus = true;
}

//-------------------------------------------------------------------------------------------------
/// Turns OFF the LED
//-------------------------------------------------------------------------------------------------
void CDVLED::Off( void )
{
	digitalWrite( mnPin,!mnOnValue );
	mbStatus = false;
}

//-------------------------------------------------------------------------------------------------
/// Toggles the state of the LED, if the LED is on it will turn it off, if the LED is off it will 
///  turn it on
//-------------------------------------------------------------------------------------------------
void CDVLED::Toggle( void )
{
	GetState() ? Off() : On();
}

//-------------------------------------------------------------------------------------------------
/// Starts Flashing the LEDs with the specified sequence
///
/// @param   pLedSequence         Pointer to the LED Sequence to start
/// @param   nSequenceTotal       Total Number of States in the Sequence
//-------------------------------------------------------------------------------------------------
void CDVLED::StartFlashingSequence( LedSequence* pLedSequence, unsigned int nSequenceTotal )
{
	mpLedSequence = pLedSequence;
	mnSequenceTotal = nSequenceTotal;
	mnCurrentIndex = 0;
	EnableFlashSequence( true );
}

//-------------------------------------------------------------------------------------------------
/// Stops LED Flashing Sequence
//-------------------------------------------------------------------------------------------------
void CDVLED::StopFlashingSequence( void )
{
	EnableFlashSequence( false );
}

//-------------------------------------------------------------------------------------------------
/// Update the LED sequence, check the time and will change the LED state accordingly
///  Only needs to be called when the LED is in a flashing sequence
//-------------------------------------------------------------------------------------------------
void CDVLED::Update( void )
{
 // check to see if it's time to change the state of the LED
  unsigned long nCurrentMillis = millis();
  
  if( mbEnableFlashSequence == true )
  {
	if(( nCurrentMillis - mnPreviousMillis ) >= mpLedSequence[mnCurrentIndex].nTime_Ms )
	{
	  mnPreviousMillis = nCurrentMillis;  // Remember the time

	  mnCurrentIndex++;
	  if( mnCurrentIndex >= mnSequenceTotal)
	  {
		mnCurrentIndex = 0;
	  }
		
	  if( mpLedSequence[mnCurrentIndex].bLedState == true )
	  {
		On();
	  }
	  else
	  {
		Off();
	  }
	}
  }
}

//-------------------------------------------------------------------------------------------------
/// Initializes the LED Set the GPIO pin as an output
///
/// @param   nLedPin          GPIO pin the led is connected to
/// @param   nOnValue		  HIGH = a write of HIGH will enable the LED, the LED is active HIGH
///							   LOW = a write of LOW will enable the LED, the LED is active LOW
//-------------------------------------------------------------------------------------------------
void CDVLED::Init( uint8_t nLedPin, uint8_t nOnValue )
{
	mnOnValue = nOnValue;
	mnPin = nLedPin;
	pinMode( mnPin, OUTPUT );
	Off();
}

//-------------------------------------------------------------------------------------------------
/// Starts or Stops the LED Sequence
///
/// @param   bEnable         true = Start, false = Stop
//-------------------------------------------------------------------------------------------------
void CDVLED::EnableFlashSequence( bool bEnable )
{
	mbEnableFlashSequence = bEnable;
	if( bEnable == true )
	{
	  if( mpLedSequence[mnCurrentIndex].bLedState == true )
	  {
		On();
	  }
	  else
	  {
		Off();
	  }
	}
	else
	{
		Off();
	}
}

