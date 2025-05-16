#ifndef RESISTIVE_TOUCH_SCREEN
#define RESISTIVE_TOUCH_SCREEN

#include <Arduino.h>

class ResistiveTouchScreen {
public:
    ResistiveTouchScreen(uint8_t XM = A0, uint8_t YM = A1, uint8_t XP = A2, uint8_t YP = A3, uint16_t screenXPixels = 480, uint16_t screenYPixels = 320, uint8_t rotation = 0);
    void getRawTouchingPoint(int16_t* x, int16_t* y);
    void getTouchingPoint(uint16_t* x, uint16_t* y);
    void getTouchingPointNoBlocking(uint16_t* x, uint16_t* y);
    void setFilter(uint8_t filterCycles, uint8_t filterMaxAllowableDrift = 50);
    void setCorrection(int xShift, int yShift, float xRatio, float yRatio);

private:
    uint8_t _XM;
    uint8_t _YM;
    uint8_t _XP;
    uint8_t _YP;
    uint16_t _screenXPixels;
    uint16_t _screenYPixels;
    uint8_t _rotation;
    
    int _xShift = -50;
    int _yShift = -50;
    float _xRatio = 1.23;
    float _yRatio = 1.29;
    uint8_t _filterCycles = 5;//How many point to be average
    uint8_t _filterMaxAllowableDrift = 50;//limit length between two pixels

    void rawToScreenCoordinate(int16_t xRaw, int16_t yRaw, int16_t* x, int16_t* y);

};


#endif