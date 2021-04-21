#include "panel.h"

//**********************************************************************************************************************
//* ViewPort                                                                                                           *
//**********************************************************************************************************************
ViewPort::ViewPort(const uint16_t &offset_x, const uint16_t &offset_y,
                   const uint16_t &width, const uint16_t &height,
                   Panel *const panel)
    : offset_x(offset_x), offset_y(offset_y), width(width), height(height),
      panel(panel)
{
}

void ViewPort::drawPixel(const uint16_t &x, const uint16_t &y, const uint16_t &color)
{
  panel->drawPixelAbsolutOffset(x, y, color, offset_x, offset_y);
}

void ViewPort::clear()
{
  panel->clear(offset_x, offset_y, width, height);
}
