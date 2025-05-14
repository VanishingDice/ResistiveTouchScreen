/*

Jimmy Zhang
2025.5.14
V0.1.0
*/

#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library

#include "ResistiveTouchScreen.h"

//For screen touching pin
#define XM A0 //X-
#define YM A1 //Y-
#define XP A2 //X+
#define YP A3 //Y+
#define SCREEN_ROTATION 0
#define SCREEN_XPIXELS 480
#define SCREEN_YPIXELS 320

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

// ResistiveTouchScreen touch = ResistiveTouchScreen();
ResistiveTouchScreen touch = ResistiveTouchScreen(XM, YM, XP, YP, SCREEN_XPIXELS, SCREEN_YPIXELS, SCREEN_ROTATION);

void setup(void) {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(SCREEN_ROTATION);

  tft.fillScreen(tft.color565(0, 0, 0));

  // tft.fillRect(100, 100, 100, 50, tft.color565(200, 100, 100));

  // tft.fillRect(300, 100, 100, 50, tft.color565(100, 200, 100));

}

void loop() {
  uint16_t x;
  uint16_t y;

  // touch.getTouchingPoint(&x, &y);

  // if(x > 100 && x < 200 && y > 100 && y < 150) {
  //   tft.fillRect(100, 100, 100, 50, tft.color565(150, 100, 100));
  //   delay(300);
  //   tft.fillRect(100, 100, 100, 50, tft.color565(200, 100, 100));
  // }
  // if(x > 300 && x < 400 && y > 100 && y < 150) {
  //   tft.fillRect(300, 100, 100, 50, tft.color565(100, 150, 100));
  //   delay(300);
  //   tft.fillRect(300, 100, 100, 50, tft.color565(100, 200, 100));
  // }

  drawTouchingPoint();

  // delay(100);

  // wait();

}

void drawTouchingPoint() {
  // tft.fillScreen(tft.color565(0, 0, 0));
  int16_t x = -1;
  int16_t y = -1;
  touch.getTouchingPoint(&x, &y);
  // Serial.println("X:"+String(x)+", Y:"+String(y));
  tft.drawPixel(x, y, tft.color565(0, 255, 0)); 
  // delay(1000);
}