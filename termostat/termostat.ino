/*
   Author: Tino Herljević
   Creation date: 23.10.2017
   Last modified: 24.10.2017.
*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(11, 12, 5, 6, 7, 8);

int relay = 4;
int downBtn = 2;
int upBtn = 3;
volatile int upPressed, downPressed = 0;
double defaultTemp = 22.00;
double tempStep = 0.50;
double currentTemp, setTemp, average;
unsigned long oldStamp = 0;
const long interval = 15000;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  pinMode(downBtn, INPUT_PULLUP);
  pinMode(upBtn, INPUT_PULLUP);
  setTemp = defaultTemp;
  sensors.begin();
  attachInterrupt(0, selectISR, LOW);
  attachInterrupt(1, selectISR, LOW);
}

void loop() {

  //check temperature every 15 seconds
  unsigned long currentStamp = millis();
  if (currentStamp - oldStamp >= interval) {
    oldStamp = currentStamp;
    getTemperature();
  }
}

void selectISR() {
  upPressed = digitalRead(upBtn);
  downPressed = digitalRead(downBtn);

  if (upPressed == LOW) {
    setTemp = setTemp + tempStep;
    delay(300);
    getTemperature();
  }
  else if (downPressed == LOW) {
    setTemp = setTemp - tempStep;
    delay(300);
    getTemperature();
  }

}

void getTemperature() {
  currentTemp = 0.0;
  average = 0.0;
  for (int i = 0; i < 3; i++) {
    sensors.requestTemperatures();
    average += sensors.getTempCByIndex(0);
    delay(10);
  }
  currentTemp = average / 3.0;
  compareTemperature();
}

void compareTemperature() {
  if (setTemp < currentTemp) {
    turnOff();
  }
  if (setTemp > currentTemp) {
    turnOn();
  }
}


void turnOn() {
  digitalWrite(relay, HIGH);
}

void turnOff() {
  digitalWrite(relay, LOW);
}

