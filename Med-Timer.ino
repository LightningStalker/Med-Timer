/*
  Med Timer
  Sets timer for medicine

  23 Aug 2017
  by The Lightning Stalker
*/

#include "avr/interrupt.h"
#include "avr/power.h"
#include "avr/sleep.h"
//#define DEBUG

// constants won't change. They're used here to
// set pin numbers:
const int ledPin = LED_BUILTIN;         // the number of the LED pin
const int buttonPin = 12;               // pushbutton pin
const int buzzPin = 4;                  // buzzer
const unsigned long interval = 200;     // alert speed, must be a divisor of 1000
const unsigned long runTime2m = 1000 / interval * 60; // minutes conversion factor

void hours( unsigned long timeh ) { // delay wrapper that waits timeh hours
  delay(timeh * 1000 * 60 * 60);
  return;
}

void minutes( unsigned long timem ) { // same as hours for waiting timem minutes
  delay(timem * 1000 * 60);
  return;
}

void seconds( unsigned long times ) { // same as above for seconds
  delay(times * 1000);
  return;
}

unsigned long alert() { // slightly modified Blink Without Delay code
  unsigned long previousMillis = 0;
  unsigned long runTime = 0;         // time in seconds since alert began
  int alertState = LOW;

  while(true) {
    if (digitalRead(buttonPin) == LOW) {
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzPin, LOW);
      break;
    }

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      // save the time alert changes state
      previousMillis = currentMillis;

      // if the LED/buzzer is off turn it on and vice-versa
      if (alertState == LOW) {
        alertState = HIGH;
      } else {
        alertState = LOW;
      }

      // set the LED and buzzer with the state of alertState:
      digitalWrite(ledPin, alertState);
      digitalWrite(buzzPin, alertState);
      runTime++;
    }
  }
  return runTime / runTime2m; // return alert runtime in minutes
}

void beep( bool pause = true ) { // short confirmation beep/flash, pause by default
  if(pause) seconds(1);          // optional delay in case of switch bounce
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzPin, LOW);
  return;
}

// the setup function runs once when you press reset or power the board
void setup() {
  #ifdef DEBUG
  Serial.begin(9600);
  while(!Serial) {
    // waiting and waiting and waiting
  }
  #endif
  // initialize the LED pin as an output.
  pinMode(ledPin, OUTPUT);
  // initialize the buzzer pin as an output.
  pinMode(buzzPin, OUTPUT);
  // initialize the pushbutton pin as an weak pullups:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  unsigned long error = 0;      // accumulated runtime of all alerts

  // initial flash/buzz to notify user of startup
  beep(false);                  // no delay since Arduino Micro already has one

  // morning dose, begin timing
  hours(4);
  error = alert();              // 4-hour Medicine
  if(error > 60) error = 60;
  beep();                       // beep confirming start of next timing cycle

  minutes(60 - error);
  error = alert();              // 5-hour Medicine
  if(error > 180) error = 180;
  beep();

  minutes(180 - error);
  alert();                      // 4-hour Medicine
  beep();

  minutes(240);
  alert();                      // 4-hour Medicine

  // time to go to sleep
  noInterrupts();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  while(true) { // enter infinite loop
    sleep_mode();
  }
}
