#ifndef TIMEDISPLAY_H
#define TIMEDISPLAY_H

//#define PxMATRIX_double_buffer true

#include <PxMatrix.h>
#include <TimeLib.h>

#include "panel.h"
#include "components.h"

class TimeDisplay
{
private:
  typedef struct
  {
    /* data */
    const uint16_t offset_x;
    const uint16_t offset_y;
    const uint16_t width;
    const uint16_t height;
    const uint8_t segment_length;
    const uint8_t pixel_size;
    const uint8_t spacing;

    /* methods */
    uint16_t getSevenSegmentDisplayWidth() const
    {
      return (1 * segment_length + 2) * pixel_size;
    }

    uint16_t getSevenSegmentDisplayHeight() const
    {
      return (2 * segment_length + 3) * pixel_size;
    }

  } dimensions;

  const dimensions settings;

  SevenSegmentDisplay seg_s_s;
  SevenSegmentDisplay seg_s_t;
  SevenSegmentDisplay seg_m_s;
  SevenSegmentDisplay seg_m_t;
  SevenSegmentDisplay seg_h_s;
  SevenSegmentDisplay seg_h_t;

  ColonDisplay colon1;
  ColonDisplay colon2;

  Panel *panel;

  Drawable *panel_s_s;
  Drawable *panel_s_t;
  Drawable *panel_m_s;
  Drawable *panel_m_t;
  Drawable *panel_h_s;
  Drawable *panel_h_t;

  Drawable *panel_c1;
  Drawable *panel_c2;

  uint32_t timestamp;
  uint8_t frame;

  uint16_t color;
  bool show_seconds;
  bool animate_colon;

public:
  TimeDisplay(const uint16_t &offset_x, const uint16_t &offset_y,
              const uint16_t &width, const uint16_t &height,
              const uint8_t segment_length = 3, const uint8_t pixel_size = 2,
              const uint8_t spacing = 2,
              const uint16_t color = 0xFFFF, const uint16_t background_color = 0x0000);
  ~TimeDisplay();

  void setColor(const uint16_t color);
  void setShowSeconds(const bool show_seconds);
  void setAnimateColon(const bool animate_colon);
  void setTime(time_t timestamp);

  void updatePanels();

  void draw(PxMATRIX &matrix);

private:
  void createMainPanel(const uint16_t &background_color);
  void recreateViewPorts();
};

#endif
