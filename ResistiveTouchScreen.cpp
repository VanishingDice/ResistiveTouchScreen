/*

Jimmy Zhang
2025.5.14
V0.1.0
*/

#include "ResistiveTouchScreen.h"

ResistiveTouchScreen::ResistiveTouchScreen(uint8_t XM, uint8_t YM, uint8_t XP, uint8_t YP, uint16_t screenXPixels, uint16_t screenYPixels, uint8_t rotation) {
    _XM = XM;
    _YM = YM;
    _XP = XP;
    _YP = YP;
    _screenXPixels = screenXPixels;
    _screenYPixels = screenYPixels;
    _rotation = rotation;
}

void ResistiveTouchScreen::getTouchingPoint(uint16_t* x, uint16_t* y) {
  int16_t xTmp[_filterCycles];
  int16_t yTmp[_filterCycles];
  for(int i = 0; i < _filterCycles; ++i) {
    while(1) {
      getRawTouchingPoint(&xTmp[i], &yTmp[i]);
      if((xTmp[i] == -1) || (yTmp[i] == -1)) { 
        i = 0;
        continue;//when the finger left the screen, restart count
      }
      if(i == 0) {
        break;//if it's the first data
      } else if((abs(xTmp[i-1]-xTmp[i]) + abs(yTmp[i-1]-yTmp[i])) <= _filterMaxAllowableDrift) {
        break;//if coming data is in the filter limit
      }
    }
  }

  *x = 0;
  *y = 0;

  for(int i = 0; i < _filterCycles; ++i) {
    *x += xTmp[i];
    *y += yTmp[i];
  }

  *x /= _filterCycles;
  *y /= _filterCycles;

}

void ResistiveTouchScreen::getTouchingPointNoBlocking(uint16_t* x, uint16_t* y) {
  int16_t xTmp[_filterCycles];
  int16_t yTmp[_filterCycles];
  for(int i = 0; i < _filterCycles; ++i) {
    getRawTouchingPoint(&xTmp[i], &yTmp[i]);
    if((xTmp[i] == -1) || (yTmp[i] == -1)) { 
      x = -1;
      y = -1;
      return;
    }
    if(i != 0 && ((abs(xTmp[i-1]-xTmp[i]) + abs(yTmp[i-1]-yTmp[i])) > _filterMaxAllowableDrift)) {
      x = -1;
      y = -1;
      return;
    }
  }

  *x = 0;
  *y = 0;

  for(int i = 0; i < _filterCycles; ++i) {
    *x += xTmp[i];
    *y += yTmp[i];
  }

  *x /= _filterCycles;
  *y /= _filterCycles;

}

void ResistiveTouchScreen::getRawTouchingPoint(int16_t* x, int16_t* y) {
  int16_t xRaw = -1;
  int16_t yRaw = -1;
  pinMode(_YP, OUTPUT);
  pinMode(_YM, OUTPUT);
  pinMode(_XP, INPUT_PULLUP);
  pinMode(_XM, INPUT_PULLUP);
  digitalWrite(_YP, HIGH);
  digitalWrite(_YM, LOW);
  int16_t tmp;
  analogRead(_XP);//The first time to do the analogRead will cause a wrong result, don't know why, it should be 101x, but it's between 940 and 1000 at first time
  tmp = analogRead(_XP);
  // Serial.print("xTmp+:"+String(tmp));
  // Serial.print(", xTmp-:"+String(analogRead(XM)));
  if(tmp > 1000) {
    xRaw = -1;
  } else {
    xRaw = ((long)tmp*_screenXPixels/1024+_xShift)*_xRatio;
  }

  pinMode(_XP, OUTPUT);
  pinMode(_XM, OUTPUT);
  pinMode(_YP, INPUT_PULLUP);
  pinMode(_YM, INPUT_PULLUP);
  digitalWrite(_XP, HIGH);
  digitalWrite(_XM, LOW);
  analogRead(_YP);//Same to this
  tmp = analogRead(_YP);
  // Serial.print(", yTmp+:"+String(tmp));
  // Serial.println(", yTmp-:"+String(analogRead(YM)));
  if(tmp > 1000) {
    yRaw = -1;
  } else {
    yRaw = ((long)tmp*_screenYPixels/1024+_yShift)*_yRatio;
  }

  rawToScreenCoordinate(xRaw, yRaw, x, y);
}

void ResistiveTouchScreen::rawToScreenCoordinate(int16_t xRaw, int16_t yRaw, int16_t* x, int16_t* y) {
  switch(_rotation) {
    case 0:
      *x = yRaw;
      *y = xRaw;
      break;
    case 1:
      *x = xRaw;
      *y = _screenYPixels - yRaw;
      break;
    case 2:
      *x = _screenYPixels - yRaw;
      *y = _screenXPixels - xRaw;
      break;
    case 3:
      *x = _screenXPixels - xRaw;
      *y = yRaw;
      break;
  }

}

void ResistiveTouchScreen::setFilter(uint8_t filterCycles, uint8_t filterMaxAllowableDrift) {
    _filterCycles = filterCycles;
    _filterMaxAllowableDrift = filterMaxAllowableDrift;
}

void ResistiveTouchScreen::setCorrection(int xShift, int yShift, float xRatio, float yRatio) {
    _xShift = xShift;
    _yShift = yShift;
    _xRatio = xRatio;
    _yRatio = yRatio;
}


