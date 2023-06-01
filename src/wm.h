// needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

// for LED status
#include <Ticker.h>
Ticker ticker;

void tick()
{
    // toggle state
    int state = digitalRead(LED_BUILTIN); // get the current state of GPIO1 pin
    digitalWrite(LED_BUILTIN, !state);    // set pin to the opposite state
}

// gets called when WiFiManager enters configuration mode
void configModeCallback(WiFiManager *myWiFiManager)
{
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    // if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
    // entered config mode, make led toggle faster
    ticker.attach(0.2, tick);
}