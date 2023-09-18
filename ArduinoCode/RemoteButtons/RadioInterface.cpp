//=================================================================================================
/// @file  DVLED.cpp
///
/// Wrapper Class for the Radio TX interface.  Pulled from RadioHead69_AddrDemo_TX
///
/// (C) 2023 Daniel VanVolkinburg 
/// Released under GNU Public License (GPL)
/// email dvanvolk@ieee.org
//=================================================================================================
#include "RadioInterface.h"
#include <SPI.h>


// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0


//------------------------------------------------------------------------------------------------
/// Constructor
///  Initialize the Radio and Interface
///
/// @param   nChipSelectPin        GPIO pin the radios chip select is tied to
/// @param   nInterruptPin		 GPIO pin the radios interrupt is tied to
/// @param   nResetPin		 	 GPIO pin the radios rest signal is tied to
//-------------------------------------------------------------------------------------------------
CRadioInterface::CRadioInterface( uint8_t nChipSelectPin, uint8_t nInterruptPin, uint8_t nResetPin )
{
	mpRadio_RF95 = new RH_RF95(nChipSelectPin, nInterruptPin);
	// Class to manage message delivery and receipt, using the driver declared above
	RHReliableDatagram rf69_manager(rf69, MY_ADDRESS);
	Reset( nResetPin );

	while (!mpRadio_RF95->init()) 
	{
		Serial.println("LoRa radio init failed");
		Serial.println("Uncomment '#define SERIAL_DEBUG' in RH_RF95.cpp for detailed debug info");
		while (1);
  	}

	// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
	if (!mpRadio_RF95->setFrequency(RF95_FREQ)) 
	{
	Serial.println("setFrequency failed");
	while (1);
	}
	Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

	// Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

	// The default transmitter power is 13dBm, using PA_BOOST.
	// If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
	// you can set transmitter powers from 5 to 23 dBm:
	mpRadio_RF95->setTxPower(23, false);
}

//-------------------------------------------------------------------------------------------------
/// Resets the Radio by toggling the Reset Pin
///
/// @param   nResetPin		 	 GPIO pin the radios rest signal is tied to
//-------------------------------------------------------------------------------------------------
void CRadioInterface::Reset( uint8_t nResetPin )
{
	// manual reset
	digitalWrite(nResetPin, LOW);
	delay(10);
	digitalWrite(nResetPin, HIGH);
	delay(10);
}