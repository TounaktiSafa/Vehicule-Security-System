#include <WiFi.h>
#include <PicoMQTT.h>

// Wi-Fi credentials
const char* ssid = "Student-PI"; // SSID of the WiFi access point
const char* password = "student2023"; // Password of the WiFi access point

// Create WiFi client to connect to the gateway's access point
WiFiClient espClient;

// MQTT client setup
PicoMQTT::Client mqttClient("172.16.18.52", 1883); // Update this with the broker's IP address

// Pin definitions for ultrasonic sensor
#define ULTRASONIC_TRIG_PIN 5
#define ULTRASONIC_ECHO_PIN 18

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize ultrasonic sensor pins
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);

  // Initialize MQTT client
  mqttClient.begin();
  while (!mqttClient.connect("ESP32_Client")) { // Attempt to connect with a client ID
    Serial.println("Attempting MQTT connection...");
    delay(1000);
  }
  Serial.println("MQTT connected");

  Serial.println("Edge device setup complete");
}

void loop() {
  mqttClient.loop(); // Keep MQTT connection alive

  // Ultrasound sensor for object detection
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);

  // Measure the duration for the echo pin to go HIGH
  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2; // Convert to distance in cm

  // Check MQTT connection before publishing
  if (mqttClient.connected()) {
    mqttClient.publish("trashBin/distance", String(distance));
    Serial.println("Sending distance to gateway: " + String(distance) + " cm");
  } else {
    Serial.println("MQTT connection lost, reconnecting...");
    while (!mqttClient.connect("ESP32_Client")) {
      Serial.println("Reconnecting to MQTT...");
      delay(1000);
    }
  }

  delay(5000); // Publish every 5 seconds
}
