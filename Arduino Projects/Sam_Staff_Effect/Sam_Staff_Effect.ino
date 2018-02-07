#include "Color_Struct.h"
#include "Adafruit_NeoPixel.h"
#define LEDPIN 6 // connect the Data from the strip to this pin on the Arduino
#define NUMBER_PIEXELS 40 // the number of pixels in your LED strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_PIEXELS, LEDPIN, NEO_GRB + NEO_KHZ800);

int waitTime = 5;
int pause = 500;
int StaffBrightlevel = 40;
int StaffBright = StaffBrightlevel;

void setup() {
  strip.begin();
}



void loop() {
  Color red = Color(255, 0, 0), orange = Color(255,128,0), yellow = Color(255,200,0), blank = Color(0,0,0), dimred = Color(63, 0, 0), midred = Color(126, 0, 0), dimorange = Color(63, 32, 0), midorange = Color(126, 64, 0), dimyellow = Color(64, 30, 0), midyellow = Color(128, 50, 0);

if (millis()<200) {
  performStripFade(blank, red, waitTime);

  }

  performStripFade(red, orange, waitTime);
  performStripFade(orange, dimyellow, waitTime);
  brighter();
  performStripFade(dimyellow, dimred, waitTime);
  performStripFade(dimred, red, waitTime);
  performStripFade(red, midorange, waitTime);
  performStripFade(midorange, dimred, waitTime);
  dimmer();
  performStripFade(dimred, orange, waitTime);
  performStripFade(orange, midyellow, waitTime);
  performStripFade(midyellow, red, waitTime);


  

}


Color crossfade(Color color1, Color color2, byte position) {
  return Color(
    color1.Red + ((int)color2.Red - color1.Red) * position / 255,

    color1.Green + ((int)color2.Green - color1.Green) * position / 255,
    color1.Blue + ((int)color2.Blue - color1.Blue) * position / 255);
}
Color crossfadeg(Color start, Color end, float position, float gamma) {
  position /= 255;
  float powStartRed = pow(start.Red / 255.0, gamma), powStartGreen = pow(start.Green / 255.0, gamma), powStartBlue = pow(start.Blue, gamma);
  return Color(
    pow(powStartRed + (pow(end.Red / 255.0, gamma) - powStartRed) * position, 1 / gamma) * 255,
    pow(powStartGreen + (pow(end.Green / 255.0, gamma) - powStartGreen) * position, 1 / gamma) * 255,
    pow(powStartBlue + (pow(end.Blue / 255.0, gamma) - powStartBlue) * position, 1 / gamma) * 255);
}

void performStripFade(Color color1, Color color2, byte wait) {
  for (byte position = 0; position < 255; position++) {
    setStripColor(strip, crossfadeg(color1, color2, position, .5));
    strip.show();
    if (position != 255) delay(wait); // Don't delay after the very last update because the next fade will immediately set the same color and delay after it.
  }
}

void setStripColor(Adafruit_NeoPixel& strip, Color color) {
  for (byte i = 0; i < NUMBER_PIEXELS; i++)
    strip.setPixelColor(i, color.Red, color.Green, color.Blue);
}

void brighter() {

  if (StaffBright < 100) {
    StaffBright++;
    strip.show();
  }
}
void dimmer() {

  if (StaffBright > StaffBrightlevel) {
    StaffBright--;
    strip.show();
  }
}
