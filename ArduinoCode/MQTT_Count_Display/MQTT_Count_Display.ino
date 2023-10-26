//=================================================================================================
/// Arduino Code to display the total count on a 7-Segment display connected to the local MQTT 
///  server via a WIFI feather
///  The code is mostly a copy of the ESP8266 MQTT example
///  Code runs on Adafruit Feather HUZZAH with ESP8266, with an 
///  Adafruit 0.54" Quad Alphanumeric FeatherWing Display
///    https://www.adafruit.com/product/2821
///    https://www.adafruit.com/product/3130 
///
/// (C) 2023 Daniel VanVolkinburg 
/// Released under GNU Public License (GPL)
/// email dvanvolk@ieee.org
//=================================================================================================
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

#include "WifiLoginInfo.h"  // This is where the passwords and login-info is stored

const char* ssid = WLAN_SSID;
const char* password = WLAN_PASS;
const char* mqtt_server = MQTT_SERVER;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

//-------------------------------------------------
// Connect to the WIFI interface
//-------------------------------------------------
void setup_wifi() 
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//-------------------------------------------------
// Callback function, called when a MQTT message 
//  is received, this function just displays the 
//  payload value on the 7-Segment Display
//  Payload is expected to be just a numeric number
//  of the count
//-------------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) 
{
  alpha4.clear();
  alpha4.writeDisplay();

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  unsigned int start_pos = (4 - length);

  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
    alpha4.writeDigitAscii((start_pos + i), (char)payload[i]);
  }

  alpha4.writeDisplay();
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') 
  {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } 
  else 
  {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH 
  }

}


//-------------------------------------------------
// Re-connect to the MQTT Server
//-------------------------------------------------
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASS))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("Halloween\\TotalCount");
    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//-------------------------------------------------
// Default Arduino Setup Function
//-------------------------------------------------
void setup() 
{
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  alpha4.begin(0x70);  // pass in the address

  alpha4.clear();
  alpha4.writeDigitAscii(3, '0');
  alpha4.writeDisplay();

}

//-------------------------------------------------
// Default Arduino Loop Function
//-------------------------------------------------
void loop() 
{

  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

  // This is an "every things ok" message, that has no real purpose 
  unsigned long now = millis();
  if (now - lastMsg > (1000 * 60)) 
  {
    lastMsg = now;
    Serial.println("Still alive");
  }
}
