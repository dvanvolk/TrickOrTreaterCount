//=================================================================================================
/// Arduino Code to read button presses and send them to the Receiver 
///
/// (C) 2023 Daniel VanVolkinburg 
/// Released under GNU Public License (GPL)
/// email dvanvolk@ieee.org
//=================================================================================================
#include <Button.h>
#include "DVLED.h"

#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

void RadioInit( void );
void SendMessage( uint8_t dataId );

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

// Radio Hardware (Feather M0)
#define RFM69_CS    8
#define RFM69_INT   3
#define RFM69_RST   4
#define RF69_FREQ   915.0

// Who am i? (server address)
#define RADIO_ADDRESS   1

// Where to send packets to! MY_ADDRESS in client (RX) should match this.
#define SERVER_ADDRESS 2

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram rf69_manager(rf69, RADIO_ADDRESS);

uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
uint8_t data[] = "  OK";

//-------------------------------------------------
// Default Arduino Setup Function
//-------------------------------------------------
void setup() 
{
    Serial.begin(115200);

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

void RadioInit( void )
{
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  Serial.println("Feather Addressed RFM69 TX Test!");
  Serial.println();

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69_manager.init()) {
    Serial.println("RFM69 radio init failed");
    while (1);
  }
  Serial.println("RFM69 radio init OK!");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  // The encryption key has to be the same as the one in the client
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
  rf69.setEncryptionKey(key);

  Serial.print("RFM69 radio @");  Serial.print((int)RF69_FREQ);  Serial.println(" MHz");
}

void SendMessage( uint8_t dataId )
{
  char radiopacket[20] = "Button ";

  itoa(dataId, radiopacket+7, 10);
// Send a message to the DESTINATION!
  if (rf69_manager.sendtoWait((uint8_t *)radiopacket, strlen(radiopacket), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAckTimeout(buf, &len, 2000, &from)) {
      buf[len] = 0; // zero out remaining string

      Serial.print("Got reply from #"); Serial.print(from);
      Serial.print(" [RSSI :");
      Serial.print(rf69.lastRssi());
      Serial.print("] : ");
      Serial.println((char*)buf);
    } else {
      Serial.println("No reply, is anyone listening?");
    }
  } else {
    Serial.println("Sending failed (no ack)");
  }
}
