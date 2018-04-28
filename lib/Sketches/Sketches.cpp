#include <Arduino.h>
#include <ColorTransform.h>
#include <Text.h>

const int ledsPerStrip = 36;
const int ledRows = 8;

uint32_t gradient[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int offset = 0;
int color1At = 0;
uint32_t color1 = 0x000000;
int color2At = 0;
uint32_t color2 = 0x000000;
int color3At = ledsPerStrip;
uint32_t color3 = 0x000000;

void resetSketch() {
  offset = 0;
  color1At = 0;
  color1 = randColor(.05);
  color2At = ledsPerStrip;
  color2 = randColor(.05);
  color3At = ledsPerStrip * 2;
  color3 = randColor(.05);
}

void rainbow(OctoWS2811 *leds) {
  int inc = 3;
  hsv wheel;
  wheel.s = 1.0;
  wheel.v = .1;

  for (int i=0; i<ledsPerStrip; i++) {
    wheel.h = (offset+i*inc) % 360;
    rgb color = hsv2rgb(wheel);
    gradient[i] = float2Color(color.r, color.g, color.b);
  }

  int totalLights = ledsPerStrip*ledRows;
  for (int i=0; i<totalLights; i++)
    leds->setPixel(i,gradient[i%ledsPerStrip]);
  leds->show();

  offset+=inc;
  delay(40);
}

void fadeOut(OctoWS2811 *leds) {
  rgb c;
  float fadeRate = .9;
  uint32_t color;
  int totalLights = ledsPerStrip*ledRows;
  for (int i=0; i<totalLights; i++) {
    color = leds->getPixel(i);
    c = colorToRGB(color);
    c.r *= fadeRate;
    c.g *= fadeRate;
    c.b *= fadeRate;
    leds->setPixel(i,float2Color(c.r, c.g, c.b));
  }
  leds->show();
  delay(30);
}

void colorBands(OctoWS2811 *leds) {
  for (int i=0; i<ledsPerStrip-1; i++) {
    gradient[i] = gradient[i+1];
  }
  if (random(10) == 0) {
    gradient[ledsPerStrip-1] = randColor(.1);
  }
  int totalLights = ledsPerStrip*ledRows;
  for (int i=0; i<totalLights; i++)
    leds->setPixel(i,gradient[i%ledsPerStrip]);
  leds->show();
  delay(50);
}

void colorBands2(OctoWS2811 *leds) {
  uint32_t color = randColor(.1);

  for (int i=0; i<ledsPerStrip; i++) {
    gradient[i] = color;

    if (random(10) == 0) {
      color = randColor(.1);
    }
  }
  int totalLights = ledsPerStrip*ledRows;
  for (int i=0; i<totalLights; i++)
    leds->setPixel(i,gradient[i%ledsPerStrip]);
  leds->show();

  delay(200);
}

void dots(OctoWS2811 *leds) {
  int totalLights = ledsPerStrip*ledRows;
  for (int i=0; i<totalLights; i++) {
    if (random(20) == 0)
      leds->setPixel(i,randColor(.1));
    if (random(30) == 0)
      leds->setPixel(i,0);
  }
  leds->show();

  delay(100);
}

void text(OctoWS2811 *leds, const char* str) {
  int characters = strlen(str);

  // calculate the background color of each column from the gradient
  for (int i=0; i<ledsPerStrip; i++) {
    if (offset + ledsPerStrip >= color3At) {
      color1 = color2;
      color1At = color2At;
      color2 = color3;
      color2At = color3At;
      color3 = randColor(.05);
      color3At = offset+2*ledsPerStrip+random(0,200);
    }
    if (offset+i >= color2At) {
      gradient[i] =
        (map(offset+i, color2At, color3At, color2 >> 16, color3 >> 16) << 16) +
        (map(offset+i, color2At, color3At, (color2 >> 8) & 0xff, (color3 >> 8) & 0xff) << 8) +
        map(offset+i, color2At, color3At, color2 & 0xff, color3 & 0xff);
    } else {
      gradient[i] =
        (map(offset+i, color1At, color2At, color1 >> 16, color2 >> 16) << 16) +
        (map(offset+i, color1At, color2At, (color1 >> 8) & 0xff, (color2 >> 8) & 0xff) << 8) +
        map(offset+i, color1At, color2At, color1 & 0xff, color2 & 0xff);
    }
  }

  // fill background with gradient colors
  int totalLights = ledsPerStrip*ledRows;
  for (int i=0; i<totalLights; i++)
    leds->setPixel(i,gradient[i%ledsPerStrip]);

  // write text shifted by offset.
  for (int i=0; i<characters; i++) {
    int charStart = (i+6)*6 - (offset/3 % ((characters+6)*6));
    if (charStart > -6 and charStart < ledsPerStrip) {
      draw_character(leds, ledsPerStrip, charStart, str[i], 0x303030);
    }
  }
  leds->show();

  offset++;
  delay(10);
}
