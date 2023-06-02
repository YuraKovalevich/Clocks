#define USE_NTP_INSTEAD_HTTP 1

#if defined(USE_NTP_INSTEAD_HTTP) && (USE_NTP_INSTEAD_HTTP > 0)
#define _USE_NTP
#else
#define _USE_HTTP
#endif
#define NTP_ADDRESS "ntp.belgim.by"

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Ticker.h>
#include "Time.h"
#include "ws2812b_7segment_lib.h"
#include "console.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const int WIFINUMBER = 3;
String SSIDS[WIFINUMBER] = {"ZTE-c2814c", "dd-wrt", "dd-wrt3"};
String PASSWORDS[WIFINUMBER] = {"Hex7075045!#", "telephone", "telephone3"};
long int WiFiErrors[WIFINUMBER] = {0};
int cycle = 0;
String joyedWiFi = "";
String TimeMinHour = "";
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
bool ifupdate = 0;
byte RGB_strip_color [3] = {0, 0, 0};
const long utcOffsetInSeconds = 10800;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, utcOffsetInSeconds);
Ticker ticker;
Ticker tickerUpdate;
Time T;
COM_console cons;
bool needUpdate = 1;
String HOST = "api.timezonedb.com";
String REQUEST = "/v2/get-time-zone?key=NRTIKLJ7TKF3&format=json&by=zone&zone=Europe/Minsk";
int PORT = 80;

void searchForWifi();
int updateTimeNTP();

void showTime(int x) {
  TimeMinHour = T.getTimeMinHour();
  String TimeSec = ":" + T.getTimeSec();
  String Data = T.getData();

  u8g2.clearBuffer();


  u8g2.setFont(u8g2_font_osb35_tn);
  int MinHourLenth = u8g2.getStrWidth(TimeMinHour.c_str());
  u8g2.drawStr(x + 63 - MinHourLenth / 2, 40, TimeMinHour.c_str());

  u8g2.setFont(u8g2_font_ncenB08_tr);
  int DataLenth = u8g2.getStrWidth(Data.c_str());
  u8g2.drawStr(x + 63 - DataLenth / 2, 58, Data.c_str());

  u8g2.drawHLine(64 - T.getSec(), 63, 2 * T.getSec());

  u8g2.sendBuffer();
}

void secondsTicker() {
  //  u8g2.setFont(u8g2_font_freedoomr25_mn);
  T.plus();
  if (!ifupdate) {
    showTime(0);
    drawStrip();
  }
  else {
    cons.upDate();
    drawStrip();
  }
}

void setup(void) {

  pixels.begin();
  pixels.show();
  drawStrip();

  Serial.begin(115200);
  u8g2.begin();
  Serial.println("seting up...");
  delay(100);

  WiFi.mode(WIFI_STA);
  Serial.println("WiFi.mode(WIFI_STA);");
  searchForWifi();
  timeClient.begin();

  updateTime_();

  tickerUpdate.attach(3623, updateTime_);
  Serial.println("tickerUpdate attached");
}


void loop(void) {
  if (needUpdate) {
#ifdef _USE_NTP
    Serial.println("Starting NTP time update");
    int errCode = updateTimeNTP();
    Serial.println("NTP time update done");
#endif /* _USE_NTP */

#ifdef _USE_HTTP
    Serial.println("Starting HTTP time update");
    int errCode = updateTime();
    Serial.println("HTTP time update done");
#endif /* _USE_HTTP */
  Serial.println("Time update done");
    if (errCode == 0) {

      for (int i = 0; i < WIFINUMBER; i++) {
        if (joyedWiFi == SSIDS[i]) {
          WiFiErrors[i]--;
        }
      }
    }
    
    if ((errCode != 0) && (WiFi.status() != WL_CONNECTED)) {

      for (int i = 0; i < WIFINUMBER; i++) {
        if (joyedWiFi == SSIDS[i]) {
          WiFiErrors[i]++;
          Serial.println(String("Error numbers: ") + WiFiErrors[i]);
        }
      }

      searchForWifi();
    }
    if ((errCode != 0) && (WiFi.status() == WL_CONNECTED)) {
      Serial.println("No Internet connection");
      for (int i = 0; i < WIFINUMBER; i++) {
        if (joyedWiFi == SSIDS[i]) {
          WiFiErrors[i]++;
          cycle++;
          Serial.println(String("Error numbers: ") + WiFiErrors[i]);
          delay(1000);
          if (cycle >= 3) WiFi.disconnect();
        }
      }


    }
    delay(500);
  }
  else {
    delay(1593);
  }
}


int tryToConnectWifi(int SSID_, int PASS_)
{
  String  SSID_0 = SSIDS[SSID_];
  String  PASS_0 = PASSWORDS[PASS_];

  Serial.println("SSID: " + SSID_0);
  Serial.println("Priority: " + String(WiFiErrors[SSID_]));
  WiFi.begin(SSID_0.c_str(), PASS_0.c_str());
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    counter++;
    if (counter > 20)
    {
      WiFiErrors[SSID_] += 10;
      return -1;
    }
  }
  joyedWiFi = WiFi.SSID();
  cycle = 0;
  return 0;
}

void searchForWifi() {
  cons.showStr("searching for Wi-Fi");
  Serial.println("searching for Wi-Fi");

  int priorityWiFi [WIFINUMBER];
  for (int i = 0; i < WIFINUMBER; i++) {
    priorityWiFi[i] = i;
  }

  for (int n = 0; n < WIFINUMBER; n++) {
    for (int m = 0; m < WIFINUMBER - 1; m++) {
      if (WiFiErrors[priorityWiFi[m]] > WiFiErrors[priorityWiFi[m + 1]]) {
        int temp = priorityWiFi[m];
        priorityWiFi[m] = priorityWiFi[m + 1];
        priorityWiFi[m + 1] = temp;
      }
    }
  }

  for (int i = 0; (WiFi.status() != WL_CONNECTED); i++) {
    if (i == WIFINUMBER) {
      i = 0;
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("------------------- " + String(i));
      tryToConnectWifi(priorityWiFi[i], priorityWiFi[i]);
    }
  }
  delay(500);
  Serial.println("Wi-Fi connected");
  Serial.println("SSID: " + WiFi.SSID());
  //  cons.showStr("IP: " + WiFi.localIP());
}

#ifdef _USE_HTTP
int updateTime() {
  ifupdate = 1;
  WiFiClient client;
  if (!client.connect(HOST, PORT)) {
    Serial.println("connection failed");
    return 2;
  }
  Serial.println("Requesting URL: ");

  client.print(String("GET ") + REQUEST + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long last = millis();
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return 1;
    }
  }

  // Read all the lines of the reply from server and print them to
  while (client.available()) {
    String line = client.readStringUntil('\r');
    int length = line.length();
    if (length > 200) {
      String info = line.substring(length - 2, length - 21);
      Serial.println(info);
      int year_ = info.substring(4, 0).toInt();
      int month_ = info.substring(7, 5).toInt();
      int day_ = info.substring(10, 8).toInt();
      int hour_ = info.substring(13, 11).toInt();
      int min_ = info.substring(16, 14).toInt();
      int sec_ = info.substring(19, 17).toInt();

      ticker.detach();
      Serial.println("ticker detached");
      T.setTime(year_, month_, day_, hour_, min_, sec_);
    }
  }
  T.corrSec(int((millis() - last) / 1000));
  ticker.attach(1, secondsTicker);
  needUpdate = 0;
  ifupdate = 0;
  return 0;
}
#endif /* _USE_HTTP */

void updateTime_() {
  needUpdate = 1;
}

uint32_t hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
  uint8_t f = (h % 60) * 255 / 60;
  uint8_t p = (255 - s) * (uint16_t)v / 255;
  uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
  uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
  uint8_t r = 0, g = 0, b = 0;
  switch ((h / 60) % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
  }
  RGB_strip_color[0] = r;
  RGB_strip_color[1] = g;
  RGB_strip_color[2] = b;
  return 0;
}


void drawStrip()
{
  long int Tsec = T.getSecOfDay();
  Serial.print(T.getTime().c_str());
  hsvToRgb((uint32_t)(((Tsec / 240) + 180) % 360), 255, 255);
  Serial.println((" H:" + String(Tsec / 240) + " R:" + String(RGB_strip_color[0]) + " G:" + String(RGB_strip_color[1]) + " B:" + String(RGB_strip_color[2])).c_str());
  showString(0, T.getTimeHM().c_str(), RGB_strip_color[0], RGB_strip_color[1], RGB_strip_color[2]);
}

#ifdef _USE_NTP
int updateTimeNTP() {
  unsigned long last = millis();
  timeClient.update();
  Serial.print("NTP: ");
  Serial.print(timeClient.getDay());
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  Serial.println("NTP_end");
  Serial.println(timeClient.getFormattedTime());

 /* int year_ = info.substring(4, 0).toInt();
  int month_ = info.substring(7, 5).toInt();*/
  int day_ = timeClient.getDay();
  int hour_ = timeClient.getHours();
  int min_ = timeClient.getMinutes();
  int sec_ = timeClient.getSeconds();

  ticker.detach();
  Serial.println("ticker detached");
  Serial.print("Setting time to:");
  Serial.print(hour_);
  Serial.print(":");
  Serial.print(min_);
  Serial.print(":");
  Serial.print(sec_);
  Serial.println("");
  //int a = T.setTime(2000, 1, 1, hour_, min_, sec_);
 // T.setHour(hour_);
  Serial.print("Time updated:");
  T.hour_ = hour_;
  T.min_ = min_;
  T.sec_ = sec_;
//  T.corrSec(int((millis() - last) / 1000));
  Serial.print("Time corrected:");
  ticker.attach(1, secondsTicker);
  needUpdate = 0;
  ifupdate = 0;
  return 0;
}
#endif /* _USE_NTP */
