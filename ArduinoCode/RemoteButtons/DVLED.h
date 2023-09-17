//=================================================================================================
/// @file  DVLED.h
///
/// Creates an LED Class to manage LED illumination without the use of the delay
///
/// (C) 2023 Daniel VanVolkinburg 
/// Released under GNU Public License (GPL)
/// email dvanvolk@ieee.org
//=================================================================================================
#ifndef _DVLED_H_
#define _DVLED_H_

#include "Arduino.h"

//-------------------------------------------------------------------------------------------------
/// Manages a single LED, provides flashing of the led without the use of the delay function
///  The class only turns on and off the LED, it will not attempt to fade the LED on or off
//-------------------------------------------------------------------------------------------------
class CDVLED
{
	public:
		//-------------------------------------------------------------------------------------------------
		/// Structure which defines the LED state and the time in that state.
		//-------------------------------------------------------------------------------------------------
		struct LedSequence
		{
		  unsigned long nTime_Ms;   ///< Time in MS for the current state
		  bool bLedState;
		};
		
   public:	  
	  //-------------------------------------------------------------------------------------------	------
      /// Constructor
	  ///  Automatically calls the pin initialization function
	  ///
	  /// @param   nLedPin        GPIO pin the led is connected to
	  /// @param   nOnValue		  HIGH = a write of HIGH will enable the LED, the LED is active HIGH
	  ///						   LOW = a write of LOW will enable the LED, the LED is active LOW
      //-------------------------------------------------------------------------------------------------
	  CDVLED( uint8_t nLedPin = 13, uint8_t nOnValue = HIGH );

	  //-------------------------------------------------------------------------------------------------
	  /// destructor
	  //-------------------------------------------------------------------------------------------------
      ~CDVLED() {}

	  // Class implementation 
	public:  
	
		//-------------------------------------------------------------------------------------------------
		/// Initialises the LED Set the GPIO pin as an output
		///
		/// @param   nLedPin          GPIO pin the led is connected to
		/// @param   nOnValue		  HIGH = a write of HIGH will enable the LED, the LED is active HIGH
		///							   LOW = a write of LOW will enable the LED, the LED is active LOW
		//-------------------------------------------------------------------------------------------------
		void Init( uint8_t nLedPin, uint8_t nOnValue );	 

		//-------------------------------------------------------------------------------------------------
		/// Returns the current LED state
		///
		/// @return  bool      LED State, true = LED is ON, false = LED is OFF
		//-------------------------------------------------------------------------------------------------
		bool GetState( void );
		
		//-------------------------------------------------------------------------------------------------
		/// Turns on the LED
		//-------------------------------------------------------------------------------------------------
		void On( void );
		
		//-------------------------------------------------------------------------------------------------
		/// Turns OFF the LED
		//-------------------------------------------------------------------------------------------------
		void Off( void );
		
		//-------------------------------------------------------------------------------------------------
		/// Toggles the state of the LED, if the LED is on it will turn it off, if the LED is off it will 
		///  turn it on
		//-------------------------------------------------------------------------------------------------
		void Toggle( void );
		
		//-------------------------------------------------------------------------------------------------
		/// Starts Flashing the LEDs with the specified sequence
		///
		/// @param   pLedSequence         Pointer to the LED Sequence to start
		/// @param   nSequenceTotal       Total Number of States in the Sequence
		//-------------------------------------------------------------------------------------------------
		void StartFlashingSequence( LedSequence* pLedSequence, unsigned int nSequenceTotal );

		//-------------------------------------------------------------------------------------------------
		/// Stops LED Flashing Sequence
		//-------------------------------------------------------------------------------------------------
		void StopFlashingSequence( void );

		//-------------------------------------------------------------------------------------------------
		/// Update the LED sequence, check the time and will change the LED state accordingly
		///  Only needs to be called when the LED is in a flashing sequence
		//-------------------------------------------------------------------------------------------------
		void Update( void );

   private:

		//-------------------------------------------------------------------------------------------------
		/// Starts or Stops the LED Sequence
		///
		/// @param   bEnable         true = Start, false = Stop
		//-------------------------------------------------------------------------------------------------
		void EnableFlashSequence( bool bEnable = true );
   
   private:
   
	    // These maintain the current flashing state
		LedSequence* mpLedSequence;       ///< Pointer to the Current LED Sequence
		unsigned long mnPreviousMillis;   ///< will store last time LED was updated
		unsigned int mnSequenceTotal;     ///< Total objects in the Sequence Array
		unsigned int mnCurrentIndex;      ///< Current array Index.
		bool mbEnableFlashSequence;       ///< Flash Sequence Enable Flag
		bool mbStatus;					  ///< LED State true = on, false = off
		uint8_t mnPin;					  ///< GPIO Pin the LED is connected
		uint8_t mnOnValue;			      ///< Value to write to the pin to turn the LED on
};

#endif /* _DVLED_H_ */
