#include <Arduino.h>
REDIRECT_STDOUT_TO(Serial);

#include "Sensors.hpp"
BLEService MainService("2086C901-9167-4F23-8A7A-F514BD665227");

typedef struct
{
  char data[60];
  int size;
  unsigned long time;
} message_t;

Thread read_thread;
Thread queue_thread;

MemoryPool<message_t, 10> mpool;
Queue<message_t, 10> serial_queue;

int split(String str, String *result)
{
  // String strs[20];
  int StringCount = 0;

  while (str.length() > 0)
  {
    int index = str.indexOf(";");

    if (index == -1) // No space found
    {
      result[StringCount++] = str;
      break;
    }
    else
    {
      result[StringCount++] = str.substring(0, index);
      str = str.substring(index + 1);
    }
  }
  return StringCount;
}

void serial_callback()
{
  while (true)
  {
    String line = "";
    if (Serial.available())
    {
      while (Serial.available() > 0)
      {
        char c = Serial.read();
        line += c;
        if (c == 0x0A)
        {
          auto data = line.c_str();
          auto size = line.length();
          message_t *message = mpool.try_alloc();
          memcpy(message->data, data, size);
          message->size = size;
          message->time = millis();
          serial_queue.try_put(message);
          line = "";
        }
      }
    }
    rtos::ThisThread::yield();
  }
}

void queue_callback()
{
  while (true)
  {
    message_t *message;
    if (serial_queue.try_get(&message))
    {
      String line = String(message->data, message->size);
      mpool.free(message);
      String strs[6];
      int parts = split(line, strs);
      if (parts > 1)
      {
        auto guid = strs[0];

        for (size_t i = 0; i < TotalSensors; i++)
        {
          auto s = sensors[i];

          if (guid.equalsIgnoreCase(String(s->CommandCharacteristic->uuid())))
          {
            int start = strs[1].toInt();
            uint32_t interval = 0;
            uint8_t port = 0;
            if (start == 1)
            {
              interval = strs[2].toInt();
              port = strs[3].toInt();
            }
            s->SerialHandler(start, interval, port);
            break;
          }
        }
      }
    }
    rtos::ThisThread::yield();
  }
}

void setup()
{
  analogReadResolution(12);
  Serial.begin(115200);
  Serial.setTimeout(100);

  read_thread.start(serial_callback);
  queue_thread.start(queue_callback);

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
