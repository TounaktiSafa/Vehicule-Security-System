
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

// Définition des broches
#define DHTPIN 4 // Broche où est connecté le DHT22
#define DHTTYPE DHT22 // Type de DHT
#define GAS_SENSOR_PIN 34 // Broche du capteur de gaz (GPIO34, entrée analogique)
#define FLAME_SENSOR_PIN 33 // Broche du capteur de flamme (GPIO35, entrée analogique)
#define BUZZER_PIN 19
#define RELAY_PIN 27
#define GREEN_LED_PIN 2
#define RED_LED_PIN 15

// Initialisation des objets
DHT dht(DHTPIN, DHTTYPE);

// Seuils pour déclencher l'alarme
const int gasThreshold = 880;
const float tempThreshold = 30;
const float humidityThreshold = 20;

// Informations de connexion WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Informations Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // Port MQTT
#define AIO_USERNAME    "RABABZA"
#define AIO_KEY         "aio_TIER10nOVfSUrwCgtrUTrUiksN7J"

// Création des clients WiFi et MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Variables de temps
unsigned long lastSensorRead = 0;
unsigned long lastPublish = 0;
const unsigned long sensorInterval = 1000; // Intervalle pour lire les capteurs
const unsigned long publishInterval = 5000; // Intervalle pour envoyer les données

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
}

void MQTT_connect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");
    if (client.connect("ESP32Client", AIO_USERNAME, AIO_KEY)) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200); // Vitesse de communication plus rapide

  // Initialisation de l'afficheur LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Bienvenue");
  lcd.setCursor(0,1);
  lcd.print("Systeme de detection");
  lcd.setCursor(0,2);
  lcd.print("d'incendie");
  delay(3000);  // Pause pour afficher le message de bienvenue
  lcd.clear();

  // Initialisation des broches
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);

  // Initialisation du capteur DHT
  dht.begin();
  
  // Connexion WiFi
  connectToWiFi();

  // Configuration du client MQTT
  client.setServer(AIO_SERVER, AIO_SERVERPORT);
}

void loop() {
  // Assurer la connexion MQTT
  MQTT_connect();

  unsigned long currentMillis = millis();

  // Lecture des valeurs des capteurs à intervalles réguliers
  if (currentMillis - lastSensorRead >= sensorInterval) {
    lastSensorRead = currentMillis;

    // Lecture des valeurs des capteurs
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int gasValue = analogRead(GAS_SENSOR_PIN);
    int flameState = digitalRead(FLAME_SENSOR_PIN);

    // Détection d'incendie
    bool fireDetected = temp > tempThreshold || gasValue > gasThreshold || hum < humidityThreshold;

    if (fireDetected) {
      tone(BUZZER_PIN, 1000); // Émet un son de 1000 Hz
      digitalWrite(RELAY_PIN, HIGH);
      digitalWrite(RED_LED_PIN, HIGH);
      digitalWrite(GREEN_LED_PIN, LOW);
    } else {
      // Alarme désactivée
      noTone(BUZZER_PIN); // Arrête le son du buzzer
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(GREEN_LED_PIN, HIGH);
    }

    // Affichage des valeurs sur le LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(hum);
    lcd.print("%");

    lcd.setCursor(0, 2);
    lcd.print("Gaz: ");
    lcd.print(gasValue);

    lcd.setCursor(0, 3);
    lcd.print("Flamme: ");
    lcd.print(flameState ? "0" : "1");

    // Envoi des données à Adafruit IO à intervalles réguliers
    if (currentMillis - lastPublish >= publishInterval) {
      lastPublish = currentMillis;

      client.publish(AIO_USERNAME "/feeds/temperature", String(temp).c_str());
      client.publish(AIO_USERNAME "/feeds/humidity", String(hum).c_str());
      client.publish(AIO_USERNAME "/feeds/gas", String(gasValue).c_str());
      client.publish(AIO_USERNAME "/feeds/flame", String(flameState).c_str());
    }
  }
}
