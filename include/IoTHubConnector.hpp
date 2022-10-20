#include <ArduinoJson.h>
#include <TaskSchedulerDeclarations.h>

#ifndef Included_IoTHubConnector
#define Included_IoTHubConnector
class IoTHubConnector
{
public:
    // Connects to the IoTHub with the given connectionstring
    void Connect(Scheduler &ts, const char *CONNECTION_STRING, const char *DEVICE_ID);

    // Sends a message
    void SendMessage(DynamicJsonDocument doc);
};
#endif
