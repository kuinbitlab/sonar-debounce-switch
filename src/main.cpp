#include <Arduino.h>
#include <NewPing.h>
#include <SimpleTimer.h>

#define GND_PIN 7
#define VCC_PIN 6

#define LED_RED 9
#define LED_GREEN 10
#define RELAY_PIN 11

#define TRIGGER_PIN 5   
#define ECHO_PIN 4    
#define MAX_DISTANCE 40 

SimpleTimer timer;

bool sonarState;

int ledState = LOW;       // the current state of the output pin
int buttonState;           // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin

unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 100;   

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

void takeData()
{
  int distance = sonar.ping_cm();
  if (distance > 5 && distance < 30)
  {
    sonarState = HIGH;
  }
  else
  {
    sonarState = LOW;
  }
}

void pinSetup(){
  pinMode(GND_PIN, OUTPUT);
  pinMode(VCC_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(GND_PIN, LOW);
  digitalWrite(VCC_PIN, HIGH);
  digitalWrite(RELAY_PIN, HIGH);
}

void debounce()
{
  int reading = sonarState;

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;

      if (buttonState == HIGH)
      {
        ledState = !ledState;
      }
    }
  }

  digitalWrite(LED_RED, !ledState);
  digitalWrite(LED_GREEN, ledState);
  digitalWrite(RELAY_PIN, !ledState);

  lastButtonState = reading;
}

void setup()
{
  pinSetup();
  timer.setInterval(50L, takeData);
}

void loop()
{
  timer.run();
  debounce();
}