 
#include <SPI.h>
#include <RH_NRF24.h>
#ifdef ESP32
#include <WiFi.h>
#endif

#define CE 4
#define CSN 5
 
RH_NRF24 nrf24(CE, CSN);
//RH_ASK driver(2000, 4, 0, 2); // ESP8266 or ESP32: do not use pin 11 or 2

const int potLeftPin = 33; // Connect to the wiper of the potentiometer
const int potRightPin = 34; // Connect to the wiper of the potentiometer

const int leftMotor = 25;
const int rightMotor = 26;

const int freq = 2000;
const int motorChannel = 0;
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

  pinMode(potLeftPin, INPUT);
  pinMode(potRightPin, INPUT);
  //pinMode(leftMotor, OUTPUT);

  ledcSetup(motorChannel, freq, resolution);
  ledcAttachPin(leftMotor, motorChannel);
  ledcAttachPin(rightMotor, motorChannel);
}
 
 
void loop()
{
  Serial.println("Sending to nrf24_server");
  // Send a message to nrf24_server
   uint8_t data[] = "This is the Receiver.";
  //uint8_t data[] = "blink";
  nrf24.send(data, sizeof(data));
  
  nrf24.waitPacketSent();



  // Now wait for a reply
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
 
  if (nrf24.waitAvailableTimeout(500)) { 
    // Should be a reply message for us now   
    if (nrf24.recv(buf, &len)){
      //Serial.print("Speed received: ");
      Serial.println(atoi((char*)buf));
      int speed = map(atoi((char*)buf), 0, 4095, 0, 4095);
      if(speed >= 1600){
        speed += 1500;
        if(speed > 4095) {
          speed = 4095;
        }
        ledcWrite(motorChannel, speed);
      }
      else{
         ledcWrite(motorChannel, 0);
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
