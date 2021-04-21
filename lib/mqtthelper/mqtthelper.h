#ifndef MQTTHELPER_H
#define MQTTHELPER_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

typedef struct
{
  uint16_t brightness;
  bool show_seconds;
  uint16_t page_duration;
  bool animate_colon;
} mqtt_settings;

class MQTTHelper
{
private:
  WiFiClient wifiClient;
  PubSubClient mqttClient;

  const char *client_id;

  uint16_t settings_brightness;
  bool settings_show_seconds;
  uint16_t settings_page_duration;
  bool settings_animate_colon;

  float temperature_indoor;
  float temperature_outdoor;

public:
  MQTTHelper(const char *server, uint16_t port, const mqtt_settings &defaults, const char *client_id);

  void loop();
  void callback(char *topic, byte *payload, unsigned int length);

  const mqtt_settings getSettings() const;

  const uint16_t &getBrightness() const;
  const bool &getShowSeconds() const;
  const uint16_t &getPageDuration() const;
  const bool &getAnimateColon() const;

  const float &getTemperatureIndoor() const;
  const float &getTemperatureOutdoor() const;

private:
  void subscribe();
};

#endif
