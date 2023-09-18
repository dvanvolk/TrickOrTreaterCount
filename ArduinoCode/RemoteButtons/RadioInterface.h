//=================================================================================================
/// @file  RadioInterface.h
///
/// Wrapper Class for the Radio TX interface.  Pulled from RadioHead69_AddrDemo_TX
///
/// (C) 2023 Daniel VanVolkinburg 
/// Released under GNU Public License (GPL)
/// email dvanvolk@ieee.org
//=================================================================================================
#ifndef _RADIOINTERFACE_H_
#define _RADIOINTERFACE_H_

#include "Arduino.h"
#include <RH_RF95.h>

//-------------------------------------------------------------------------------------------------
/// Manages a single LED, provides flashing of the led without the use of the delay function
///  The class only turns on and off the LED, it will not attempt to fade the LED on or off
//-------------------------------------------------------------------------------------------------
class CRadioInterface
{
		
   public:	  
	  //-------------------------------------------------------------------------------------------------
      /// Constructor
	  ///  Initialize the Radio and Interface
	  ///
	  /// @param   nChipSelectPin        GPIO pin the radios chip select is tied to
	  /// @param   nInterruptPin		 GPIO pin the radios interrupt is tied to
	  /// @param   nResetPin		 	 GPIO pin the radios rest signal is tied to
      //-------------------------------------------------------------------------------------------------
	  CRadioInterface( uint8_t nChipSelectPin, uint8_t nInterruptPin, uint8_t nResetPin );

	  //-------------------------------------------------------------------------------------------------
	  /// destructor
	  //-------------------------------------------------------------------------------------------------
      ~CRadioInterface() {}

	  // Class implementation 
	public:  
	


   private:

		//-------------------------------------------------------------------------------------------------
		/// Resets the Radio by toggling the Reset Pin
		///
		/// @param   nResetPin		 	 GPIO pin the radios rest signal is tied to
		//-------------------------------------------------------------------------------------------------
		void Reset( uint8_t nResetPin );
   
   private:
	RH_RF95 *mpRadio_RF95;
};

#endif /* _RADIOINTERFACE_H_ */
