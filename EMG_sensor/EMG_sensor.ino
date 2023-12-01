
#include <RH_NRF24.h>
#include <SPI.h>
#define CE 4
#define CSN 5
#define potentiometer 26
RH_NRF24 nrf24(CE,CSN);

void setup() {
  Serial.begin(9600);

  if (!nrf24.init())
    Serial.println("init failed");
  
if (!nrf24.setChannel(22))
    Serial.println("setChannel failed");

  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
    Serial.println("setRF failed");

}

void loop() {
  int sensorValue = analogRead(25);

  int potValue = analogRead(potentiometer);
  int motorPercentage = map(potValue, 0, 4095, 0, 100);

  int leftPercentage = 0;
  int rightPercentage = 0;

  if(motorPercentage != 50){
    leftPercentage = 100 - motorPercentage;
    rightPercentage = 100 - leftPercentage;
  }
  // else if(motorPercentage > 50){
  //   rightPercentage = 100 - motorPercentage;
  //   leftPercentage = 100 - rightPercentage;
  // }
  else{
    leftPercentage = 100;
    rightPercentage = 100;
  }

  Serial.print("Potentiometer: ");
  Serial.println(potValue);
  Serial.print("Motor Percentage: ");
  Serial.println(motorPercentage);
  Serial.print("Right percentage ");
  Serial.println(rightPercentage);
  Serial.print("Left percentage ");
  Serial.println(leftPercentage);
  float leftMotor = sensorValue * (leftPercentage / 100.0f);
  float rightMotor = sensorValue * (rightPercentage / 100.0f);

  Serial.print(leftMotor);
  Serial.print(" | ");
  Serial.println(rightMotor);

  if (sensorValue < 1000) {
    sensorValue = 0;
  }

  char leftMsg[16];
  char rightMsg[16];
  itoa(leftMotor, leftMsg, 10);
  itoa(rightMotor, rightMsg, 10);


  Serial.println(sensorValue);
  char msg[16];
  itoa(sensorValue, msg, 10);

  nrf24.send((uint8_t*)msg, strlen(msg));
  nrf24.send((uint8_t*)leftMsg, strlen(leftMsg));
  nrf24.send((uint8_t*)rightMsg, strlen(rightMsg));
  nrf24.waitPacketSent();

  delay(100);

}
