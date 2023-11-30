#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// define wifi and firebase
#define WIFI_SSID "ufdevice"
#define WIFI_PASSWORD "gogators"
#define API_KEY "AIzaSyBFFyscZA31a09ptrLpEMPgJp6dBDglppk"
#define DATABASE_URL "https://rc-car-8e3cd-default-rtdb.firebaseio.com/"

FirebaseData fbdb;
FirebaseAuth auth;
FirebaseConfig config;

Adafruit_MPU6050 mpu;

bool signupOK = false;
float accX = 0.0;
float accY = 0.0;
float velocityX = 0.0;
float velocityY = 0.0;
float deltaTime = 0.1;
int i = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MPU6050 test!");

  // connect to wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print('.');
    delay(200);
  }

  Serial.println();
  Serial.print("Connected with IP:");
  Serial.println(WiFi.localIP());
  Serial.println();

  // connect to firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("sign up OK");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // setup accelerometer
  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("");
  delay(100);
}

void loop() {
  // get sensor data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
    
  // get acceleration from accelerometer
  accX = abs(a.acceleration.x);
  accY = abs(a.acceleration.y);

  // calculate velocity
  velocityX = accX * deltaTime;
  velocityY = accY * deltaTime;

  // calculate overall speed
  float speed = sqrt(velocityX * velocityX + velocityY * velocityY);

  // print speed to the serial monitor for debugging purposes
  /*
  Serial.print("Acceleration X (Roll): ");
  Serial.print(accX, 2);
  Serial.println(" m/s^2");
  Serial.print("Acceleration Y (Pitch): ");
  Serial.print(accY, 2);
  Serial.println(" m/s^2");
  Serial.print("Speed: ");
  Serial.print(speed, 2);
  Serial.println(" m/s");
  Serial.println("");
` */

  // send to firebase
  Firebase.RTDB.setFloat(&fbdb, "Sensor/speed", speed);
  Firebase.RTDB.setFloat(&fbdb, "Sensor/accX", accX);
  Firebase.RTDB.setFloat(&fbdb, "Sensor/accY", accY);

  delay(100);
}