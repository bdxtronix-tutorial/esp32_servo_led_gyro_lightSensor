#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h>
#include <Servo.h>

// Define Firebase credentials
#define FIREBASE_HOST "Database URL Anda"
#define FIREBASE_AUTH "Database Secret Anda "
#define DATABASE_PATH "Project Bucket Anda"

// Define Wi-Fi credentials
#define WIFI_SSID "Nama WiFi Anda"
#define WIFI_PASSWORD "Password WiFi Anda"

// Define pins
#define SERVO_PIN 13       
#define LED_PIN 23         

// Variables to store sensor data
float gyroscopeX, gyroscopeY, gyroscopeZ, lightSensor;

// Objects
FirebaseData firebaseData;
Servo servoMotor;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize servo
  servoMotor.attach(SERVO_PIN);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Read data from Firebase
  String path;

  path = String(DATABASE_PATH) + "/sensorData/gyroscopeX";
  if (Firebase.getFloat(firebaseData, path.c_str())) {
      gyroscopeX = firebaseData.floatData();
  } else {
      Serial.println("Failed to get gyroX data from Firebase!");
      Serial.println(firebaseData.errorReason());
  }

  path = String(DATABASE_PATH) + "/sensorData/gyroscopeY";
  if (Firebase.getFloat(firebaseData, path.c_str())) {
      gyroscopeY = firebaseData.floatData();
  } else {
      Serial.println("Failed to get gyroY data from Firebase!");
      Serial.println(firebaseData.errorReason());
  }

  path = String(DATABASE_PATH) + "/sensorData/gyroscopeZ";
  if (Firebase.getFloat(firebaseData, path.c_str())) {
      gyroscopeZ = firebaseData.floatData();
  } else {
      Serial.println("Failed to get gyroZ data from Firebase!");
      Serial.println(firebaseData.errorReason());
  }

  path = String(DATABASE_PATH) + "/sensorData/lightSensor";
  if (Firebase.getFloat(firebaseData, path.c_str())) {
      lightSensor = firebaseData.floatData();
  } else {
      Serial.println("Failed to get lightSensor data from Firebase!");
      Serial.println(firebaseData.errorReason());
  }

  // Print sensor data
  Serial.print("Gyro X: ");
  Serial.println(gyroscopeX);
  Serial.print("Gyro Y: ");
  Serial.println(gyroscopeY);
  Serial.print("Gyro Z: ");
  Serial.println(gyroscopeZ);
  Serial.print("Light Intensity: ");
  Serial.println(lightSensor);

  // Control servo based on Gyroscope X
  if (gyroscopeX < 0) {
      servoMotor.write(0);   
  } else {
      servoMotor.write(180); 
  }

  // Control LED brightness based on light intensity
  int ledBrightness = map((int)lightSensor, 1, 100, 0, 255);
  analogWrite(LED_PIN, ledBrightness);

  delay(1000); 
}
