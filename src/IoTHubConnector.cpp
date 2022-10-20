#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include "IoTHubConnector.hpp"
#include <iothubtransportmqtt.h>
#include "ntp.hpp"
#include <SPI.h>

IOTHUB_DEVICE_CLIENT_LL_HANDLE _device_ll_handle;
const char *_device_id;

void connectionStatusCallback(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void *user_context)
{
    if (result == IOTHUB_CLIENT_CONNECTION_AUTHENTICATED)
    {
        Serial.println("The device client is connected to iothub");
    }
    else
    {
        Serial.println("The device client has been disconnected");
    }
}

int directMethodCallback(const char *method_name, const unsigned char *payload, size_t size, unsigned char **response, size_t *response_size, void *userContextCallback)
{
    Serial.printf("Direct method received %s\r\n", method_name);
    return IOTHUB_CLIENT_OK;
}

void doWorkForIoTHubClient()
{
    IoTHubDeviceClient_LL_DoWork(_device_ll_handle);
}
Task onTick(250, TASK_FOREVER, &doWorkForIoTHubClient);

void IoTHubConnector::Connect(Scheduler &ts, const char *CONNECTION_STRING, const char *DEVICE_ID)
{
    // Store the device id
    _device_id = DEVICE_ID;

    // Sync the time before we begin
    initTime();

    // init the IoTHub library
    IoTHub_Init();

    // Create a device based on the connectionstring and MQTT transport
    _device_ll_handle = IoTHubClient_LL_CreateFromConnectionString(CONNECTION_STRING, MQTT_Protocol);
    if (_device_ll_handle == NULL)
    {
        Serial.println("Failure creating Iothub device. Hint: Check your connection string.");
        return;
    }

    IoTHubDeviceClient_LL_SetConnectionStatusCallback(_device_ll_handle, connectionStatusCallback, NULL);
    IoTHubClient_LL_SetDeviceMethodCallback(_device_ll_handle, directMethodCallback, NULL);

    ts.addTask(onTick);
    onTick.enable();
}

void IoTHubConnector::SendMessage(DynamicJsonDocument doc) {
    std::string message;
    doc["DeviceId"] = _device_id;

    // Serialize the message
    JsonObject obj = doc.as<JsonObject>();
    serializeJson(obj, message);

    // Send to the cloud
    Serial.print("Sending message: ");
    Serial.println(message.c_str());
    IOTHUB_MESSAGE_HANDLE message_handle = IoTHubMessage_CreateFromString(message.c_str());
    IoTHubDeviceClient_LL_SendEventAsync(_device_ll_handle, message_handle, NULL, NULL);
    IoTHubMessage_Destroy(message_handle);
}