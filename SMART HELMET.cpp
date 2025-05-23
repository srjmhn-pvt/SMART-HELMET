#include <Wire.h>
#include <SoftwareSerial.h>
#include <MPU6050.h>
#include <TinyGPS++.h>

MPU6050 mpu;
SoftwareSerial gsm(7, 8);  // GSM module pins
SoftwareSerial gps(4, 3);  // GPS module pins
TinyGPSPlus gpsData;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  gsm.begin(9600);
  gps.begin(9600);
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Simple threshold for accident detection
  if (abs(ax) > 15000 || abs(ay) > 15000 || abs(az) > 15000) {
    while (gps.available()) {
      gpsData.encode(gps.read());
    }
    if (gpsData.location.isValid()) {
      String message = "Accident at: ";
      message += gpsData.location.lat();
      message += ", ";
      message += gpsData.location.lng();
      sendSMS(message);
    }
    delay(10000);  // Avoid spamming
  }
}

void sendSMS(String text) {
  gsm.println("AT+CMGF=1");
  delay(1000);
  gs
