#include <Arduino.h>
#include <ColorTransform.h>
#include <Text.h>

const int ledsPerStrip = 36;
const int ledRows = 8;
const int totalLights = ledsPerStrip * ledRows;

uint32_t gradient[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int offset = 0;
int color1At = 0;
uint32_t color1 = 0x000000;
int color2At = 0;
uint32_t color2 = 0x000000;
int color3At = ledsPerStrip;
uint32_t color3 = 0x000000;

const float blurKernel[] = { 1.0, 4.0, 1.0, 4.0, 16.0, 4.0, 1.0, 2.0, 1.0 };

void chromeBlur(OctoWS2811 *leds, uint32_t next[]);

void resetSketch() {
  offset = 0;
  color1At = 0;
  color1 = 0x000000;
  color2At = ledsPerStrip;
  color2 = randColor(.04);
  color3At = ledsPerStrip * 2;
  color3 = randColor(.1);
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

float rgbAdd(rgb *color, uint32_t other, float weight) {
  rgb otherRgb = colorToRGB(other);
  color->r += weight * otherRgb.r;
  color->g += weight * otherRgb.g;
  color->b += weight * otherRgb.b;
  return weight;
}

void effect(OctoWS2811 *leds, const float kernel[]) {
  uint32_t next[totalLights];
  rgb color;
  for (int i=0; i<ledsPerStrip; i++) {
    for (int j=0; j<ledRows; j++) {
      float weight = 0.0;
      color.r = color.g = color.b = 0.0;
      if (i > 1) {
        if (j > 1)
          weight += rgbAdd(&color, leds->getPixel((i-1) + (j-1)*ledsPerStrip), kernel[0]);
        weight += rgbAdd(&color, leds->getPixel((i-1) + j*ledsPerStrip), kernel[1]);
        if (j < ledRows-1)
          weight += rgbAdd(&color, leds->getPixel((i-1) + (j+1)*ledsPerStrip), kernel[2]);
      }
      if (j > 1) weight += rgbAdd(&color, leds->getPixel(i + (j-1)*ledsPerStrip), kernel[3]);
      weight += rgbAdd(&color, leds->getPixel(i + j*ledsPerStrip), kernel[4]);
      if (j < ledRows-1) weight += rgbAdd(&color, leds->getPixel(i + (j+1)*ledsPerStrip), kernel[5]);
      if (i < ledsPerStrip - 1) {
        if (j > 1)
          weight += rgbAdd(&color, leds->getPixel((i+1) + (j-1)*ledsPerStrip), kernel[6]);
        weight += rgbAdd(&color, leds->getPixel((i+1) + j*ledsPerStrip), kernel[7]);
        if (j < ledRows-1)
          weight += rgbAdd(&color, leds->getPixel((i+1) + (j+1)*ledsPerStrip), kernel[8]);
      }

      next[i + j*ledsPerStrip] = float2Color(color.r/weight, color.g/weight, color.b/weight);
    }
  }

  for (int i=0; i<totalLights; i++)
    leds->setPixel(i, next[i]);

  leds->show();
  delay(20);
}

void blur(OctoWS2811 *leds) {
  effect(leds, blurKernel);
}

void rgbAdd(rgb *color, uint32_t other, float rw, float gw, float bw) {
  rgb otherRgb = colorToRGB(other);
  color->r += rw * otherRgb.r;
  color->g += gw * otherRgb.g;
  color->b += bw * otherRgb.b;
}

void redblue(OctoWS2811 *leds) {
  uint32_t next[totalLights];
  chromeBlur(leds, next);

  for (int i=0; i<totalLights; i++) {
    if (random(200) == 0)
      leds->setPixel(i, 0x303030);
    else
      leds->setPixel(i, next[i]);
  }

  leds->show();
  delay(20);
}

void colorBands(OctoWS2811 *leds) {
  for (int i=0; i<ledsPerStrip-1; i++) {
    gradient[i] = gradient[i+1];
  }
  if (random(10) == 0) {
    gradient[ledsPerStrip-1] = randColor(.1);
  }
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
  for (int i=0; i<totalLights; i++)
    leds->setPixel(i,gradient[i%ledsPerStrip]);
  leds->show();

  delay(200);
}

void dots(OctoWS2811 *leds) {
  for (int i=0; i<totalLights; i++) {
    if (random(20) == 0)
      leds->setPixel(i,randColor(.1));
    if (random(30) == 0)
      leds->setPixel(i,0);
  }
  leds->show();

  delay(100);
}

void glowtext(OctoWS2811 *leds, const char* str) {
  int characters = strlen(str);
  int onCycles = 75;
  int offCycles = 50;
  int phaseOffset = offset % (onCycles + offCycles);

  if (phaseOffset < onCycles) {
    rgb background = colorToRGB(0x000000);
    rgb foreground = colorToRGB(0x303030);

    float fade = ((float) phaseOffset) / onCycles;
    uint32_t fadeBackground = float2Color(background.r*fade, background.g*fade, background.b*fade);
    uint32_t fadeForeground = float2Color(foreground.r*fade, foreground.g*fade, foreground.b*fade);

    // fill background with gradient colors
    for (int i=0; i<totalLights; i++) {
      leds->setPixel(i, fadeBackground);
    }

    // write text shifted by offset.
    int offset = (ledsPerStrip - characters*6 + 1) / 2;
    for (int i=0; i<characters; i++) {
      int charStart = i*6 + offset;
      if (charStart > -5 && charStart < ledsPerStrip) {
        draw_character(leds, ledsPerStrip, charStart, str[i], fadeForeground);
      }
    }
  } else {
    uint32_t next[totalLights];
    chromeBlur(leds, next);

    for (int i=0; i<totalLights; i++)
      leds->setPixel(i, next[i]);
  }

  leds->show();

  offset++;
  delay(20);
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
      color3 = randColor(.1);
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
  for (int i=0; i<totalLights; i++) {
    leds->setPixel(i,gradient[i%ledsPerStrip]);
  }

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

void chromeBlur(OctoWS2811 *leds, uint32_t next[]) {
  float attenuate = .999;
  rgb color;
  for (int i=0; i<ledsPerStrip; i++) {
    for (int j=0; j<ledRows; j++) {
      float redWeight = 0.0;
      float blueWeight = 0.0;
      float greenWeight = 0.0;
      color.r = color.g = color.b = 0.0;
      if (i > 1) {
        rgbAdd(&color, leds->getPixel((i-1) + j*ledsPerStrip), 6.0, 4.0, 2.0);
        redWeight += 6.0;
        greenWeight += 4.0;
        blueWeight += 2.0;
      }
      rgbAdd(&color, leds->getPixel(i + j*ledsPerStrip), 16.0, 16.0, 16.0);
      redWeight += 16.0;
      greenWeight += 16.0;
      blueWeight += 16.0;
      if (i < ledsPerStrip - 1) {
        rgbAdd(&color, leds->getPixel((i+1) + j*ledsPerStrip), 2.0, 4.0, 6.0);
        redWeight += 2.0;
        greenWeight += 4.0;
        blueWeight += 6.0;
      }

      next[i + j*ledsPerStrip] = float2Color(color.r/redWeight*attenuate, color.g/greenWeight*attenuate, color.b/blueWeight*attenuate);
    }
  }
}
