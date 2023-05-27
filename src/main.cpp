/*
  The define ESPxWebFlMgr_FileSystem holds the selected filesystem in (surprise!) ESPxWebFlMgr.

  You can
    -- ignore it. Then LittleFS will be selected and you need to use LittleFS.whatever().
    -- use it. Like in this example.
    -- change it. Set a new value *BEFORE* #including <ESPxWebFlMgr.h>
       Only "#define ESPxWebFlMgr_FileSystem SPIFFS" makes any sense.
*/

// Board settings
// Board: "Generic ESP8266 Module"
// Builtin Led: "2"
// Upload Speed: "115200"
// CPU Frequency: "160 MHz"
// Crystal Frequency: "26 MHz"
// Flash Size: "4MB (FS:2MB OTA:~1019KB)"
// Flash Mode: "DOUT (compatible)"
// Flash Frequency: "40MHz"
// Reset Method: "dtr (aka nodemcu)"
// Debug port: "Disabled"
// Debug Level: "Keine"
// lwIP Variant: "v2 Lower Memory"
// VTables: "Flash"
// Exceptions: "Legacy (new can return nullptr)"
// Erase Flash: "Only Sketch"
// Espressif FW: "nonos-sdk 2.2.1+100 (190703)"
// SSL Support: "All SSL ciphers (most compatible)"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESPxWebFlMgr.h>
#include <FS.h>
#include <LittleFS.h>

#include <ArduinoJson.h>

// OTA
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "uptime.h"

#include <arduino-timer.h>
#include <WebSocketsServer.h>

WebSocketsServer webSocket = WebSocketsServer(81);

auto timer = timer_create_default();

FSInfo fs_info;

#ifndef STASSID
#define STASSID "MEFFENDI"
#define STAPSK "5758WASISGo"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

// mark parameters not used in example
#define UNUSED __attribute__((unused))

// TRACE output simplified, can be deactivated here
#define TRACE(...) Serial.printf(__VA_ARGS__)

// name of the server. You reach it using http://webserver
#define HOSTNAME "webserver"

ESP8266WebServer server(80);
const word filemanagerport = 8080;

ESPxWebFlMgr filemgr(filemanagerport); // we want a different port than the webserver

int blue = 2;
unsigned long intervalBlue = 1000; // how long to delay in millis
unsigned long previousBlue = 0;
int blueState = LOW;

String Days, Hours, Minutes, Seconds, Milliseconds;

String WiFiStrength;

const int RSSI_MAX = -50;  // define maximum strength of signal in dBm
const int RSSI_MIN = -100; // define minimum strength of signal in dBm

void fancyled()
{
  for (int i = 0; i < 30; i++)
  {
    analogWrite(blue, (i * 100) % 1001);
    delay(50);
  }
}

void handleRoot()
{
  File file = LittleFS.open("/index.html", "r");
  if (!file)
  {
    Serial.println("No Saved Data!");
  }

  server.streamFile(file, "text/html");
  file.close();
}

void handleRedirect()
{
  TRACE("Redirect...");
  String url = "/index.html";

  if (!LittleFS.exists(url))
  {
    url = "/$update.html";
  }

  server.sendHeader("Location", url, true);
  server.send(302);
} // handleRedirect()

void handleGetData()
{
  File file = LittleFS.open("/data.json", "r");
  server.streamFile(file, "application/json");
  file.close();
}

void handlePostData()
{
  if (server.method() == HTTP_POST)
  {
    Serial.println("handlePostData");

    fancyled();

    String data = server.arg("plain");
    File file = LittleFS.open("/data.json", "w");
    file.print(data);
    file.close();
    server.send(200, "text/plain", "Data on littleFS saved successfully");
  }
}

void handleDeleteData()
{
  if (server.method() == HTTP_DELETE)
  {
    fancyled();

    File file = LittleFS.open("/data.json", "w");
    file.print("{}");
    file.close();
    server.send(200, "text/plain", "Data on littleFS deleted successfully");
  }
}

void handleUpdateData()
{
  if (server.method() == HTTP_PUT)
  {
    String data = server.arg("plain");
    File file = LittleFS.open("/data.json", "w");
    file.print(data);
    file.close();
    server.send(200, "text/plain", "Data on littleFS updated successfully");
  }
}

// API sisi arduino microcontroller

struct Person
{
  String id;
  String name;
  int age;
};

void handleAPI()
{
  File file = LittleFS.open("/bed.html", "r");
  server.streamFile(file, "text/html");
  file.close();
}

void handleBuatData()
{
  if (server.method() == HTTP_POST)
  {
    Serial.println("handleCreateData");

    fancyled();

    String data = server.arg("plain");
    File file = LittleFS.open("/dataku.json", "w");
    file.print(data);
    file.close();
    server.send(200, "text/plain", "Data on littleFS saved successfully");
  }
}

void handleTambah()
{
  File file = LittleFS.open("/dataku.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  String name = server.arg("name");
  int age = server.arg("age").toInt();

  String id = String(data.size());
  Serial.println("id: " + id);

  Person person = {id, name, age};
  JsonObject obj = data.createNestedObject(id);
  obj["id"] = person.id;
  obj["name"] = person.name;
  obj["age"] = person.age;

  File outFile = LittleFS.open("/dataku.json", "w");
  if (!outFile)
  {
    Serial.println("Failed to open file for writing");
    server.send(500, "text/plain", "Failed to open file for writing");
    return;
  }

  serializeJson(data, outFile);

  outFile.close();

  server.send(200, "text/plain", "Record added successfully");
}

void handleDapat()
{
  File file = LittleFS.open("/dataku.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  String output;
  serializeJsonPretty(data, output);

  server.send(200, "application/json", output);
}

void handleDapatById()
{
  File file = LittleFS.open("/dataku.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  // String id = server.pathArg(0);

  int id = server.arg("id").toInt();
  Serial.println(id);

  JsonObject obj = data[id];

  String output;
  serializeJsonPretty(obj, output);

  server.send(200, "application/json", output);
}

void handlePerbarui()
{
  File file = LittleFS.open("/dataku.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  int id = server.arg("id").toInt();

  JsonObject obj = data[id];

  String name = server.arg("name");
  int age = server.arg("age").toInt();

  String roomname = server.arg("roomname");
  String bedname = server.arg("bedname");
  bool status = false;

  obj["name"] = name;
  obj["age"] = age;

  obj["roomname"] = roomname;
  obj["bedname"] = bedname;
  obj["status"] = status;

  File outFile = LittleFS.open("/dataku.json", "w");
  if (!outFile)
  {
    Serial.println("Failed to open file for writing");
    server.send(500, "text/plain", "Failed to open file for writing");
    return;
  }

  serializeJson(data, outFile);

  outFile.close();

  server.send(200, "text/plain", "Record updated successfully");
}

void handleHapus()
{
  File file = LittleFS.open("/dataku.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());
  // Serial.println(buf.get());
  // Serial.println(data);

  int id = server.arg("id").toInt();
  // Serial.println("id: " + id);

  data.remove(id);

  File outFile = LittleFS.open("/dataku.json", "w");
  if (!outFile)
  {
    Serial.println("Failed to open file for writing");
    server.send(500, "text/plain", "Failed to open file for writing");
    return;
  }

  serializeJson(data, outFile);

  outFile.close();

  server.send(200, "text/plain", "Record deleted successfully");
}

void handleCall()
{
  File file = LittleFS.open("/dataku.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  int id = server.arg("id").toInt();

  JsonObject obj = data[id];

  bool status;
  // String roomname = server.arg("roomname");
  // String bedname = server.arg("bedname");
  if (server
          .arg("status") == "1")
  {
    status = true;
  }
  else
  {
    status = false;
  }

  // obj["roomname"] = roomname;
  // obj["bedname"] = bedname;
  obj["status"] = status;

  File outFile = LittleFS.open("/dataku.json", "w");
  if (!outFile)
  {
    Serial.println("Failed to open file for writing");
    server.send(500, "text/plain", "Failed to open file for writing");
    return;
  }

  serializeJson(data, outFile);

  serializeJsonPretty(data, Serial);

  outFile.close();

  server.send(200, "text/plain", "Record updated successfully");
}

int dBmtoPercentage(int dBm)
{
  int quality;
  if (dBm <= RSSI_MIN)
  {
    quality = 0;
  }
  else if (dBm >= RSSI_MAX)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (dBm + 100);
  }

  return quality;
}

int bytestoKB(int bkb)
{
  return bkb = bkb / 1024;
}

void hwInit()
{
  Serial.println("hwInit");

  String FreeHeap = String(ESP.getFreeHeap());
  String SketchSize = String(ESP.getSketchSize());

  String ChipRealSize = String(ESP.getFlashChipRealSize());
  String VendorID = String(ESP.getFlashChipVendorId());
  String FlashChipID = String(ESP.getFlashChipId());
  String CPUFreqMhz = String(ESP.getCpuFreqMHz());
  uint16_t v = ESP.getVcc();
  // float vcc = ((float)v / 1024.0f);
  // char v_str[10];
  // dtostrf(vcc, 5, 3, v_str);
  // sprintf(v_str, "%s V", v_str);
  // Serial.println(v_str);

  String VCC = String(v);

  String WiFiSSID = String(WiFi.SSID());
  String LocalIP = WiFi.localIP().toString();
  String SubnetMask = WiFi.subnetMask().toString();
  String GateWay = WiFi.gatewayIP().toString();
  String DNSIP = WiFi.dnsIP().toString();
  String MACAddressHW = WiFi.macAddress();

  LittleFS.info(fs_info);
  String TotalSize = String(fs_info.totalBytes);
  String UsedSize = String(fs_info.usedBytes);

  int TotalSizeKB = bytestoKB(TotalSize.toInt());

  int FreeSpaces = ChipRealSize.toInt() - TotalSize.toInt() - SketchSize.toInt();
  float FreeSpacesPercent = FreeSpaces / ChipRealSize.toFloat();
  float TotalSizePercent = TotalSize.toFloat() / ChipRealSize.toFloat();
  float SketchSizePercent = SketchSize.toFloat() / ChipRealSize.toFloat();

  int FreeSpacesPercentInt = FreeSpacesPercent * 100;
  int TotalSizePercentInt = TotalSizePercent * 100;
  int SketchSizePercentInt = SketchSizePercent * 100;

  String FreeSpace = String(FreeSpaces);

  WiFiStrength = String(dBmtoPercentage(WiFi.RSSI()));

  File file = LittleFS.open("/system.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  // int id = server.arg("id").toInt();

  JsonObject obj = data.to<JsonObject>();

  obj["FreeHeap"] = FreeHeap;
  obj["SketchSize"] = SketchSize;
  obj["FlashChipID"] = FlashChipID;
  obj["ChipRealSize"] = ChipRealSize;
  obj["FreeSpace"] = FreeSpace;
  obj["TotalSize"] = TotalSize;
  obj["TotalSizeKB"] = String(TotalSizeKB);
  obj["UsedSize"] = UsedSize;
  obj["FreeSpacesPercent"] = String(FreeSpacesPercentInt);
  obj["TotalSizePercent"] = String(TotalSizePercentInt);
  obj["SketchSizePercent"] = String(SketchSizePercentInt);
  obj["VendorID"] = VendorID;
  obj["CPUFreqMhz"] = CPUFreqMhz;
  obj["VCC"] = VCC;
  obj["WiFiSSID"] = WiFiSSID;
  obj["LocalIP"] = LocalIP;
  obj["SubnetMask"] = SubnetMask;
  obj["GateWay"] = GateWay;
  obj["DNSIP"] = DNSIP;
  obj["MACAddressHW"] = MACAddressHW;
  obj["TotalSize"] = TotalSize;
  obj["UsedSize"] = UsedSize;
  obj["WiFiStrength"] = WiFiStrength;
  obj["Days"] = Days;
  obj["Hours"] = Hours;
  obj["Minutes"] = Minutes;
  obj["Seconds"] = Seconds;
  obj["MilliSeconds"] = Milliseconds;

  File outFile = LittleFS.open("/system.json", "w");
  if (!outFile)
  {
    Serial.println("Failed to open file for writing");
    // server.send(500, "text/plain", "Failed to open file for writing");
    return;
  }

  serializeJson(data, outFile);
  serializeJsonPretty(data, Serial);
  outFile.close();
  fancyled();
}

void handleSystem()
{
  hwInit();

  File file = LittleFS.open("/system.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  DynamicJsonDocument data(1024);
  deserializeJson(data, buf.get());

  String output;
  serializeJsonPretty(data, output);

  server.send(200, "application/json", output);
}

void handleRestart()
{
  server.send(200, "text/plain", "ESP Restart");
  fancyled();
  delay(500);
  ESP.reset();
}

bool UPTime(void *)
{

  Serial.print("days: ");
  Serial.println(uptime::getDays());
  Days = uptime::getDays();

  Serial.print("hours: ");
  Serial.println(uptime::getHours());
  Hours = uptime::getHours();

  Serial.print("minutes: ");
  Serial.println(uptime::getMinutes());
  Minutes = uptime::getMinutes();

  Serial.print("seconds: ");
  Serial.println(uptime::getSeconds());
  Seconds = uptime::getSeconds();

  Serial.print("milliseconds: ");
  Serial.println(uptime::getMilliseconds());
  Milliseconds = uptime::getMilliseconds();

  Serial.print("\n");

  return true;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{

  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;

  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
    // send message to client
    webSocket.sendTXT(num, "0");
  }
  break;

  case WStype_TEXT:
    Serial.printf("[%u] get Text: %s\n", num, payload);
    // send message to client
    // webSocket.sendTXT(num, "message here");
    // send data to all connected clients
    // webSocket.broadcastTXT("message here");
    break;

  case WStype_BIN:
    Serial.printf("[%u] get binary length: %u\n", num, length);
    hexdump(payload, length);
    // send message to client
    // webSocket.sendBIN(num, payload, length);
    break;
  }
}

void setup()
{
  // the usual Serial stuff....
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\nESP8266WebFlMgr Demo basic");

  ESPxWebFlMgr_FileSystem.begin();

  // login into WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.print("Open Filemanager with http://");
    Serial.print(WiFi.localIP());
    Serial.print(":");
    Serial.print(filemanagerport);
    Serial.print("/");
    Serial.println();
  }

  pinMode(blue, OUTPUT);

  ArduinoOTA.onStart([]()
                     {
    analogWrite(blue, 0);
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type); });
  ArduinoOTA.onEnd([]()
                   { 
                    fancyled();
                    Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    } });
  ArduinoOTA.begin();

  server.on("/", HTTP_GET, handleRedirect);
  server.on("/get-data", handleGetData);
  server.on("/post-data", handlePostData);
  server.on("/delete-data", handleDeleteData);
  server.on("/update-data", handleUpdateData);

  // API sisi arduino microcontroller

  server.on("/api", handleAPI);
  server.on("/add", HTTP_POST, handleTambah);
  server.on("/create", HTTP_POST, handleBuatData);
  server.on("/get", handleDapat);
  server.on("/getid", handleDapatById);
  server.on("/updateid", HTTP_POST, handlePerbarui);
  server.on("/deleteid", HTTP_POST, handleHapus);
  server.on("/call", handleCall);
  server.on("/system", handleSystem);
  server.on("/restart", handleRestart);

  // UPLOAD and DELETE of files in the file system using a request handler.
  // server.addHandler(new FileServerHandler());

  // enable CORS header in webserver results
  server.enableCORS(true);

  // enable ETAG header in webserver results from serveStatic handler
  server.enableETag(true);

  // serve all static files
  server.serveStatic("/index.html", LittleFS, "/index.html");
  server.serveStatic("/", LittleFS, "/index.html");
  server.serveStatic("/main.js", LittleFS, "/main.js");
  server.serveStatic("/styles.css", LittleFS, "/styles.css");
  server.serveStatic("/api", LittleFS, "/bed.html");
  server.serveStatic("/bed.js", LittleFS, "/bed.js");
  server.serveStatic("/bed.css", LittleFS, "/bed.css");

  // handle cases when file is not found
  // server.onNotFound([](){
  // standard not found in browser.
  // server.send(404, "text/html", FPSTR(notFoundContent)); });

  server.begin();
  filemgr.begin();
  hwInit();

  // call the toggle_led function every 1000 millis (1 second)
  timer.every(1000, UPTime);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  filemgr.handleClient();
  delay(2);
  server.handleClient();
  delay(2);
  ArduinoOTA.handle();

  // First call calculate_uptime() to calculate the uptime
  // and then read the uptime variables.
  uptime::calculateUptime();

  timer.tick();

  webSocket.loop();

  webSocket.broadcastTXT(Milliseconds);

  unsigned long currentMillis = millis();

  if (currentMillis - previousBlue >= intervalBlue)
  {
    // save this reading!
    previousBlue = currentMillis;

    // figure out if you should turn the LED on or off
    if (blueState == LOW)
    {
      blueState = HIGH;
    }
    else
    {
      blueState = LOW;
    }
    digitalWrite(blue, blueState);
  }
}