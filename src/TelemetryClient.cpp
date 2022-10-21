#include <Arduino.h>
#include <ArduinoJson.h>
#include "TelemetryClient.hpp"

IoTHubConnector _telemetry_iothub_client;

void sendTelemetryMessage()
{
    DynamicJsonDocument doc(1024);
    doc["Protocol"] = "Device.3";
    doc["MessageType"] = "Telemetry";
    doc["Data"]["valveOpen"] = false;
    doc["Data"]["light"] = analogRead(WIO_LIGHT);
    _telemetry_iothub_client.SendMessage(doc);
}
Task onTelemetryTick(15000, TASK_FOREVER, &sendTelemetryMessage);

void TelemetryClient::setup(Scheduler &ts, IoTHubConnector &client)
{
    _telemetry_iothub_client = client;
    pinMode(WIO_LIGHT, INPUT);

    ts.addTask(onTelemetryTick);
    onTelemetryTick.enable();
}
