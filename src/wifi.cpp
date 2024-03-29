#include <Arduino.h>

#include <WiFi.h>

#include <wifi.h>

class wifi
{
public:
    static void connect()
    {
        delay(10);
        // We start by connecting to a WiFi network
        Serial.println();
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.print("WiFi connected - ESP IP address: ");
        Serial.println(WiFi.localIP());
    }
};