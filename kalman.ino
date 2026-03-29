#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
// Kalman Filter
class Kalman {
  public:
    float Q = 0.001;
    float R = 0.1;
    float P = 1;
    float X = 0;
    float K;

    float update(float measurement) {
      P = P + Q;
      K = P / (P + R);
      X = X + K * (measurement - X);
      P = (1 - K) * P;
      return X;
    }
};

Kalman kalmanGX, kalmanGY, kalmanGZ;

int16_t gx_offset = 0, gy_offset = 0, gz_offset = 0;

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

  // Calibration
  int16_t ax, ay, az, gx, gy, gz;

  for (int i = 0; i < 1000; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    gx_offset += gx;
    gy_offset += gy;
    gz_offset += gz;
    delay(3);
  }

  gx_offset /= 1000;
  gy_offset /= 1000;
  gz_offset /= 1000;

  Serial.println("Calibration done");
}

void loop() {

  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  //RAW 
  float raw_gx = gx - gx_offset;
  float raw_gy = gy - gy_offset;
  float raw_gz = gz - gz_offset;

  //FILTER 
  float kx = kalmanGX.update(raw_gx);
  float ky = kalmanGY.update(raw_gy);
  float kz = kalmanGZ.update(raw_gz);

  //PRINT 
  Serial.print("RAW: ");
  Serial.print(raw_gx); Serial.print(" | ");
  Serial.print(raw_gy); Serial.print(" | ");
  Serial.print(raw_gz);

  Serial.print(" || FILTER: ");
  Serial.print(kx); Serial.print(" | ");
  Serial.print(ky); Serial.print(" | ");
  Serial.println(kz);

  delay(10);
}