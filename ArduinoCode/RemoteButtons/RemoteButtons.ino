//=================================================================================================
/// Arduino Code to read button presses and send them to the Receiver, four indicator LEDs show
///  status
///  Mostly an Copy of RadioHead69_AddrDemo_TX, from Adafruit
///  Code runs on Adafruit Feather M0 RFM69HCW Packet Radio
///    https://www.adafruit.com/product/3176 
///
/// (C) 2023 Daniel VanVolkinburg 
/// Released under GNU Public License (GPL)
/// email dvanvolk@ieee.org
//=================================================================================================
#include "DVLED.h"

#include <SPI.h>
#include <RH_RF69.h>
#include <RHReliableDatagram.h>

// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

// -- Function Prototypes ---
void RadioInit( void );
void SendMessage( const char* message );
void HeartBeat( void );

// -- Hardware Configuration ---
// Buttons
#define RED_BUTTON_PIN A1
Bounce2::Button mCRedButton = Bounce2::Button();
#define UP_BUTTON_PIN A2
Bounce2::Button mCUpButton = Bounce2::Button();
#define DOWN_BUTTON_PIN A3
Bounce2::Button mCDownButton = Bounce2::Button();
#define BUTTON_INTERVAL_MS 5

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

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = (1000 * 30);  // interval at which to blink (milliseconds)
unsigned long heart_count = 0;

//-------------------------------------------------
// Default Arduino Setup Function
//-------------------------------------------------
void setup() 
{
    Serial.begin(115200);

    mCRedButton.attach( RED_BUTTON_PIN,  INPUT_PULLUP );
    mCRedButton.interval(BUTTON_INTERVAL_MS);
    mCRedButton.setPressedState(LOW); 

    mCUpButton.attach( UP_BUTTON_PIN,  INPUT_PULLUP );
    mCUpButton.interval(BUTTON_INTERVAL_MS);
    mCUpButton.setPressedState(LOW); 

    mCDownButton.attach( DOWN_BUTTON_PIN,  INPUT_PULLUP );
    mCDownButton.interval(BUTTON_INTERVAL_MS);
    mCDownButton.setPressedState(LOW); 

    mCBuiltInLed.Off();
    mCPowerLed.On();
    mCRedStatusLed.Off();
    mCYellowStatusLed.Off();
    mCGreenStatusLed.Off();

    RadioInit();

}

//-------------------------------------------------
// Default Arduino Loop Function
//-------------------------------------------------
void loop() 
{
  char Button_Message[20] = "Button ";
  mCRedButton.update();
  mCUpButton.update();
  mCDownButton.update();

  if ( mCRedButton.pressed() ) 
  {
    mCRedStatusLed.On();
    itoa(1, Button_Message+7, 10);
    SendMessage( Button_Message );
  }
  else if( mCRedButton.released() )
  {
    mCRedStatusLed.Off();
  }
  
  if ( mCUpButton.pressed() ) 
  {
    mCYellowStatusLed.On();
    itoa(2, Button_Message+7, 10);
    SendMessage( Button_Message );
  }
  else if( mCUpButton.released() )
  {
    mCYellowStatusLed.Off();
  }

  if ( mCDownButton.pressed() ) 
  {
    mCGreenStatusLed.On();
    itoa(3, Button_Message+7, 10);
    SendMessage( Button_Message );
  }
  else if( mCDownButton.released() )
  {
    mCGreenStatusLed.Off();
  }

  HeartBeat();
}

//-------------------------------------------------
// Initialize the Radio Interface
//-------------------------------------------------
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

//-------------------------------------------------
// Send a message to the Radio Receiver 
//
// @param  message     Pointer to the message String to send
//-------------------------------------------------
void SendMessage( const char* message )
{
  // Send a message to the DESTINATION!
  if (rf69_manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf69_manager.recvfromAckTimeout(buf, &len, 2000, &from)) 
    {
      buf[len] = 0; // zero out remaining string

      Serial.print("Got reply from #"); Serial.print(from);
      Serial.print(" [RSSI :");
      Serial.print(rf69.lastRssi());
      Serial.print("] : ");
      Serial.println((char*)buf);
    } 
    else 
    {
      Serial.println("No reply, is anyone listening?");
    }
  } 
  else 
  {
    Serial.println("Sending failed (no ack)");
  }
}

//-------------------------------------------------
// Send a Heart Beat Message to let the host know 
//  The radio is alive 
//-------------------------------------------------
void HeartBeat(void)
{
  String Heart_Message = "Heart ";
    
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    heart_count++;

    Heart_Message += heart_count;

    Serial.println(Heart_Message);

    mCPowerLed.On();
    SendMessage( Heart_Message.c_str() );
    mCPowerLed.Off();
  }
}