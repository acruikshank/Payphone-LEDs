
#include "Text.h"
#include <Arduino.h>

uint32_t LOW_CHARS[] = {0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,0L,138547332L,10570L,3198516554L,3907164100L,1413622450L,1917912358L,2114L,68192408L,285745283L,1576489984L,166858752L,201326592L,32505856L,0L,1145184784L,1662567982L,2216757346L,1145324078L,1661420078L,301245737L,1627882559L,1659930158L,2286166559L,1662469678L,568903214L,67174400L,2214658048L,68227328L,235110400L,2290223136L,138691118L,2069755438L,1677247812L,1662502447L,1645266478L,1662567983L,1123058751L,1123058751L,1670432302L,1677248049L,2216757319L,1661485584L,1244763441L,1108378657L,1666903921L,1935330929L,1662567982L,1123599919L,1662567982L,1257817647L,1652770350L,138547359L,1662567985L,346375729L,4149921329L,1413622321L,138750513L,1145184799L,2216757326L,272762913L,277094670L,10692L,0L,8322L,1671102464L,3812000801L,1125580800L,1671119376L,1595342848L,2222000716L,1025030144L,1664517153L,2217740288L,1212285056L,1179943969L,2216757314L,1800765440L,1664516096L,1662451712L,1729737728L,3005011968L,1127645184L,471793664L,139432064L,1662550016L,2501410816L,1796767744L,2292744192L,756580352L,2291105792L};
uint8_t HIGH_CHARS[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,82,35,77,178,0,193,25,5,1,17,0,16,8,116,56,248,116,66,116,116,16,116,132,0,0,65,0,8,32,116,140,124,116,124,248,8,116,140,56,116,138,248,140,140,116,8,246,138,116,33,116,33,142,140,33,248,112,132,114,0,248,0,182,108,116,182,112,16,116,140,56,57,73,16,141,140,116,11,133,8,124,33,182,34,85,138,50,248};

void draw_character(OctoWS2811 *leds, int ledsPerStrip, int x, uint8_t c, int color) {
  uint32_t low = LOW_CHARS[c];
  uint8_t high = HIGH_CHARS[c];

  for (int i=0; i<32; i++) {
    int xOff = x + i%5;
    if (xOff >= 0 && xOff < ledsPerStrip && (low >> i) & 0x1) {
      leds->setPixel(ledsPerStrip * (i/5) + xOff, color);
    }
  }
  for (int i=0; i<8; i++) {
    int xOff = x + (i+32)%5;
    if (xOff >= 0 && xOff < ledsPerStrip && (high >> i) & 0x1) {
      leds->setPixel(ledsPerStrip * ((i+32)/5) + xOff, color);
    }
  }
}