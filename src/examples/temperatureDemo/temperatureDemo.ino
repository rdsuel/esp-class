
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN  4         // what pin we're connected to
#define DHTTYPE DHT22     // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

//Variables
float humidity;
float temperature;
uint8_t frame;
uint8_t second;

const unsigned char activeSymbol[] PROGMEM = {
    B00000000,
    B00000000,
    B00011000,
    B00100100,
    B01000010,
    B01000010,
    B00100100,
    B00011000
};

const unsigned char inactiveSymbol[] PROGMEM = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00011000,
    B00011000,
    B00000000,
    B00000000
};

const char* ssid     = "SSID";
const char* password = "PASSWORD";

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, 3, 1);
OLEDDisplayUi ui ( &display );

int screenW = 128;
int screenH = 64;
int displayCenterX = screenW/2;
int displayCenterY = ((screenH-16)/2)+16;   // top yellow part is 16 px height

void displayOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {

}

void temperatureFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  String temperatureString = String(String(temperature) + " F");
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(displayCenterX + x , displayCenterY + y, temperatureString );
}

void humidityFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  String humidityString = String(String(humidity) + " %");
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(displayCenterX + x , displayCenterY + y, humidityString );
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { temperatureFrame, humidityFrame };
// how many frames are there?
int frameCount = 2;

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = { displayOverlay };
int overlaysCount = 1;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("DHT connecting ");
  dht.begin();

  Serial.println("WiFi connecting ");
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
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

  // The ESP is capable of rendering 60fps in 80Mhz mode
  // but that won't give you much time for anything else
  // run it in 160Mhz mode or just set it to 30 fps
  ui.setTargetFPS(30);
  // Customize the active and inactive symbol
  ui.setActiveSymbol(activeSymbol);
  ui.setInactiveSymbol(inactiveSymbol);
  // You can change this to
  // TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorPosition(TOP);
  // Defines where the first frame is located in the bar.
  ui.setIndicatorDirection(LEFT_RIGHT);
  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);
  // Add frames
  ui.setFrames(frames, frameCount);
  // Add overlays
  ui.setOverlays(overlays, overlaysCount);
  // Initialising the UI will init the display too.
  ui.init();
  display.flipScreenVertically();
  humidity = dht.readHumidity();
  temperature = dht.readTemperature() * 9.0f/5.0f + 32.0;
}

void loop() {
  frame++;

  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) {
    if(frame>=30)
    {
      frame=0;
      second++;
      if (second >= 60)
      {
        second = 0;
        humidity = dht.readHumidity();
      }
      else
      {
        temperature = dht.readTemperature() * 9.0f/5.0f + 32.0;
      }
    }
    else
    {
      delay(remainingTimeBudget);
    }
  }
}
