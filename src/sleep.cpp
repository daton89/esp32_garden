#include <Arduino.h>

// Conversion factor for micro seconds to seconds
#define uS_TO_S_FACTOR 1000000
// Time ESP32 will go to sleep (in seconds)
#define TIME_TO_SLEEP 30

class sleep
{ // Function that prints the reason by which ESP32 has been awaken from sleep
public:
    static void print_wakeup_reason()
    {
        esp_sleep_wakeup_cause_t wakeup_reason;
        wakeup_reason = esp_sleep_get_wakeup_cause();
        switch (wakeup_reason)
        {
        case 1:
            Serial.println("Wakeup caused by external signal using RTC_IO");
            break;
        case 2:
            Serial.println("Wakeup caused by external signal using RTC_CNTL");
            break;
        case 3:
            Serial.println("Wakeup caused by timer");
            break;
        case 4:
            Serial.println("Wakeup caused by touchpad");
            break;
        case 5:
            Serial.println("Wakeup caused by ULP program");
            break;
        default:
            Serial.println("Wakeup was not caused by deep sleep");
            break;
        }
    }
    static void go_to_sleep()
    {
        // Set timer to 30 seconds
        esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
        Serial.println("Go to sleep now and wakeup in " + String(TIME_TO_SLEEP) + " Seconds");

        // Go to sleep now
        esp_deep_sleep_start();
    }
};
