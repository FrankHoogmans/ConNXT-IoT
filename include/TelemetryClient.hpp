#include "IoTHubConnector.hpp"
#include <TaskSchedulerDeclarations.h>

#ifndef Included_TelemetryClient
#define Included_TelemetryClient
class TelemetryClient
{
public:
    void setup(Scheduler &ts, IoTHubConnector &client);
};
#endif