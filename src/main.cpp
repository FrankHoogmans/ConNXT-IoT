#include <Arduino.h>
#include <TaskScheduler.h>
#include <SPI.h>
#include "config.hpp"
#include "Gui.hpp"
#include "IoTHubConnector.hpp"
#include "lvgl.h"
#include "TelemetryClient.hpp"
#include "WifiConnection.hpp"

Scheduler ts;
WifiConnection wifi;
IoTHubConnector client;
TelemetryClient telemetryClient;
Gui gui;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ; // Wait for Serial to be ready

  // Setup the UI
  gui.setup(ts);

  // Connect to wifi
  wifi.Connect(SSID, PASSWORD);
  delay(500);

  gui.connected();

  // Connect to iot hub
  client.Connect(ts, IOTHUB_CONNECTION_STRING, IOTHUB_DEVICEID);
  delay(500);

  // Start the telemetry stream
  telemetryClient.setup(ts, client);
}

void loop()
{
  // Run the scheduler to make everything go
  ts.execute();

  // Let the UI do it's work
  lv_tick_inc(5);
  lv_task_handler();
}