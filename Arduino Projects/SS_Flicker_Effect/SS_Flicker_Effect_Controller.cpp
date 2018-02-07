#include <Arduino.h>
#include "Tlc5940.h"

class Flicker_Effect_Controller
{
  byte channel; //The Channel of the LED output on the TLC5940

  int minLevel; //The lower bound of the random value for LED Brightness
  int maxLevel; //The upper bound of the random value for LED Brightness

  int minRefreshInterval; //The lower bound of the random value for LED refresh time
  int maxRefreshInterval; //The upper bound of the random value for LED refresh time

  int destinationLevel; //The level the LED is increasing or decreasing to
  int previousLevel;

  long previousMillis; //Used to compare the current time to how long has passed
  long interval; //The random refresh time
  long previousMicros_Smoothing; //Used to compare the current time to how long has passed for the smoothing

  int maxPossibleLevel;
  int Usermin;
 
public:
  Flicker_Effect_Controller(byte channel, int minRefreshInterval, int maxRefreshInterval, int maxPossibleLevel)
  : channel(channel),
    minLevel(0),
    maxLevel(0),
    minRefreshInterval(minRefreshInterval),
    maxRefreshInterval(maxRefreshInterval),
    destinationLevel(0),
    previousLevel(0),
    previousMillis(0),
    interval(0),
    previousMicros_Smoothing(0),
    maxPossibleLevel(maxPossibleLevel)
  {
  }

  void Update()
  {
    unsigned long currentMillis = millis(); //Saves the current millis() to be used in determining if the interval time has passed

    // Periodically change the destination level
    if (currentMillis - previousMillis > interval)
    {
      interval = random(minRefreshInterval, maxRefreshInterval); // Generates the interval of the next LED update
      previousMillis = currentMillis; //mark the time down for next time
      previousLevel = destinationLevel;
      destinationLevel = random(minLevel, maxLevel); //Generate the level that the LED will be going to
    }

    Tlc.set(channel, previousLevel + ((long)(destinationLevel - previousLevel) * (long)(currentMillis - previousMillis)) / (long)interval);
  }

  void Update_Rock()
  {
    int minRefreshInterval_Rock = 50;
    int maxRefreshInterval_Rock = 350;

    unsigned long currentMillis = millis(); //Saves the current millis() to be used in determining if the interval time has passed

                        // Periodically change the destination level
    if (currentMillis - previousMillis > interval)
    {
      interval = random(minRefreshInterval_Rock, maxRefreshInterval_Rock); // Generates the interval of the next LED update
      previousMillis = currentMillis; //mark the time down for next time
      previousLevel = destinationLevel;
      destinationLevel = random(30, maxLevel/3); //Generate the level that the LED will be going to
    }

    Tlc.set(channel, previousLevel + ((long)(destinationLevel - previousLevel) * (long)(currentMillis - previousMillis)) / (long)interval);
  }


  void SetLevels(int minLevel, int maxLevel)
  {
    this->minLevel = (long)minLevel * (long)maxPossibleLevel / 4095;
    this->maxLevel = (long)maxLevel * (long)maxPossibleLevel / 4095;
  }
};

