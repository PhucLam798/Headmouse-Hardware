const int sensorPin = A0;
int sensorValue = 0;
int threshold = 500;  

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(sensorPin);

  Serial.print("Sensor value: ");
  Serial.println(sensorValue);

  if (sensorValue < threshold) {
    
    Serial.println("Eyes closed");
  } else {
    
    Serial.println("Eyes open");
  }

  delay(100);
}
