#include "uptime.h"

String Days, Hours, Minutes, Seconds, Milliseconds;
String UPTimes;

bool UPTime(void *)
{

    // Serial.print("days: ");
    // Serial.println(uptime::getDays());
    Days = uptime::getDays();

    // Serial.print("hours: ");
    // Serial.println(uptime::getHours());
    Hours = uptime::getHours();

    // Serial.print("minutes: ");
    // Serial.println(uptime::getMinutes());
    Minutes = uptime::getMinutes();

    // Serial.print("seconds: ");
    // Serial.println(uptime::getSeconds());
    Seconds = uptime::getSeconds();

    // Serial.print("milliseconds: ");
    // Serial.println(uptime::getMilliseconds());
    Milliseconds = uptime::getMilliseconds();

    // Serial.print("\n");

    UPTimes = Days + "Days :" + Hours + "Hours :" + Minutes + "Minutes :" + Seconds + "s :" + Milliseconds + "ms";

    return true;
}