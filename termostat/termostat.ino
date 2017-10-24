/*
   Author: Tino Herljević
   Creation date: 23.10.2017
   Last modified: 24.10.2017.
*/
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int relay = 4;
int downBtn = 2;
int upBtn = 3;
int upPressed, downPressed;
double defaultTemp = 22.00;
double tempStep = 0.50;
double currentTemp, setTemp, average;

void setup() {
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  pinMode(downBtn, INPUT_PULLUP);
  pinMode(upBtn, INPUT_PULLUP);
  setTemp = defaultTemp;
  sensors.begin();
}

void loop() {
  upPressed = digitalRead(upBtn);
  downPressed = digitalRead(downBtn);

  if (upPressed == LOW) {
    setTemp = setTemp + tempStep;
  }
  else if (downPressed == LOW) {
    setTemp = setTemp - tempStep;
  }


}

void compareTemp() {
  if (setTemp < currentTemp) {
    turnOff();
  }

  else if (setTemp > currentTemp) {
    turnOn();
  }
}

void getTemperature() {
  currentTemp = 0.0;
  average = 0.0;
  for (int i = 0; i < 3; i++) {
    sensors.requestTemperatures();
    average += sensors.getTempCByIndex(0);
    delay(50);
  }
  currentTemp = average / 3.0;
}

void turnOn() {
  digitalWrite(relay, HIGH);
}

void turnOff() {
  digitalWrite(relay, LOW);
}
