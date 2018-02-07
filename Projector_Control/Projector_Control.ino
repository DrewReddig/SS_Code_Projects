#include <Bounce2.h>
#include <SoftwareSerial.h>

SoftwareSerial projectorSerial(8, 9); // RX, TX
//int DebugLED = 7;

int PWR_pin = 2;
int AVMute_pin = 3;

// Instantiate a Bounce object
Bounce debouncer1 = Bounce();

// Instantiate another Bounce object
Bounce debouncer2 = Bounce();

long previousMillis_ON = 0;
long previousMillis_OFF = 0;

long interval_ON = 10000;
long interval_OFF = 10000;

void setup()
{
  pinMode(PWR_pin, INPUT);
  pinMode(AVMute_pin, INPUT);

  // After setting up the button, setup the Bounce instance :
  debouncer1.attach(PWR_pin);
  debouncer1.interval(5); // interval in ms

  // After setting up the button, setup the Bounce instance :
  debouncer2.attach(AVMute_pin);
  debouncer2.interval(5); // interval in ms

  //pinMode(DebugLED, OUTPUT);

  Serial.begin(9600);
  projectorSerial.begin(9600);

}

void loop()
{
  // Update the Bounce instances :
  debouncer1.update();
  debouncer2.update();

  if (debouncer1.read() == HIGH && debouncer1.read() == LOW) {
    unsigned long currentMillis_ON = millis();
    
    if (currentMillis_ON - previousMillis_ON > interval_ON) {

      Serial.println("PWR ON");
      projectorSerial.println("PWR ON");

      previousMillis_ON = currentMillis_ON;
    }
  }


  if (debouncer1.read() == LOW && debouncer1.read() == HIGH) {
    unsigned long currentMillis_OFF = millis();

    if (currentMillis_OFF - previousMillis_OFF > interval_OFF) {

      Serial.println("PWR OFF");
      projectorSerial.println("PWR OFF");

      previousMillis_OFF = currentMillis_OFF;
    }
  }
  
  
  
  
  /*
    if (digitalRead(AVMute_pin) == 1 && AVMute_Count > 0) {
      Serial.print("MUTE ON");
      projectorSerial.println("MUTE ON");
      digitalWrite(DebugLED, HIGH);
      delay(500);
      AVMute_Count == --AVMute_Count;
      digitalWrite(DebugLED, LOW);
    }



    if (digitalRead(AVMute_pin) == 0 && AVMute_Count < AVMute_Print_Amount) {
      Serial.print("MUTE OFF");
      projectorSerial.println("MUTE OFF");
      digitalWrite(DebugLED, HIGH);
      delay(500);
      AVMute_Count == ++AVMute_Count;
      digitalWrite(DebugLED, LOW);
    }
    */
  }
