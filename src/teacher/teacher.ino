
// =====================
// Include Libraries.
// =====================
// Exercise 2: Connect to wifi.
#include <ESP8266WiFi.h>
// Exercise 3: Display and NTP time.
#include <Wire.h>
#include "SSD1306Wire.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
// Exercise 4: Temp/Humidity Sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
// Exercise 5: Adafruit
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// =====================
// Variables/Constants
// =====================
// Exercise 2: Connect to wifi.
const char* ssid     = "iot_class";
const char* password = "password";

// Exercise 3: OLED display and NTP time.
SSD1306Wire  display(0x3c, 5, 4);
int utc = -4; // Eastern daylight time
WiFiUDP udp;
NTPClient timeClient(udp, "time.nist.gov", utc * 3600, 60000);

// Exercise 4: Temp/Humidity Sensor
DHT dht(0, DHT22);
float humidity;
float temperature;

// Exercise 5: Adafruit
// Add Adafruit credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "rdsuel"
#define AIO_KEY         "9ee103074b6548eea4c5e7ed722efaf5"

WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// Setup feeds for temperature & humidity
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");


// =====================
// Functions
// =====================

void connectToAdafruit() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0)
  {
    switch (ret)
    {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
    {
      mqtt.disconnect();
    }

    Serial.println(F("Retrying connection..."));
    delay(5000);
  }

  Serial.println(F("Adafruit IO Connected!"));
}

void setup()
{
  // Exercise 1: Blink the on-board LED.
  pinMode(LED_BUILTIN, OUTPUT);

  // Exercise 2: Connect to wifi.
  Serial.begin(115200);
  delay(10);
  Serial.println();
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
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Exercise 3: OLED display
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 0, "Hello world");
  display.display();

  // Exercise 3: NTP Time
  timeClient.begin();
  timeClient.update();

  // Exercise 4: Temp/Humidity Sensor
  dht.begin();

  // Exercise 5: Adafruit
  connectToAdafruit();
}

int seconds = 0;
void loop()
{
  // Exercise 1: Blink the on-board LED.
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);

  // Exercise 3: OLED display and NTP time.
  Serial.println(timeClient.getFormattedTime());
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  display.drawString(63, 0, timeClient.getFormattedTime());

  // Exercise 4: Temp/Humidity Sensor
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(true);
  // https://www.arduino.cc/en/Reference.StringConstructor
  String tempString = String(temperature, 1);
  String humidityString = String(humidity, 1);
  String sensorString = String(tempString + "F   " + humidityString + "%");
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(63, 31, sensorString);
  Serial.println(sensorString);
  display.display();

  // Exercise 5: Adafruit
  if (seconds++ > 10)
  {
    seconds = 0;
    
    if (! mqtt.ping(3))
    {
      Serial.println(F("Failed to reach the server"));
      if (! mqtt.connected())
      {
        connectToAdafruit();
      }
    }

    if (! temperatureFeed.publish(temperature))
    {
      Serial.println(F("Failed to publish temperature"));
    }
    else
    {
      Serial.print(temperature);
      Serial.println(F(" F published!"));
    }

    if (! humidityFeed.publish(humidity))
    {
      Serial.println(F("Failed to publish humidity"));
    }
    else
    {
      Serial.print(humidity);
      Serial.println(F("% published!"));
    }
  }
}
