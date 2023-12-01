#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 4, 5, 2);

void setup() {
  Serial.begin(9600);

  if (!driver.init()) {
        Serial.println("init failed");
    }

}

void loop() {
  int sensorValue = analogRead(4);
  Serial.println(sensorValue);
  char msg[16];
  itoa(sensorValue, msg, 10);

  driver.send((uint8_t*)msg, strlen(msg));
    driver.waitPacketSent();
    delay(100);

}
