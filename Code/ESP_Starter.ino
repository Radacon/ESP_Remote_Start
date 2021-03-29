#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <ESP8266mDNS.h> 
#include <WiFiUdp.h>
//Use This Library: https://github.com/taranais/NTPClient
//To get Month/Day/Year
#include <NTPClient.h>
#include <ArduinoOTA.h>


//Set webserver to serve on port 80
ESP8266WebServer server(80);

//Network Info
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

//Globals r bad 4 u 
String AmandaSchedDateStr = "";
String ConradSchedDateStr = "";
long AmandaSchedDate = 0;
long ConradSchedDate = 0;
long ESP_Reset_Date = 0;

//LocalTimeInfo
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -21600;
const int   daylightOffset_sec = 3600;
//Update once every 12 hours
const long  refresh_int_ms = 43200000;

//Days of the week LuT
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Define Time Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, refresh_int_ms);
 
void setup() {
  init_GPIO();
  
  //Start Serial and Print out Connection Diagnostic Data
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WIFI CONNECTED!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println("dB");

  //Restart if the connection is bad
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(500);
    ESP.restart();
  }
  
  //Update Time
  timeClient.update();
  setLocalTime();

  //MDNS Setup so you can go to http://cars.local when this fires up
  if (!MDNS.begin("cars")) 
  {
    Serial.println("Error setting up MDNS responder!");
    while(1) {
        delay(1000);
        ESP.restart();
    }
  }  

  Serial.print(F("Inizializing FS..."));
  if (SPIFFS.begin()){
    Serial.println(F("done."));
  }else{
    Serial.println(F("fail."));
  }

  //Print out all files on the SPI Flash Card
  //File dir = SPIFFS.open("/","r");
  // Cycle all the content
  //printDirectory(dir,1);
  
  initHandlers();
  server.begin();
  Serial.println("HTTP server started");

  //Start OTA Handler
  init_OTA();


}

  
void loop() 
{
  ArduinoOTA.handle();
  timeClient.update();
  server.handleClient();
  checkforalarms();
}
