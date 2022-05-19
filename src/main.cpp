#include <Arduino.h>
REDIRECT_STDOUT_TO(Serial);

#include "Sensors.hpp"
BLEService MainService("2086C901-9167-4F23-8A7A-F514BD665227");

void setup()
{  
  analogReadResolution(12);
  Serial.begin(9600);

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
}

void loop()
{
  BLE.poll();
}