#include "components.h"

ColonDisplay::ColonDisplay(const uint8_t &segment_length, const uint16_t &color)
    : Component(color), segment_length(segment_length)
{
}

void ColonDisplay::draw(Drawable &drawable)
{
  if (visible_next)
  {
    uint8_t col_length = segment_length / 2;

    drawable.drawVerticalLine(0, segment_length + 1 - col_length, col_length, color);
    drawable.drawVerticalLine(0, segment_length + 2, col_length, color);
  }
  else
  {
    drawable.clear();
  }
}

void ColonDisplay::drawNextFrame(Drawable &drawable)
{
  if (visible_current == visible_next)
  {
    return;
  }

  draw(drawable);

  visible_current = visible_next;
}
