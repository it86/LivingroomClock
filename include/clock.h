#ifndef CLOCK_H
#define CLOCK_H

#include <Ticker.h>
#include <NTPClient.h>
#include <TimeLib.h>
#include <Timezone.h>

#include "mqtthelper.h"
#include "timedisplay.h"
#include "video_codec.h"

extern const uint16_t number_of_rows;
extern const uint16_t number_of_columns;

extern const uint16_t COLOR_TEXT;
extern const uint16_t COLOR_BORDER;
extern const uint16_t COLOR_BACKGROUND;

extern mqtt_settings settings;

extern MQTTHelper mqttHelper;

extern Ticker display_ticker;

extern PxMATRIX matrixDisplay;

extern TimeDisplay timeDisplay;

extern NTPClient ntpClient;

extern Timezone timezone;

extern void handle_changed_settings(mqtt_settings &new_settings);

extern void display_updater();
extern void time_updater();
extern void text_updater();

extern void drawBorder(const uint16_t color, const uint16_t inset = 0);

#endif
