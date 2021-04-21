#include "clock.h"

char daysOfTheWeek[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

uint8_t page = 0;


void handle_changed_settings(mqtt_settings &new_settings)
{
  // brightness
  if (settings.brightness != new_settings.brightness)
  {
    matrixDisplay.setBrightness(new_settings.brightness);
    settings.brightness = new_settings.brightness;
  }

  // show seconds
  if (settings.show_seconds != new_settings.show_seconds)
  {
    timeDisplay.setShowSeconds(new_settings.show_seconds);
    settings.show_seconds = new_settings.show_seconds;
  }

  // page duration
  if (settings.page_duration != new_settings.page_duration)
  {
    settings.page_duration = new_settings.page_duration;
  }

  // animate colon
  if (settings.animate_colon != new_settings.animate_colon)
  {
    timeDisplay.setAnimateColon(new_settings.animate_colon);
    settings.animate_colon = new_settings.animate_colon;
  }
}

void display_updater()
{
  // default 30
  matrixDisplay.display(50);
}

void time_updater()
{
  timeDisplay.setTime(timezone.toLocal(now()));
  timeDisplay.updatePanels();
  timeDisplay.draw(matrixDisplay);

  matrixDisplay.showBuffer();
}

void text_updater()
{
  const int offsetY = 21;

  for (int x = 2; x < 62; x++)
  {
    for (int y = offsetY; y < offsetY + 8; y++)
    {
      matrixDisplay.drawPixel(x, y, COLOR_BACKGROUND);
    }
  }

  time_t epochTime = timezone.toLocal(now());

  if (page == 0)
  {
    matrixDisplay.setCursor(3, offsetY);
    matrixDisplay.print(day(epochTime));
    matrixDisplay.print(".");
    matrixDisplay.print(month(epochTime));
    matrixDisplay.print(".");
    matrixDisplay.print(year(epochTime));
  }

  if (page == 1)
  {
    matrixDisplay.setCursor(3, offsetY);
    matrixDisplay.print(daysOfTheWeek[weekday(epochTime) - 1]);
  }

  if (page == 2)
  {
    matrixDisplay.setCursor(3, offsetY);
    matrixDisplay.print(mqttHelper.getTemperatureIndoor(), 1);
    matrixDisplay.print((char)248);
    matrixDisplay.print("C");
  }

  if (page == 3)
  {
    matrixDisplay.setCursor(3, offsetY);
    matrixDisplay.print(mqttHelper.getTemperatureOutdoor(), 1);
    matrixDisplay.print((char)248);
    matrixDisplay.print("C");
  }

  page += 1;
  if (page == 4)
  {
    page = 0;
  }
}

void drawBorder(const uint16_t color, const uint16_t inset)
{
  matrixDisplay.clearDisplay();
  //**************************************************
  //* Inset: 0 Pixel                                 *
  //**************************************************
  // horizontal border lines
  matrixDisplay.drawLine(1, 0, 62, 0, color);
  matrixDisplay.drawLine(1, 31, 62, 31, color);

  // vertical border lines
  matrixDisplay.drawLine(0, 1, 0, 30, color);
  matrixDisplay.drawLine(63, 1, 63, 30, color);

  // border edges
  matrixDisplay.drawPixel(1, 1, color);
  matrixDisplay.drawPixel(62, 1, color);
  matrixDisplay.drawPixel(62, 30, color);
  matrixDisplay.drawPixel(1, 30, color);

  // separator
  matrixDisplay.drawLine(1, 17, 62, 17, color);

  // separatpr edges
  matrixDisplay.drawPixel(1, 16, color);
  matrixDisplay.drawPixel(62, 16, color);
  matrixDisplay.drawPixel(62, 18, color);
  matrixDisplay.drawPixel(1, 18, color);

  //  //**************************************************
  //  //* Inset: 1 Pixel                                 *
  //  //**************************************************
  //  // horizontal border lines
  //  display.drawLine(2, 1, 61, 1, COLOR_BORDER);
  //  display.drawLine(2, 30, 61, 30, COLOR_BORDER);
  //
  //  // vertical border lines
  //  display.drawLine(1, 2, 1, 29, COLOR_BORDER);
  //  display.drawLine(62, 2, 62, 29, COLOR_BORDER);
  //
  //  // border edges
  //  display.drawPixel(2, 2, COLOR_BORDER);
  //  display.drawPixel(61, 2, COLOR_BORDER);
  //  display.drawPixel(61, 29, COLOR_BORDER);
  //  display.drawPixel(2, 29, COLOR_BORDER);
  //
  //  // separator
  //  display.drawLine(2, 17, 61, 17, COLOR_BORDER);
  //
  //  // separatpr edges
  //  display.drawPixel( 2, 16, COLOR_BORDER);
  //  display.drawPixel(61, 16, COLOR_BORDER);
  //  display.drawPixel(61, 18, COLOR_BORDER);
  //  display.drawPixel( 2, 18, COLOR_BORDER);
}
