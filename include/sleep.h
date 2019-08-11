#ifndef sleep_h
#define sleep_h

#include <Arduino.h>

// Conversion factor for micro seconds to seconds
#define uS_TO_S_FACTOR 1000000
// Time ESP32 will go to sleep (in seconds)
#define TIME_TO_SLEEP 30

class Sleep
{
public:
    static void print_wakeup_reason(int &bootCount);
    static void go_to_sleep();
};

#endif /* _SLEEP_H_ */