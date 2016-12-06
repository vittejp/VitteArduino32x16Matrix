#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);

const char str[] PROGMEM = "Merry X'mas, vitte";
int    textX   = matrix.width(),
       textMin = sizeof(str) * -12,
       hue     = 0;
int8_t ball[3][4] = {
  {  3,  0,  0, 1 }, // Initial X,Y pos & velocity for 3 bouncy balls
  { 17, 15,  0, 1 },
  { 27,  4,  0, 1 }
};

static const uint16_t PROGMEM ballcolor[3] = {
  0x1082, // Green=1
  0x1082, // Blue=1
  0x1082  // Red=1
};

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setTextSize(2);
}

void loop() {
  byte i;

  matrix.fillScreen(0);

  // snow
  for(i=0; i<3; i++) {
    matrix.fillCircle(ball[i][0], ball[i][1], 1, pgm_read_word(&ballcolor[i]));
    // Update X, Y position
    ball[i][0] += ball[i][2];
    ball[i][1] += ball[i][3];

    if((ball[i][0] == 0) || (ball[i][0] == (matrix.width() - 1)))
      ball[i][2] *= -1;
    if((ball[i][1] > matrix.height() - 1))
    {
      ball[i][1] = 0;
      ball[i][0] = random(0, 32);
    }
  }

  // Draw big scrolly text on top
  matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
  matrix.setCursor(textX, 1);
  matrix.print(F2(str));

  // Move text left (w/wrap), increase hue
  if((--textX) < textMin) textX = matrix.width();
  hue += 7;
  if(hue >= 1536) hue -= 1536;

  // Update display
  matrix.swapBuffers(false);

  delay(50);
}
