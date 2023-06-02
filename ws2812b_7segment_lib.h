#include <Adafruit_NeoPixel.h>
#define PIN 2
#define LED_NUMBER 30
const byte BOX_NUMBER = 5;
int c = 0;

#ifdef LED_NUMBER 30
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_NUMBER, PIN, NEO_RGB + NEO_KHZ800);
const byte zero[] = {7, 255, 255, 255, 0, 255, 255, 255};
const byte one[] = {7, 0, 0, 255, 0, 0, 0, 255};
const byte two[] = {7, 0, 255, 255, 255, 255, 255, 0};
const byte three[] = {7, 0, 255, 255, 255, 0, 255, 255};
const byte four[] = {7, 255, 0, 255, 255, 0, 0, 255};
const byte five[] = {7, 255, 255, 0, 255, 0, 255, 255};
const byte six[] = {7, 255, 255, 0, 255, 255, 255, 255};
const byte seven[] = {7, 0, 255, 255, 0, 0, 0, 255};
const byte eught[] = {7, 255, 255, 255, 255, 255, 255, 255};
const byte nine[] = {7,  255, 255, 255, 255, 0, 255, 255};
const byte colon[] = {1, 255, 255};
const byte* font[] = {zero, one, two, three, four, five, six, seven, eught, nine, colon};
const byte box_sizes[] = {7, 7, 2, 7, 7};
#endif /* LED_NUMBER_30 */

byte* getMask(char symbol) {
  byte* mask;
  switch (symbol) {
    case '0':
      mask = (byte*)font[0];
      break;

    case '1':
      mask = (byte*)font[1];
      break;

    case '2':
      mask = (byte*)font[2];
      break;

    case '3':
      mask = (byte*)font[3];
      break;


    case '4':
      mask = (byte*)font[4];
      break;

    case '5':
      mask = (byte*)font[5];
      break;

    case '6':
      mask = (byte*)font[6];
      break;

    case '7':
      mask = (byte*)font[7];
      break;

    case '8':
      mask = (byte*)font[8];
      break;

    case '9':
      mask = (byte*)font[9];
      break;

    case ':':
      mask = (byte*)font[10];
      break;   
  }
  return mask;
}

void showSymbol(int pos, char symbol, byte G, byte R, byte B) {

  byte* mask = getMask(symbol);
  byte mask_size = box_sizes[pos];
  int smeshc = 0;
  for (byte i = 0; (i < pos) && (i < BOX_NUMBER); i++) {
    smeshc += box_sizes[i];
  }
  for (byte i = 1; i <= mask_size; i++) {
    byte pixelColor = mask[i];
    pixels.setPixelColor(smeshc + i - 1, pixels.Color(R * (pixelColor / 255.0), G * (pixelColor / 255.0), B * (pixelColor / 255.0)));
  }
}

void showString(int pos, String text, byte G, byte R, byte B ) {
  for (int i = 0; i < text.length(); i++) {
    showSymbol(pos + i, text[i],  G, R, B);
  }
  pixels.show();
}
