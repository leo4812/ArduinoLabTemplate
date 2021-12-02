#include <Arduino.h>

REDIRECT_STDOUT_TO(Serial);

#include "Sensors.hpp"

BLEService MainService("2086C901-9167-4F23-8A7A-F514BD665227");

void setup()
{
  pinMode(LED_RED, OUTPUT);
  analogReadResolution(12);
  Serial.begin(115200);
  while (!Serial)
  {
    delay(1);
  }

  if (BLE.begin())
  {
    auto address = BLE.address();
    address.replace(":", "");
    address.toUpperCase();
    String localName = "LAB_" + address;
    Serial.print("Local name: ");
    Serial.println(localName);
    BLE.setDeviceName(localName.c_str());
    BLE.setLocalName(localName.c_str());
    BLE.setAdvertisedService(MainService);
    init_sensors(MainService);
    BLE.addService(MainService);
    BLE.setEventHandler(BLEConnected, [](BLEDevice central) { Serial.println("Device connected"); });
    BLE.setEventHandler(BLEDisconnected, [](BLEDevice central) { Serial.println("Device disconnected"); });
    BLE.advertise();
    Serial.println("Application started");
  }
  else
  {
    Serial.println("Error init ble");
    for (;;)
    {
      digitalWrite(LED_RED, HIGH);
      delay(50);
      digitalWrite(LED_RED, HIGH);
      delay(50);
      digitalWrite(LED_RED, HIGH);
      delay(1000);
    }
  }
}

void loop()
{
  BLE.poll();
}