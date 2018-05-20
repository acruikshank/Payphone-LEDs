#include <Arduino.h>
#include <OctoWS2811.h>
#include <Sketches.h>
#include <Adafruit_WS2801.h>
#include <main.h>

#define LED_WIDTH      8   // number of LEDs horizontally
#define LED_HEIGHT     32   // number of LEDs vertically (must be multiple of 8)
#define LED_LAYOUT     1    // 0 = even rows left->right, 1 = even rows right->left


const int ledsPerStrip = 256;
const int DATA_PIN = 11;
const int CLK_PIN = 13;
const unsigned int AMBI_DELAY = 40;
const unsigned int FLASH_DELAY = 50;
char str[200] =  "";

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];
int state = 7;

//ambiled setup
unsigned int ambiLedLastUpdate = 0;
unsigned char ambiLedState = 0;
bool ringing = false;
bool flashState = false;
unsigned int flashLastUpdate = 0;

const int config = WS2811_RGB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);
Adafruit_WS2801 strip = Adafruit_WS2801(20, DATA_PIN, CLK_PIN);
void setup() {
  leds.begin();
  randomSeed(analogRead(0));
  Serial.begin(9600);
  // strip.begin();

  // Update LED contents, to start they are all 'off'
  // strip.show();
  
}

void loop() {
  // if (millis() > ambiLedLastUpdate + AMBI_DELAY && !ringing) {
  //   updateAmbiLeds();
  //   ambiLedLastUpdate = millis();
  // }
  // if (millis() > flashLastUpdate + FLASH_DELAY && ringing) {
  //   if (flashState) {
  //     colorWipe(Color(255, 255, 255), 0);
  //   } else {
  //     colorWipe(Color(255, 255, 255), 0);
  //   }
  //   flashState = !flashState;
  // }

  if (Serial.available() > 0) {
    int byte = Serial.read();
    state = abs(byte-48) % 9;
    resetSketch();

    if (state == 0 || state == 8) {
      Serial.readBytesUntil('\n', str, 200);
    }
    if (state == 9) {
      ringing = true;
    } else {
      ringing = false;
    }
  }

  switch (state) {
    case 0: return text(&leds, str);
    case 1: return rainbow(&leds);
    case 2: return colorBands(&leds);
    case 3: return colorBands2(&leds);
    case 4: return dots(&leds);
    case 5: return fadeOut(&leds);
    case 6: return blur(&leds);
    case 7: return redblue(&leds);
    case 8: return glowtext(&leds, str);
  }
}

void updateAmbiLeds() {
  colorWipe(Wheel(ambiLedState), 0);
  ambiLedState++;
}



/* Helper functions */

// Create a 24 bit color value from R,G,B
unsigned int Color(unsigned char r, unsigned char g, unsigned char b)
{
  unsigned int c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
unsigned int Wheel(unsigned char WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void colorWipe(unsigned int c, unsigned char wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  strip.show();
}