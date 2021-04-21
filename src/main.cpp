#include <ESP8266WiFi.h> //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <WiFiUdp.h>

#include <DNSServer.h>        //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h> //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>

// https://community.platformio.org/t/adafruit-gfx-lib-will-not-build-any-more-pio-5/15776/5
#include <Adafruit_I2CDevice.h>

#include "clock.h"
#include "video_player.h"

const uint16_t number_of_rows = 32;
const uint16_t number_of_columns = 64;

#define LARGE

#define ROWS 32
#define COLUMNS 64

#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

const char wifiHostname[] = "clock_livingroom_test"; // TODO Change
const char wifiManagerAPName[] = "WallClock";
const char wifiManagerAPPassword[] = "LetMeIn";

#ifdef LARGE
const uint16_t OFFSET_X = 2; // 3 + 3
const uint16_t OFFSET_Y = 2; // 3
const uint16_t WIDTH = 60;
const uint16_t HEIGHT = 14;
const uint8_t SEGMENT_LENGTH = 2; // 5
const uint8_t PIXEL_SIZE = 2;     // 1
const uint8_t SPACING = 1;        // 1
#else
const uint16_t OFFSET_X = 6;      // 3 + 3
const uint16_t OFFSET_Y = 3;      // 3
const uint8_t SEGMENT_LENGTH = 5; // 5
const uint8_t PIXEL_SIZE = 1;     // 1
const uint8_t SPACING = 1;        // 1
#endif

const uint16_t COLOR_TEXT = matrixDisplay.color565(255, 255, 255);
const uint16_t COLOR_BORDER = matrixDisplay.color565(0, 0, 255);
const uint16_t COLOR_BACKGROUND = matrixDisplay.color565(0, 0, 0);



WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP, "pool.ntp.org");

// clock.h
mqtt_settings settings{255, true, 5, true};

MQTTHelper mqttHelper("10.10.20.20", 1883, settings, wifiHostname);

Ticker display_ticker;
Ticker time_ticker;
Ticker text_ticker;

// Timezone
TimeChangeRule tcrCEST = {"CEST", Last, Sun, Mar, 2, 120};    // Daylight time = UTC + 2 hours
TimeChangeRule tcrCET  = {"CET" , Last, Sun, Oct, 2, 60};     // Standard time = UTC + 1 hours
Timezone timezone(tcrCEST, tcrCET);

// NTP Client
// TODO


PxMATRIX matrixDisplay(COLUMNS, ROWS, P_LAT, P_OE, P_A, P_B, P_C, P_D);
TimeDisplay timeDisplay(OFFSET_X, OFFSET_Y, WIDTH, HEIGHT, SEGMENT_LENGTH, PIXEL_SIZE, SPACING, COLOR_TEXT, COLOR_BACKGROUND);






time_t getUTCTime() {
  ntpClient.forceUpdate();
  return ntpClient.getEpochTime();
}

void setup()
{
  Serial.begin(115200);

  /* setup display */
  matrixDisplay.begin(16);
  matrixDisplay.setFastUpdate(true);
  matrixDisplay.setBrightness(settings.brightness);
  matrixDisplay.clearDisplay();
  matrixDisplay.flushDisplay();

  /* enable display output */
  display_ticker.attach_ms(4, display_updater);

  /* begin video playback */
  start_playback(&matrixDisplay);

  /* connect to wifi */
  WiFi.hostname(wifiHostname);
  WiFiManager wifiManager;
  wifiManager.autoConnect(wifiManagerAPName, wifiManagerAPPassword);
  //wifiManager.resetSettings();
  //wifiManager.setAPCallback(configModeCallback);

  /* get time using ntp */
  ntpClient.begin();
  ntpClient.forceUpdate();

  /* get TimeLib ready for use */
  setSyncInterval(15 * 60);     // timespan in seconds
  setSyncProvider(getUTCTime);

  /* get time display ready for use */
  timeDisplay.setShowSeconds(settings.show_seconds);
  timeDisplay.setAnimateColon(settings.animate_colon);
  
  /* wait until video is finished */
  while(!is_playback_done()) {
    delay(100);
  }

  /* extra delay for last frame */
  delay(1000);

  /* get ready to loop */
  matrixDisplay.clearDisplay();
  drawBorder(COLOR_BORDER, 0);



  // startup text
  // drawBorder(COLOR_BORDER, 0);
  // matrixDisplay.setCursor(3, 3);
  // matrixDisplay.setTextColor(COLOR_TEXT);
  // matrixDisplay.setTextSize(1);
  // matrixDisplay.setTextWrap(true);
  // matrixDisplay.print("Startup...");
}



unsigned long last_update_time = 0;
unsigned long last_update_text = 0;

void loop()
{
  unsigned long now = millis();

  // update time display
  if (now - last_update_time > 150L)
  {
    last_update_time = now;

    time_updater();
  }

  // update text display
  if (now - last_update_text > settings.page_duration * 1000L)
  {
    last_update_text = now;

    text_updater();
  }

  // process MQTT Messages
  mqttHelper.loop();

  // process changed settings
  mqtt_settings new_settings = mqttHelper.getSettings();
  handle_changed_settings(new_settings);

  // wait
  delay(5);
}
