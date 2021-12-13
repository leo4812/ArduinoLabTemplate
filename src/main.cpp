#include <Arduino.h>

#define DEBUG
//#define LED_RED 22

#ifdef DEBUG
REDIRECT_STDOUT_TO(Serial);
#endif

#include "Sensors.hpp"

BLEService MainService("2086C901-9167-4F23-8A7A-F514BD665227");

void setup()
{
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, HIGH);
  analogReadResolution(12);

#ifdef DEBUG
  Serial.begin(115200);
// while (!Serial)
//  {
//    delay(1);
//  }
#endif

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
    BLE.setEventHandler(BLEConnected, [](BLEDevice central)
                        { printf("Connected %s\n", central.address().c_str()); });
    BLE.setEventHandler(BLEDisconnected, [](BLEDevice central)
                        { printf("Device disconnected\n"); });
    BLE.advertise();
    printf("Application started\n");
  }
  else
  {
    printf("Error init ble\n");
    for (;;)
    {
      digitalWrite(LED_RED, HIGH);
      delay(50);
      digitalWrite(LED_RED, LOW);
      delay(50);
      digitalWrite(LED_RED, HIGH);
      delay(50);
      digitalWrite(LED_RED, LOW);
      delay(50);
      digitalWrite(LED_RED, HIGH);
      delay(50);
      digitalWrite(LED_RED, LOW);
      delay(1000);
    }
  }
}

void loop()
{
  BLE.poll();
}