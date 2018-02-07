#include "SS_Flicker_Effect_Timeout.cpp"
#include "SS_Flicker_Effect_Controller.cpp"
#include <Tlc5940.h>
#include <Bounce2.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
volatile int oldButtonState = LOW;
volatile bool Latch = 0;

Bounce debouncer = Bounce();
int Button = 2;
long previousMillisCounter = 0;
long intervalCounter = 5;
long int Current_Time = 0;

int LED_Enable = 7;
int First_Boot = 0;
int PWR_LED = 4;
int Fan = 6;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MASTER_MIN_LEVEL 0
#define MASTER_MAX_LEVEL 4095
#define MASTER_FADE_INTERVAL 1000
#define LATCH_TIMEOUT_INTERVAL 600000
#define DEFAULT_MIN_REFRESH_INTERVAL 200
#define DEFAULT_MAX_REFRESH_INTERVAL 700

#define NUM_LEDS 8
Flicker_Effect_Controller leds[NUM_LEDS] = {
  Flicker_Effect_Controller(0, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  Flicker_Effect_Controller(2, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  Flicker_Effect_Controller(1, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  Flicker_Effect_Controller(3, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  Flicker_Effect_Controller(4, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  Flicker_Effect_Controller(5, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  Flicker_Effect_Controller(6, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  Flicker_Effect_Controller(7, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  //Flicker_Effect_Controller(8, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  //Flicker_Effect_Controller(9, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095),
  //Flicker_Effect_Controller(10, DEFAULT_MIN_REFRESH_INTERVAL, DEFAULT_MAX_REFRESH_INTERVAL, 4095)
};
int ledMinLevels[NUM_LEDS] = { // Minimum levels while at full brightness
  1500, //0
  1500, //1
  1500, //2
  1500,  //3
  1500,  //4
  1500,  //5
  1500,  //6
  1500,  //7
  //0,  //8
  //0,  //9
  //0,  //10
};

SIGNAL(TIMER0_COMPA_vect)
{
  Button_Press();
}



int masterFadeStartLevel;
unsigned long masterFadeStartTime;
unsigned long masterFadeEndTime;


volatile bool wasAlreadyOn;
Timeout latchTimeout;
Timeout foggerOffTimeout;
Timeout foggerOnTimeout;

int calcMasterFadeEndLevel(bool isLatchOn) // tells the fade which end it is going to, on is 4095 off 0
{
  return isLatchOn ? 4095 : 0;
}

int calcMasterFadeCurrentLevel(int masterFadeEndLevel, unsigned long currentTime)
{
  if (masterFadeStartTime == 0 & masterFadeEndTime == 0) return 0;
  if (currentTime > masterFadeEndTime) return masterFadeEndLevel;
  return masterFadeStartLevel + ((long)(masterFadeEndLevel - masterFadeStartLevel) * (long)(currentTime - masterFadeStartTime)) / (long)(masterFadeEndTime - masterFadeStartTime);
}
void setup()
{

  pinMode(Button, INPUT);
  debouncer.attach(Button);



  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  
  pinMode(LED_Enable, OUTPUT);
  pinMode(PWR_LED, OUTPUT);
  pinMode(Fan,OUTPUT);

  Tlc.init(0); // initialise TLC5940 and set all channels off
}

void loop() {

  while (First_Boot == 0) {
    digitalWrite(LED_Enable, HIGH);
    digitalWrite(PWR_LED, HIGH);
    First_Boot = 1;
  }


  // For logic consistency, these values will be the same throughout the loop:
  unsigned long time = millis();
  bool isOn = Latch;

  if (wasAlreadyOn != isOn) // Latch has just changed state
  {
    wasAlreadyOn = isOn;

    // Figure out what the current level was before the button was pressed, and make it the new starting level
    masterFadeStartLevel = calcMasterFadeCurrentLevel(calcMasterFadeEndLevel(!isOn), time);
    // Set new start and end times
    masterFadeStartTime = time;
    masterFadeEndTime = time + ((long)MASTER_FADE_INTERVAL * (long)abs(masterFadeStartLevel - calcMasterFadeEndLevel(isOn))) / 4095;

    if (isOn) { // Latch has just turned on
      latchTimeout.Reset(time + LATCH_TIMEOUT_INTERVAL);
      digitalWrite(Fan, HIGH);
    }
    else { // Latch has just turned off
      digitalWrite(Fan, LOW);
    }
  }

  if (isOn) { // If latch is currently on
#if LATCH_TIMEOUT_INTERVAL != 0
    if (latchTimeout.Tick(time)) Latch = false;
#endif
  }
  else { // If latch is currently off

  }
  UpdateLEDs(isOn, time);

}


void Button_Press() {
  debouncer.update();
  int newButtonState = debouncer.read();
  if (newButtonState == HIGH && oldButtonState == LOW) Latch = !Latch;
  oldButtonState = newButtonState;
}


void UpdateLEDs(bool isLatchOn, unsigned long currentTime) {


  int masterFadeLevel = calcMasterFadeCurrentLevel(calcMasterFadeEndLevel(isLatchOn), currentTime);


  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].SetLevels(((long)ledMinLevels[i] * masterFadeLevel) / 4095, masterFadeLevel);
    //if (DMXSerial.read(DMX_CHANNEL + 1)>230) {
      //leds[i].Update_Rock();
  //  }
    //else
    leds[i].Update();
  }
  Tlc.update();
}
