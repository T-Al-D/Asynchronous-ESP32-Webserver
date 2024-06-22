#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#endif
#include <Arduino.h>

#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <HTML.h>
#include "SPIFFS.h"
#include "../.pio/libdeps/heltec_wifi_lora_32_V3/U8g2/src/U8g2lib.h"
#include "../lib/HTML/HTML.h"


// pins for the display
#define oledScl 18
#define oledSda 17
#define oledRst 21

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/oledScl, /* data=*/oledSda, /* reset=*/oledRst);
// U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
// U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 16, /* data=*/ 17, /* reset=*/ U8X8_PIN_NONE);   // ESP32 Thing, pure SW emulated I2C

// FUNCTION DECLARATION
void handleOnConnect(AsyncWebServerRequest *request);
void handleButton1On(AsyncWebServerRequest *request);
void handleButton1Off(AsyncWebServerRequest *request);
void handleButton2On(AsyncWebServerRequest *request);
void handleButton2Off(AsyncWebServerRequest *request);
void handleInputText(AsyncWebServerRequest *request);

void handleNotFound(AsyncWebServerRequest *request);

// GLOBAL VARIABLES

/*display coordinates*/
int displayX = 0;
int displayY = 9;

// class String from Aduino.h
// display words 
String displayResult = "HTTP server active !\n";
// other info for <pre> element
String otherInfo = "";
// name attribute of the <input> element in <forms> element
String paramInput = "param"; 

// current button status
bool button1status = false;
bool button2status = false;

/* Put your SSID & Password */
const char *ssid = "ESP32_Experiment"; // Enter SSID here
const char *password = "12345678";     // Enter Password here

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// declare an object of WebServer library
// Port
AsyncWebServer server(80);

// put your setup code here, to run once:
void setup()
{
  // check for the serial connection
  Serial.begin(9600);
  while (!Serial)
  {
  };
  Serial.println("Serial Test");

  // webserver AP - access point (creates own network)
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_AP);
  delay(100);

  if (!SPIFFS.begin(true))
  {
    Serial.println("Error SPIFFS");
  }
  else
  {
    Serial.println("SPIFF works !");
  }

  // set the HTTP requests
  server.on("/", HTTP_GET, handleOnConnect);
  server.on("/button1on", HTTP_GET, handleButton1On);
  server.on("/button1off", HTTP_GET, handleButton1Off);
  server.on("/button2on", HTTP_GET, handleButton2On);
  server.on("/button2off", HTTP_GET, handleButton2Off);
  server.on("/inputText", HTTP_GET, handleInputText);

  server.onNotFound(handleNotFound);

  // start Server and check for connection
  server.begin();
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("WiFi Failed!");
    /*
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    */
  }
  else
  {
    Serial.println("WiFi ON!");
  }

  // for display
  u8g2.begin();

  Serial.println("Setup finished");
  Serial.println(WiFi.localIP());
}

// put your main code here, to run repeatedly:
void loop()
{

  // displays the first page in the storage
  u8g2.firstPage();

  // displays one page after the next until the end of storage
  do
  {
    // set font
    u8g2.setFont(u8g2_font_9x6LED_tr);

    // set where to write
    u8g2.drawStr(displayX, displayY, displayResult.c_str());

  } while (u8g2.nextPage());
}

void handleOnConnect(AsyncWebServerRequest *request)
{
  button1status = false;
  button2status = false;

  displayResult = "HTTP server active !\n";
  displayResult += "Button1 and Button2 Status: OFF";
  Serial.println("Button1 Status: OFF | Button2 Status: OFF");
  request->send(200, "text/html", SendHTML(button1status, button2status, otherInfo));
}

void handleButton1On(AsyncWebServerRequest *request)
{
  button1status = true;

  displayResult = "Button1 Status: ON";
  Serial.println("Button1 Status: ON");
  request->send(200, "text/html", SendHTML(button1status, button2status, otherInfo));
}

void handleButton1Off(AsyncWebServerRequest *request)
{
  button1status = false;

  displayResult = "Button1 Status: OFF";
  Serial.println("Button1 Status: OFF");
  request->send(200, "text/html", SendHTML(button1status, button2status, otherInfo));
}

void handleButton2On(AsyncWebServerRequest *request)
{
  button2status = true;

  displayResult = "Button2 Status: ON";
  Serial.println("Button2 Status: ON");
  request->send(200, "text/html", SendHTML(button1status, button2status, otherInfo));
}

void handleButton2Off(AsyncWebServerRequest *request)
{
  button2status = false;

  displayResult = "Button2 Status: OFF";
  Serial.println("Button2 Status: OFF");
  request->send(200, "text/html", SendHTML(button1status, button2status, otherInfo));
}

void handleInputText(AsyncWebServerRequest *request)
{
  otherInfo = "";

  if (request->hasParam(paramInput))
  {
    otherInfo = request->getParam(paramInput)->value();
  }

  displayResult = "Text input ..." + otherInfo;
  Serial.println("Text input ..." + otherInfo);
  request->send(200, "text/html", SendHTML(button1status, button2status, otherInfo));
}

void handleNotFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}