#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP);
// NTPClient timeClient(ntpUDP, "id.pool.ntp.org", timeofset, updateinterval)
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", 25200, 60000);
