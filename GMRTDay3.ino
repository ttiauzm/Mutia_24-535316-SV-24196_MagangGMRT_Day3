//Mutia Umniati Z
//24/535316/SV/2196
#include <Wire.h>
#include <MPU6050.h>
#include <ESP32Servo.h>

MPU6050 mpu;
Servo s1, s2, s3, s4, s5;

#define SERVO1_PIN 18
#define SERVO2_PIN 19
#define SERVO3_PIN 23
#define SERVO4_PIN 25
#define SERVO5_PIN 26
#define SDA_PIN 21
#define SCL_PIN 22
#define PIR_PIN 13

int lastMotionState = LOW;
int currentMotionState = LOW;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  mpu.initialize();

  pinMode(PIR_PIN, INPUT);

  s1.attach(SERVO1_PIN);
  s2.attach(SERVO2_PIN);
  s3.attach(SERVO3_PIN);
  s4.attach(SERVO4_PIN);
  s5.attach(SERVO5_PIN);

  //posisi awal
  s1.write(90);
  s2.write(90);
  s3.write(90);
  s4.write(90);
  s5.write(90);

  Serial.println("System Initialized!");
}

void loop() {
  lastMotionState = currentMotionState;
  currentMotionState = digitalRead(PIR_PIN);

  //baca data mpu
  int16_t ax, ay, az; //variabel untuk accelerometer
  int16_t gx, gy, gz; //variabel untuk gryoscoper
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float roll = gx / 131.0;  // konversi gyro ke derajat/s
  float pitch = gy / 131.0;
  float yaw = gz / 131.0;

  Serial.print("Roll: "); Serial.print(roll);
  Serial.print("\tPitch: "); Serial.print(pitch);
  Serial.print("\tYaw: "); Serial.println(yaw);

  //roll
  if (roll > 5) {  //kalau rotasi ke arah positif, s1 s2 bergerak berlawanan arah (>90 dan <90)
    s1.write(45);
    s2.write(135);
  } else if (roll < -5) { //kalau ke arah negatif, s1 s2 bergerak sebaliknya
    s1.write(135);
    s2.write(45);
  } else {
    s1.write(90);
    s2.write(90);
  }

  //pitch
  if (pitch > 5) {  //kalau rotasi positif, s3 s4 searah rotasi
    s3.write(135);
    s4.write(135);
  } else if (pitch < -5) { //sebaliknya
    s3.write(45);
    s4.write(45);
  } else {
    s3.write(90);
    s4.write(90);
  }
  //yaw
  if (yaw > 5) {
    s5.write(135);  //searah rotasi positif
    delay(1000);    //diam 1 detik
    s5.write(90);   //kembali ke posisi awal
  } 
  else if (yaw < -5) {
    s5.write(45);   //sebaliknya
    delay(1000);
    s5.write(90);
  }

  //pir (kode dapet dari https://esp32io.com/tutorials/esp32-motion-sensor-servo-motor)
  if (currentMotionState == HIGH && lastMotionState == LOW) {
    Serial.println("Motion detected! All servos moving...");
    s1.write(150); s2.write(150); s3.write(150); s4.write(150); s5.write(150);
    delay(1000);
    s1.write(90); s2.write(90); s3.write(90); s4.write(90); s5.write(90);
  }

  delay(200);
}

//referensi:
//https://esp32io.com/tutorials/esp32-motion-sensor-servo-motor
//https://www.electronicwings.com/esp32/pir-sensor-interfacing-with-esp32
//https://randomnerdtutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
