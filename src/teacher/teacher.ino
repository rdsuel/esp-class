
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

// =====================
// Variables
// =====================
// Exercise 2: Connect to wifi.
const char* ssid     = "iot_class";
const char* password = "password";

// Exercise 3: OLED display and NTP time.
SSD1306Wire  display(0x3c, 5, 4);
int utc = -4;
WiFiUDP udp;
NTPClient timeClient(udp, "time.nist.gov", utc * 3600, 60000);

// Exercise 4: Temp/Humidity Sensor
DHT dht(0, DHT22);
float humidity;
float temperature;

// =====================
// Functions
// =====================

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
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello world");
  display.display();

  // Exercise 3: NTP Time
  timeClient.begin();
  timeClient.update();

  // Exercise 4: Temp/Humidity Sensor
  dht.begin();
}

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
//  display.display();

  // Exercise 4: Temp/Humidity Sensor
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Temp: ");
  Serial.print(temperature);
  Serial.println(" Celsius");
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 32, "100 F");
  display.drawString(64, 32, "60%");
  display.display();
}
