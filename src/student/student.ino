
// =====================
// Include Libraries.
// =====================
// Exercise 2: Connect to wifi.
//#include <ESP8266WiFi.h>

// Exercise 3: Display and NTP time.
//#include <Wire.h>
//#include "SSD1306Wire.h"
//#include <NTPClient.h>
//#include <WiFiUdp.h>

// Exercise 4: Temp/Humidity Sensor
//#include <Adafruit_Sensor.h>
//#include <DHT.h>

// Exercise 5: Adafruit
//#include "Adafruit_MQTT.h"
//#include "Adafruit_MQTT_Client.h"

// =====================
// Variables/Constants
// =====================

// Exercise 2: Connect to wifi.
//const char* ssid     = "SSID";
//const char* password = "PASSWORD";

// Exercise 3: OLED display and NTP time.
//SSD1306Wire  display(0x3c, 5, 4);
//int utc = -4; // Eastern daylight time
//WiFiUDP udp;
//NTPClient timeClient(udp, "time.nist.gov", utc * 3600, 60000);

// Exercise 4: Temp/Humidity Sensor
//DHT dht(0, DHT22);
//float humidity;
//float temperature;

// Exercise 5: Adafruit
// Add Adafruit credentials
//#define AIO_SERVER      "io.adafruit.com"
//#define AIO_SERVERPORT  1883
//#define AIO_USERNAME    "YOUR-USERNAME"
//#define AIO_KEY         "YOUR-KEY"

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
//WiFiClient client;
//Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup feeds for temperature & humidity
//Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
//Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");


// =====================
// Functions
// =====================

//void connectToAdafruit() {
//
//  Serial.print(F("Connecting to Adafruit IO... "));
//
//  int8_t ret;
//
//  while ((ret = mqtt.connect()) != 0)
//  {
//    switch (ret)
//    {
//      case 1: Serial.println(F("Wrong protocol")); break;
//      case 2: Serial.println(F("ID rejected")); break;
//      case 3: Serial.println(F("Server unavail")); break;
//      case 4: Serial.println(F("Bad user/pass")); break;
//      case 5: Serial.println(F("Not authed")); break;
//      case 6: Serial.println(F("Failed to subscribe")); break;
//      default: Serial.println(F("Connection failed")); break;
//    }
//
//    if (ret >= 0)
//    {
//      mqtt.disconnect();
//    }
//
//    Serial.println(F("Retrying connection..."));
//    delay(5000);
//  }
//
//  Serial.println(F("Adafruit IO Connected!"));
//}

void setup()
{
  // Exercise 1: Blink the on-board LED.
  pinMode(LED_BUILTIN, OUTPUT);

  // Exercise 2: Connect to wifi.

  // Exercise 3: OLED display

  // Exercise 3: NTP Time

  // Exercise 4: Temp/Humidity Sensor

  // Exercise 5: Adafruit
}

void loop()
{
  // Exercise 1: Blink the on-board LED.
  // Blink the LED.
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  // Clear the display.
  // display.clear();

  // Exercise 3: OLED display and NTP time.
  // Read the time from the NTP client.
  // Write it to the display.

  // Exercise 4: Temp/Humidity Sensor
  // Read temp and humidity and write them to the display.

  // Render the display.
  // display.display();

  // Exercise 5: Adafruit
  // Publish temp and humidity every 5 seconds, max.
}
