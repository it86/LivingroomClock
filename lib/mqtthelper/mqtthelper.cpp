#include "MQTTHelper.h"

static const char *topic_settings_brightness    = "livingroom/clock/settings/brightness/set";
static const char *topic_settings_show_seconds  = "livingroom/clock/settings/show_seconds/set";
static const char *topic_settings_page_duration = "livingroom/clock/settings/page_duration/set";
static const char *topic_settings_animate_colon = "livingroom/clock/settings/animate_colon/set";
static const char *topic_temperature_indoor     = "livingroom/clock/temperature_indoor/set";
static const char *topic_temperature_outdoor    = "livingroom/clock/temperature_outdoor/set";

static MQTTHelper *instance = NULL;

void message_callback(char *topic, byte *payload, unsigned int length)
{
  instance->callback(topic, payload, length);
}

MQTTHelper::MQTTHelper(const char *server, const uint16_t port, const mqtt_settings &defaults, const char *client_id)
    : wifiClient(),
      mqttClient(server, port, wifiClient),
      client_id(client_id),
      settings_brightness(defaults.brightness),
      settings_show_seconds(defaults.show_seconds),
      settings_page_duration(defaults.page_duration),
      settings_animate_colon(defaults.animate_colon)
{
  instance = this;
  mqttClient.setCallback(message_callback);
}

void MQTTHelper::loop()
{
  // TODO: rewrite
  if (!mqttClient.connected())
  {
    if (!mqttClient.connect(client_id))
    {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
    else
    {
      subscribe();
    }
  }
  else
  {
    mqttClient.loop();
  }
}

void MQTTHelper::callback(char *topic, byte *payload, unsigned int length)
{
  // convert message to c-string
  char *message = new char[length + 1];

  for (unsigned int i = 0; i < length; i++)
  {
    message[i] = static_cast<char>(payload[i]);
  }

  message[length] = '\0';

  Serial.println("New MQTT Message received:");
  Serial.println(topic);
  Serial.println(message);

  // determine topic and call corresponding handler
  if (strcmp(topic, topic_settings_brightness) == 0)
  {
    settings_brightness = atoi(message);
  }
  else if (strcmp(topic, topic_settings_show_seconds) == 0)
  {
    if (strcmp(message, "ON") == 0)
    {
      settings_show_seconds = true;
    }
    else
    {
      settings_show_seconds = false;
    }
  }
  else if (strcmp(topic, topic_settings_page_duration) == 0)
  {
    settings_page_duration = atoi(message);
  }
  else if (strcmp(topic, topic_settings_animate_colon) == 0)
  {
    if (strcmp(message, "ON") == 0)
    {
      settings_animate_colon = true;
    }
    else
    {
      settings_animate_colon = false;
    }
  }
  else if (strcmp(topic, topic_temperature_indoor) == 0)
  {
    temperature_indoor = atof(message);
  }
  else if (strcmp(topic, topic_temperature_outdoor) == 0)
  {
    temperature_outdoor = atof(message);
  }

  // delete message
  delete message;
}

const mqtt_settings MQTTHelper::getSettings() const
{
  return mqtt_settings{settings_brightness,
                       settings_show_seconds,
                       settings_page_duration,
                       settings_animate_colon};
}

const uint16_t &MQTTHelper::getBrightness() const
{
  return settings_brightness;
}

const bool &MQTTHelper::getShowSeconds() const
{
  return settings_show_seconds;
}

const uint16_t &MQTTHelper::getPageDuration() const
{
  return settings_page_duration;
}

const bool &MQTTHelper::getAnimateColon() const
{
  return settings_animate_colon;
}

const float &MQTTHelper::getTemperatureIndoor() const
{
  return temperature_indoor;
}

const float &MQTTHelper::getTemperatureOutdoor() const
{
  return temperature_outdoor;
}

void MQTTHelper::subscribe()
{
  mqttClient.subscribe(topic_settings_brightness);
  mqttClient.subscribe(topic_settings_show_seconds);
  mqttClient.subscribe(topic_settings_page_duration);
  mqttClient.subscribe(topic_settings_animate_colon);

  mqttClient.subscribe(topic_temperature_indoor);
  mqttClient.subscribe(topic_temperature_outdoor);
}
