#include <Arduino.h>
#include <OctoWS2811.h>
#include <Sketches.h>

const int ledsPerStrip = 36;
char str[200] =  "";

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];
int state = 5;

const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup() {
  leds.begin();
  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    int byte = Serial.read();
    state = abs(byte-48) % 6;
    resetSketch();

    if (state == 0) {
      Serial.readBytesUntil('\n', str, 200);
    }
  }

  switch (state) {
    case 0: return text(&leds, str);
    case 1: return rainbow(&leds);
    case 2: return colorBands(&leds);
    case 3: return colorBands2(&leds);
    case 4: return dots(&leds);
    case 5: return fadeOut(&leds);
  }
}
