# Homekit-Mower-Door
A HomeKit-compatible smart controller for robot mower doors using ESP32-C6 and HomeSpan. Control your mower's access door through Apple Home with simple open/close functionality. Features relay control for a linear actuator with configurable timing and safety features. Easy to set up with minimal hardware requirements.

## Hardware
- ESP32-C6-DevKitC-1-N8 board
- Two relays connected to pins 19 and 20 to control linear actuator direction
- Linear actuator

## Setup
1. Update WiFi credentials in the code (replace `SOME_SSID` and `SOME_PASSWORD`)
2. Flash the code to your ESP32 (can be easily done using Arduino IDE)
3. The system automatically extends the actuator fully on startup as a safety feature
4. Add to Apple Home using the pairing code: `8293740`
5. Device will appear as a garage door opener with the name "Mower Door"

## Operation
- The actuator is controlled via the HomeKit app with simple open/close functionality
- Operation timeout is set to 14 seconds (can be adjusted in the code)
- Uses relay control in H-bridge configuration for bidirectional actuator control

## Attribution
This project was created by Trevor Tomlinson and is licensed under the MIT License with Attribution Requirement.

### Dependencies
- [HomeSpan](https://github.com/HomeSpan/HomeSpan) - HomeKit implementation for ESP32
