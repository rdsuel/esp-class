
// =====================
// Include Libraries.
// =====================
// Exercise 2: Connect to wifi.
#include <ESP8266WiFi.h>
// Exercise 3: OLED display.
#include <Wire.h>
#include "SSD1306Wire.h"

// =====================
// Variables
// =====================
// Exercise 2: Connect to wifi.
const char* ssid     = "iot_class";
const char* password = "password";
// Exercise 3: OLED display.
SSD1306Wire display(0x3c, 3, 1);

void connectToWifi()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void drawDisplay(char *timeOfDay, char *temp, char *humidity, char *ip)
{
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  // Time of day (rows 0-12)
  display.drawString(0, 0, "Time:"); 
  display.drawString(6, 0, timeOfDay);
  display.drawHorizontalLine(0, 12, 128);
  // Temperature (rows 15-27)
  display.drawString(0, 15, "Temp:");
  display.drawString(6, 15, temp);
  display.drawHorizontalLine(0, 27, 128);
  // Humidity (rows 30-42)
  display.drawString(0, 30, "Hum%:");
  display.drawString(6, 30, humidity);
  // IP address (rows 45+)
  display.drawString(0, 45, "IP  :");
  display.drawString(6, 45, ip);
  // Render
  display.display();
}

void setup()
{
  // Exercise 1: Blink the on-board LED.
  pinMode(LED_BUILTIN, OUTPUT);

  // Exercise 2: Connect to wifi.
  connectToWifi();

  // Exercise 3: OLED display
  display.init();
  drawDisplay("12:34:56 PM", "96 Degrees", "53%", "192.168.1.12");
}

void loop()
{
  // Exercise 1: Blink the on-board LED.
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);

  // Exercise 3: OLED display

}
