/*
   Author: Tino Herljević
   Creation date: 23.10.2017
   Last modified: 24.10.2017.
*/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#define ONE_WIRE_BUS 5
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
const long interval = 5000;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  pinMode(downBtn, INPUT_PULLUP);
  pinMode(upBtn, INPUT_PULLUP);
  setTemp = defaultTemp;
  sensors.begin();

  Serial.println("Initialized everything");
}

void loop() {
  upPressed = digitalRead(upBtn);
  downPressed = digitalRead(downBtn);

  if (upPressed == LOW) {
    tempUp();
  }
  if (downPressed == LOW) {
    tempDown();
  }
  //check temperature every 15 seconds
  unsigned long currentStamp = millis();
  if (currentStamp - oldStamp >= interval) {
    oldStamp = currentStamp;
    Serial.println("Getting temperature");
    getTemperature();
    compareTemperature();
    Serial.print("currentTemp ");
    Serial.println(currentTemp);
    Serial.print("Set temperature ");
    Serial.println(setTemp);
  }
}


void tempDown() {
  setTemp = setTemp - tempStep;
  Serial.print("Step down");
  delay(500);
  getTemperature();
  compareTemperature();
  Serial.print("currentTemp ");
  Serial.println(currentTemp);
  Serial.print("Set temp ");
  Serial.println(setTemp);
}
void tempUp() {
  setTemp = setTemp + tempStep;
  Serial.print("Step up");
  delay(500);
  getTemperature();
  compareTemperature();
  Serial.print("currentTemp ");
  Serial.println(currentTemp);
  Serial.print("Set temp ");
  Serial.println(setTemp);
}
void getTemperature() {
  currentTemp = 0.0;
  average = 0.0;
  for (int i = 0; i<3; i++){
  
  sensors.requestTemperatures();
  average = average + sensors.getTempCByIndex(0);
  delay(100);
  }
  currentTemp = average / 3.0;
}

void compareTemperature() {
  if (setTemp < currentTemp) {
    Serial.println("Set LOWER than current");
    turnOff();

  }
  if (setTemp > currentTemp) {
    Serial.println("Set HIGHER than current");
    turnOn();
  }
}

void turnOn() {
  digitalWrite(relay, HIGH);
  Serial.println("Relay ON");
}

void turnOff() {
  digitalWrite(relay, LOW);
  Serial.println("Relay OFF");
}

