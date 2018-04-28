#include <Arduino.h>
#include <OctoWS2811.h>

void resetSketch();
void rainbow(OctoWS2811 *leds);
void colorBands(OctoWS2811 *leds);
void colorBands2(OctoWS2811 *leds);
void dots(OctoWS2811 *leds);
void text(OctoWS2811 *leds, const char *str);
void fadeOut(OctoWS2811 *leds);
void blur(OctoWS2811 *leds);
void redblue(OctoWS2811 *leds);
