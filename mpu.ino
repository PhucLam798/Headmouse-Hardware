#include <Wire.h>
#include <MPU6050.h>
#include <Mouse.h>

MPU6050 mpu;
int16_t gx_offset = 0, gy_offset = 0; 

void setup() {
  Wire.begin(); 
  mpu.initialize(); 

  Serial.begin(115200);

  if (mpu.testConnection()) {
    Serial.println("Connected");
  } else {
    Serial.println("Not Connected");
    while (1);
  }

  int16_t ax, ay, az, gx, gy, gz;
  for (int i = 0; i < 1000; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    gx_offset += gx;
    gy_offset += gy;
    delay(3);
  }
  gx_offset /= 1000; 
  gy_offset /= 1000; 

  Mouse.begin(); 

}

void loop() {
  
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  
  gx -= gx_offset;
  gy -= gy_offset;

 
  int ngThreshold = 900; 
  int moveX = 0, moveY = 0;


  int heSoNhay = 2; 


  if (abs(gx) > ngThreshold) {
    moveX = map(gx, -17000, 17000, -10 * heSoNhay, 10 * heSoNhay);
  } else {
    moveX = 0; 
  }

  if (abs(gy) > ngThreshold) {
    moveY = map(gy, -17000, 17000, -10 * heSoNhay, 10 * heSoNhay); 
  } else {
    moveY = 0; 
  }


  if (moveX != 0 || moveY != 0) {
    Mouse.move(moveX, moveY); 
  }

  delay(10); 
}
