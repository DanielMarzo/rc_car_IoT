// ask_transmitter.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to transmit messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) transmitter with an TX-C1 module
// Tested on Arduino Mega, Duemilanova, Uno, Due, Teensy, ESP-12
 
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

 

RH_ASK driver(2000, 5, 4, 2); // ESP8266 or ESP32: do not use pin 11 or 2
// RH_ASK driver(2000, 3, 4, 0); // ATTiny, RX on D3 (pin 2 on attiny85) TX on D4 (pin 3 on attiny85), 
// RH_ASK driver(2000, PD14, PD13, 0); STM32F4 Discovery: see tx and rx on Orange and Red LEDS

const int potLeftPin = 33; // Connect to the wiper of the potentiometer
const int potRightPin = 34; // Connect to the wiper of the potentiometer


const int leftMotor = 4;
const int rightMotor = 12;

const int freq = 2000;
const int motorChannel = 0;
const int resolution = 10;
 
void setup()
{

    Serial.begin(9600);   // Debugging only

    if (!driver.init())
         Serial.println("init failed");

    pinMode(potLeftPin, INPUT);
    pinMode(potRightPin, INPUT);
    //pinMode(leftMotor, OUTPUT);

    ledcSetup(motorChannel, freq, resolution);
    ledcAttachPin(leftMotor, motorChannel);




}
 
void loop()
{
    //const char *msg = "blink";
    int left = analogRead(potLeftPin);
    int right = analogRead(potRightPin);

    int leftSpeed = map(left, 0, 4095, 0, 1024);
    int rightSpeed = map(right, 0, 4095, 0, 1024);

    Serial.println("LEFT SPEED: ");
    Serial.println(leftSpeed);
    ledcWrite(motorChannel, leftSpeed);
    //digitalWrite(leftMotor, HIGH);
    Serial.print("Left: "); 
    Serial.print(left);
    Serial.print(" | Right: "); 
    Serial.println(right);



    // driver.send((uint8_t *)msg, strlen(msg));
    // driver.waitPacketSent();
    delay(100);
}