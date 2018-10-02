
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"

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

// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, 3, 1);
OLEDDisplayUi ui ( &display );

int screenW = 128;
int screenH = 64;
int displayCenterX = screenW/2;
int displayCenterY = ((screenH-16)/2)+16;   // top yellow part is 16 px height

void displayOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
}

void frame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  String frame1String = String("Frame: " + String(1));
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(displayCenterX + x , displayCenterY + y, frame1String );
}

void frame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  String frame2String = String("Frame: " + String(2));
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_24);
  display->drawString(displayCenterX + x , displayCenterY + y, frame2String );
}

// This array keeps function pointers to all frames
// frames are the single views that slide in
FrameCallback frames[] = { frame1, frame2 };
// how many frames are there?
int frameCount = 2;

// Overlays are statically drawn on top of a frame eg. a clock
OverlayCallback overlays[] = { displayOverlay };
int overlaysCount = 1;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("Starting up");

  // The ESP is capable of rendering 60fps in 80Mhz mode
  // but that won't give you much time for anything else
  // run it in 160Mhz mode or just set it to 30 fps
  ui.setTargetFPS(60);
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
}

void loop() {
  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) {
      delay(remainingTimeBudget);
  }
}
