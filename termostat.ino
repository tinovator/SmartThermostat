#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
void setup() {
  int relay = 3;
  sensors.begin();


}

void loop() {
  // put your main code here, to run repeatedly:

}
