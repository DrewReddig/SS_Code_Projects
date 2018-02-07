#include "Arduino.h"
// The struct type definition has to come before the first function in the file- it could be put in Color.h and included
struct Color {
  byte Red, Green, Blue;

  Color(byte red, byte green, byte blue)
    : Red(red), Green(green), Blue(blue)
  { }
};
