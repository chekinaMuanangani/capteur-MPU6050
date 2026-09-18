#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <Servo.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

Servo servoX;
Servo servoY;

void setup() {
Serial.begin(9600);

if(!accel.begin())
{
Serial.println("ADXL345 non détecté");
while(1);
}

servoX.attach(9);
servoY.attach(10);
}

void loop() {

sensors_event_t event;
accel.getEvent(&event);

float ax = event.acceleration.x;
float ay = event.acceleration.y;
float az = event.acceleration.z;

float angleX = atan2(ay, az) * 180 / PI;
float angleY = atan2(ax, az) * 180 / PI;

int servoPosX = map(angleX, -90, 90, 0, 180);
int servoPosY = map(angleY, -90, 90, 0, 180);

servoX.write(servoPosX);
servoY.write(servoPosY);

delay(20);
}