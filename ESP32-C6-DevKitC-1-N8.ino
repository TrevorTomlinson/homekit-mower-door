#include "HomeSpan.h"
#include "Ticker.h"
// Define relay control pins
#define RELAY1_PIN 19  // Controls one direction
#define RELAY2_PIN 20  // Controls the other direction

Ticker StopActuatorTicker;
// WiFi Credentials - replace with your own
const char* ssid = "SOME_SSID";
const char* password = "SOME_PASSWORD";

// Pre-define HomeSpan configuration to reduce binary size
#define HOMESPAN_MINIMIZE_BUILTIN_SERVICES

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void stopActuator() {
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
}

struct ActuatorSwitch : Service::Switch {
  SpanCharacteristic *power;
  
  ActuatorSwitch() : Service::Switch() {
    power = new Characteristic::On();
  }
  
  boolean update() override {
    StopActuatorTicker.detach(); // Cancel any previously scheduled stopActuator call
    
    if (power->getNewVal()) {
      // retract
      digitalWrite(RELAY1_PIN, HIGH);
      digitalWrite(RELAY2_PIN, LOW);
    } else {
      // extend
      digitalWrite(RELAY1_PIN, LOW);
      digitalWrite(RELAY2_PIN, HIGH);
    }
    
    StopActuatorTicker.once_ms(14*1000, stopActuator); // You may need to adjust this value based on your actuator's travel time
    return true;
  }
};

void setup() {
  Serial.begin(115200);
  
  // Initialize pins before anything else
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  
  // Safety feature: Always extend actuator fully at startup
  Serial.println("Performing initial safety extension...");
  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, HIGH);
  delay(14000); // Run for the full extension time
  stopActuator();
  Serial.println("Initial extension complete");
  
  connectToWiFi();
  
  homeSpan.setPairingCode("8293740");
  homeSpan.begin(Category::GarageDoorOpeners,"Mower Door");
  
  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Identify();
  new Characteristic::Manufacturer("XXX");
  new Characteristic::Model("MowerDoorActuator");
  new Characteristic::SerialNumber("12345678");
  new ActuatorSwitch();
}

void loop() {
  homeSpan.poll();
}