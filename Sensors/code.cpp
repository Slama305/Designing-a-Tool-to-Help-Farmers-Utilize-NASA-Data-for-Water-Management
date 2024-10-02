#include <WiFi.h>
#include <FirebaseESP32.h>

// Firebase credentials
#define FIREBASE_HOST "https://nasa-backend-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "0MbfV5xtV8H7byp1OLsMSgx4R4wmTSYHuvXUhsrJ"
#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASSWORD "your-wifi-password"

// Firebase data object
FirebaseData firebaseData;

// Sensor pins
int soilMoisturePin = 34;
int waterLevelPin = 35;
int pHPin = 32;
int tempPin = 33;
int pestSensorPin = 36;

// Setup WiFi and Firebase
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Serial.println("Connected to Firebase");
}

// Function to check soil moisture
void soilMoistureCheck() {
  int soilMoistureValue = analogRead(soilMoisturePin);

  if (soilMoistureValue < 300) {
    Serial.println("Soil is dry. Suggested solution: Increase irrigation.");
    Firebase.setString(firebaseData, "/soilStatus", "Dry");
    Firebase.setString(firebaseData, "/solution", "Increase irrigation");
  } else if (soilMoistureValue > 700) {
    Serial.println("Soil is too wet. Suggested solution: Reduce watering.");
    Firebase.setString(firebaseData, "/soilStatus", "Wet");
    Firebase.setString(firebaseData, "/solution", "Reduce watering");
  } else {
    Serial.println("Soil moisture is optimal.");
    Firebase.setString(firebaseData, "/soilStatus", "Optimal");
    Firebase.setString(firebaseData, "/solution", "No action needed");
  }
}

// Function to check water level
void waterLevelCheck() {
  int waterLevelValue = analogRead(waterLevelPin);

  if (waterLevelValue > 600) {
    Serial.println("Flood risk detected. Suggested solution: Drain excess water.");
    Firebase.setString(firebaseData, "/waterStatus", "Flood risk");
    Firebase.setString(firebaseData, "/solution", "Drain excess water");
  } else {
    Serial.println("Water level is normal.");
    Firebase.setString(firebaseData, "/waterStatus", "Normal");
    Firebase.setString(firebaseData, "/solution", "No action needed");
  }
}

// Function to check soil quality (pH and temperature)
void soilQualityCheck() {
  int pHValue = analogRead(pHPin);
  int tempValue = analogRead(tempPin);

  if (pHValue < 300 || tempValue > 700) {
    Serial.println("Soil quality is poor. Suggested solution: Apply fertilizers or adjust pH.");
    Firebase.setString(firebaseData, "/soilQuality", "Poor");
    Firebase.setString(firebaseData, "/solution", "Apply fertilizers or adjust pH");
  } else {
    Serial.println("Soil quality is good.");
    Firebase.setString(firebaseData, "/soilQuality", "Good");
    Firebase.setString(firebaseData, "/solution", "No action needed");
  }
}

// Function to check for pest detection
void pestCheck() {
  bool pestDetected = digitalRead(pestSensorPin);

  if (pestDetected) {
    Serial.println("Pests detected. Suggested solution: Use pest control measures.");
    Firebase.setString(firebaseData, "/pestStatus", "Detected");
    Firebase.setString(firebaseData, "/solution", "Use pest control");
  } else {
    Serial.println("No pests detected.");
    Firebase.setString(firebaseData, "/pestStatus", "None");
    Firebase.setString(firebaseData, "/solution", "No action needed");
  }
}

// Main loop
void loop() {
  soilMoistureCheck();
  waterLevelCheck();
  soilQualityCheck();
  pestCheck();

  delay(5000); // Repeat every 5 seconds
}
