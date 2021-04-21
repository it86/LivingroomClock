#include "TimeDisplay.h"

//**********************************************************************************************************************
//* TimeDisplay                                                                                                        *
//**********************************************************************************************************************
TimeDisplay::TimeDisplay(const uint16_t &offset_x, const uint16_t &offset_y,
                         const uint16_t &width, const uint16_t &height,
                         const uint8_t segment_length, const uint8_t pixel_size,
                         const uint8_t spacing,
                         const uint16_t color, const uint16_t background_color)
    : settings({offset_x, offset_y, width, height, segment_length, pixel_size, spacing}),
      seg_s_s(8, true, segment_length, color),
      seg_s_t(8, true, segment_length, color),
      seg_m_s(8, true, segment_length, color),
      seg_m_t(8, true, segment_length, color),
      seg_h_s(8, true, segment_length, color),
      seg_h_t(8, false, segment_length, color),
      colon1(segment_length, color),
      colon2(segment_length, color),
      panel(NULL),
      panel_s_s(NULL), panel_s_t(NULL),
      panel_m_s(NULL), panel_m_t(NULL),
      panel_h_s(NULL), panel_h_t(NULL),
      timestamp(0),
      frame(0),
      color(color)
{
  createMainPanel(background_color);
  recreateViewPorts();
}

TimeDisplay::~TimeDisplay()
{
  delete panel_h_t;
  delete panel_h_s;
  delete panel_m_t;
  delete panel_m_s;
  delete panel_s_t;
  delete panel_s_s;

  delete panel_c1;
  delete panel_c2;

  delete panel;
}

void TimeDisplay::setColor(const uint16_t color)
{
  this->color = color;

  seg_s_s.setColor(color);
  seg_s_t.setColor(color);
  seg_m_s.setColor(color);
  seg_m_t.setColor(color);
  seg_h_s.setColor(color);
  seg_h_t.setColor(color);
}

void TimeDisplay::setShowSeconds(const bool show_seconds)
{
  this->show_seconds = show_seconds;

  recreateViewPorts();
}

void TimeDisplay::setAnimateColon(const bool animate_colon)
{
  this->animate_colon = animate_colon;
}

void TimeDisplay::setTime(time_t timestamp)
{
  if (this->timestamp == timestamp)
  {
    return;
  }

  this->timestamp = timestamp;
  this->frame = 0;

  uint8_t h = hour(timestamp);
  uint8_t m = minute(timestamp);
  uint8_t s = second(timestamp);

  seg_h_t.setValue(h / 10);
  seg_h_s.setValue(h % 10);

  seg_m_t.setValue(m / 10);
  seg_m_s.setValue(m % 10);

  seg_s_t.setValue(s / 10);
  seg_s_s.setValue(s % 10);
}

void TimeDisplay::updatePanels()
{
  // colon animation
  if (animate_colon)
  {
    uint8_t s = second(timestamp);
    if (s % 2 == 0)
    {
      colon1.setVisible(true);
      colon2.setVisible(show_seconds);
    }
    else
    {
      colon1.setVisible(false);
      colon2.setVisible(false);
    }
  }
  else
  {
    colon1.setVisible(true);
    colon2.setVisible(show_seconds);
  }

  colon1.drawNextFrame(*panel_c1);

  if (show_seconds)
  {
    colon2.drawNextFrame(*panel_c2);

    seg_s_s.drawNextFrame(*panel_s_s);
    seg_s_t.drawNextFrame(*panel_s_t);
  }

  seg_m_s.drawNextFrame(*panel_m_s);
  seg_m_t.drawNextFrame(*panel_m_t);
  seg_h_s.drawNextFrame(*panel_h_s);
  seg_h_t.drawNextFrame(*panel_h_t);
}

void TimeDisplay::draw(PxMATRIX &matrix)
{
  if (!panel->isModified())
  {
    // return;
  }

  uint16_t y = panel->getOffsetY();
  const uint16_t *buffer = panel->getBuffer();

  for (uint16_t j = 0; j < panel->getPhysicalHeight(); j++, y++)
  {
    for (uint16_t i = 0; i < panel->getPhysicalWidth(); i++)
    {
      matrix.drawPixelRGB565(panel->getOffsetX() + i, y, buffer[j * panel->getPhysicalWidth() + i]);
      // writePixel(x + i, y, pgm_read_word(&bitmap[j * w + i]));
    }
  }

  // matrix.drawRGBBitmap(panel->getOffsetX(),
  //                      panel->getOffsetY(),
  //                      panel->getBuffer(),
  //                      panel->getPhysicalWidth(),
  //                      panel->getPhysicalHeight());
  panel->setModified(false);
}

void TimeDisplay::createMainPanel(const uint16_t &background_color)
{
  // uint16_t sevenSegmentDisplayWidth = settings.getSevenSegmentDisplayWidth();
  // uint16_t sevenSegmentDisplayHeight = settings.getSevenSegmentDisplayHeight();

  // uint16_t panelWidth = (sevenSegmentDisplayWidth * 6) + // 6 Seven-Segment-Displays
  //                       (settings.pixel_size * 2) +      // 2 Colons
  //                       (settings.spacing * 7);          // 7 inbetween Spacings

  panel = create_panel(settings.offset_x, settings.offset_y,
                       settings.width, settings.height,
                       settings.pixel_size, background_color);
}

void TimeDisplay::recreateViewPorts()
{
  const uint16_t height = settings.getSevenSegmentDisplayHeight();
  const uint16_t width = settings.getSevenSegmentDisplayWidth();

  panel->clear();

  uint16_t off_x = 0;
  uint16_t off_y = 0;

  if (show_seconds == false)
  {
    off_x = panel->getPhysicalWidth() - (width * 4) // 4 Seven-Segment-Displays
            - (settings.pixel_size * 1)             // 1 Colon
            - (settings.spacing * 4);               // 4 inbetween Spacings
    off_x /= 2;
  }

  delete panel_h_t;
  panel_h_t = panel->createViewPort(off_x, off_y, width, height);
  seg_h_t.draw(*panel_h_t);

  off_x += width + settings.spacing;
  delete panel_h_s;
  panel_h_s = panel->createViewPort(off_x, off_y, width, height);
  seg_h_s.draw(*panel_h_s);

  off_x += width + settings.spacing;
  delete panel_c1;
  panel_c1 = panel->createViewPort(off_x, off_y, settings.pixel_size, height);
  colon1.draw(*panel_c1);

  off_x += settings.pixel_size + settings.spacing;
  delete panel_m_t;
  panel_m_t = panel->createViewPort(off_x, off_y, width, height);
  seg_m_t.draw(*panel_m_t);

  off_x += width + settings.spacing;
  delete panel_m_s;
  panel_m_s = panel->createViewPort(off_x, off_y, width, height);
  seg_m_s.draw(*panel_m_s);

  // TODO: Rework
  off_x += width + settings.spacing;
  delete panel_c2;
  panel_c2 = panel->createViewPort(off_x, off_y, settings.pixel_size, height);

  off_x += settings.pixel_size + settings.spacing;
  delete panel_s_t;
  panel_s_t = panel->createViewPort(off_x, off_y, width, height);

  off_x += width + settings.spacing;
  delete panel_s_s;
  panel_s_s = panel->createViewPort(off_x, off_y, width, height);
}