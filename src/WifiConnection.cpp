#include "wifiConnection.hpp"

void WifiConnection::Connect(const char *SSID, const char *PASSWORD)
{
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(2000);

    WiFi.begin(SSID, PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); // prints out the device's IP address
}