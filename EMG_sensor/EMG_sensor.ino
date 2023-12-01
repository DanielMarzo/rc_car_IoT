#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(2000, 4, 5, 2);

struct Data {
  int int1;
  int int2;
} dataToSend;

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

  dataToSend.int1 = leftMotor;
  dataToSend.int2 = rightMotor;

  nrf24.send((uint8_t*)&dataToSend, sizeof(Data));

  //nrf24.send((uint8_t*)msg, strlen(msg));
  //nrf24.send((uint8_t*)leftMsg, strlen(leftMsg));
  //nrf24.send((uint8_t*)rightMsg, strlen(rightMsg));
  nrf24.waitPacketSent();

  delay(100);

}
