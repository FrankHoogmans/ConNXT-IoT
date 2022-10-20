#include <rpcWiFi.h>

class WifiConnection
{
public:
    // Connects the Wifi to the given SSID
    void Connect(const char *SSID, const char *PASSWORD);
};