#include <SPI.h>
#include <RH_NRF24.h>
#ifdef ESP32
#include <WiFi.h>
#endif

#define CE 4
#define CSN 5
 
RH_NRF24 nrf24(CE, CSN);
//RH_ASK driver(2000, 4, 0, 2); // ESP8266 or ESP32: do not use pin 11 or 2

// const int potLeftPin = 33; // Connect to the wiper of the potentiometer
// const int potRightPin = 34; // Connect to the wiper of the potentiometer

//byte motorMovement[2];

struct Data {
    int leftMotor;
    int rightMotor;
} receivedData;

const int leftMotor = 14;
const int rightMotor = 12;

const int freq = 2000;
const int motorChannelLeft = 0;
const int motorChannelRight = 1;
const int resolution = 12;

 
void setup() 
{
  Serial.begin(9600);
  while (!Serial) ; // wait for serial port to connect. Needed for Leonardo only
  if (!nrf24.init()) {
    Serial.println("init failed");
  }
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(22)) {
    Serial.println("setChannel failed");
  }

  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)){
    Serial.println("setRF failed");    
  }

  ledcSetup(motorChannelLeft, freq, resolution);
  ledcAttachPin(leftMotor, motorChannelLeft);

  ledcSetup(motorChannelRight, freq, resolution);
  ledcAttachPin(rightMotor, motorChannelRight);
}
 
 
void loop()
{
  //Serial.println("Sending to nrf24_server");
  // Send a message to nrf24_server
  //uint8_t data[] = "This is the Receiver.";
  //uint8_t data[] = "blink";
  //nrf24.send(data, sizeof(data));
  //nrf24.waitPacketSent();

  // Now wait for a reply
  // uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  // uint8_t len = sizeof(buf);


  //nrf24.read(movement, sizeof(movement));

 
  if (nrf24.waitAvailableTimeout(500)) { 
    // Should be a reply message for us now   
    //if (nrf24.recv(buf, &len)){
    //if (nrf24.read(movement, sizeof(movement))){ //Trying to receive data as an array
    if (read(&receivedData, sizeof(Data))){ // usign objects now????

      //og
      //Serial.print("Speed received: ");
      //Serial.println(atoi((char*)buf));

      //array
      // int valLeftMotor = atoi((char*)movement[0]);
      // int valRightMotor = atoi((char*)movement[1]);

      //object
      Serial.print("Received integers: ");
      Serial.print(receivedData.leftMotor);
      Serial.print(", ");
      Serial.println(receivedData.rightMotor);

      //og
      // Serial.print("EMG VAL: ");
      // Serial.println(emgVal);

      //array
      // Serial.print("Left VAL: ");
      // Serial.println(valLeftMotor);
      // Serial.print("Right VAL: ");
      // Serial.println(valRightMotor);

      //int speed = map(atoi((char*)buf), 0, 4095, 0, 4095);
      //Serial.println(speed);

      //og
      // if(emgVal > 500){
      //   //emgVal += 2000;
      //   if(emgVal > 4095) {
      //     emgVal = 4095;
      //   }
      //   int speed = map(emgVal, 0, 4095, 0, 4095);
      //   Serial.print("Speed: ");
      //   Serial.println(speed);
      //   ledcWrite(motorChannel, speed);
      // }
      // else{
      //    ledcWrite(motorChannel, 0);
      // }

      //object
      if(receivedData.leftMotor > 500 && receivedData.rightMotor > 500){ //both motors greater than 500

        receivedData.leftMotor += 2000;
        receivedData.rightMotor += 2000;

        if(receivedData.leftMotor > 4095) {
          receivedData.leftMotor = 4095;
        }
        if(receivedData.rightMotor > 4095) {
          receivedData.rightMotor = 4095;
        }

        Serial.print("Left Motor: ");
        Serial.print(receivedData.leftMotor);
        Serial.print(" | ");
        Serial.print("Right Motor: ");
        Serial.println(receivedData.rightMotor);
        ledcWrite(motorChannelLeft, receivedData.leftMotor);
        ledcWrite(motorChannelRight, receivedData.rightMotor);
      }
      else if(receivedData.leftMotor < 500 && receivedData.rightMotor > 500){ //left motor less than 500; right, greater

        receivedData.leftMotor = 0;
        receivedData.rightMotor += 2000;

        if(receivedData.rightMotor > 4095) {
          receivedData.rightMotor = 4095;
        }

        Serial.print("Left Motor: ");
        Serial.print(receivedData.leftMotor);
        Serial.print(" | ");
        Serial.print("Right Motor: ");
        Serial.println(receivedData.rightMotor);
        ledcWrite(motorChannelLeft, receivedData.leftMotor);
        ledcWrite(motorChannelRight, receivedData.rightMotor);
      }
      else if(receivedData.leftMotor > 500 && receivedData.rightMotor < 500){ //right motor less than 500; left, greater

        receivedData.leftMotor += 2000;
        receivedData.rightMotor = 0;

        if(receivedData.leftMotor > 4095) {
          receivedData.leftMotor = 4095;
        }

        Serial.print("Left Motor: ");
        Serial.print(receivedData.leftMotor);
        Serial.print(" | ");
        Serial.print("Right Motor: ");
        Serial.println(receivedData.rightMotor);
        ledcWrite(motorChannelLeft, receivedData.leftMotor);
        ledcWrite(motorChannelRight, receivedData.rightMotor);
      }
      else{
        ledcWrite(motorChannelLeft, 0);
        ledcWrite(motorChannelRight, 0);
      }
      
    }
    else {
      Serial.println("recv failed");
    }
  }
  else {
    Serial.println("No reply, is nrf24_server running?");
  }
  delay(100);
}
