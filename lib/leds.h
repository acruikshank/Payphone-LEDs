#define LED_HEIGHT 8
#define LED_WIDTH = 32
#define pixelIndex(x, y) ((x)*LED_HEIGHT + ((x)&1 ? LED_HEIGHT - (y) - 1 : (y)))
#define ledWrite(x, y, color) (leds->setPixel(pixelIndex(x,y), color))
#define ledRead(x, y) (leds->getPixel(pixelIndex(x, y)))
