#include <Arduino.h>
#include <SPI.h>
#include "config.hpp"
#include "WifiConnection.hpp"
#include "IoTHubConnector.hpp"
#include <TaskScheduler.h>
#include "TelemetryClient.hpp"

Scheduler ts;
WifiConnection wifi;
IoTHubConnector client;
TelemetryClient telemetryClient;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ; // Wait for Serial to be ready

  wifi.Connect(SSID, PASSWORD);
  delay(500);

  client.Connect(ts, IOTHUB_CONNECTION_STRING, IOTHUB_DEVICEID);
  delay(500);

  telemetryClient.setup(ts, client);
}

void loop()
{
  ts.execute();
}