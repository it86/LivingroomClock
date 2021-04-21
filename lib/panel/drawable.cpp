#include "panel.h"

void Drawable::drawVerticalLine(const uint16_t &x, const uint16_t &y, const uint16_t &length, const uint16_t &color)
{
  if (length == 0)
    return;

  for (uint16_t i = y; i < y + length; i++)
  {
    drawPixel(x, i, color);
  }
}

void Drawable::drawHorizontalLine(const uint16_t &x, const uint16_t &y, const uint16_t &length, const uint16_t &color)
{
  if (length == 0)
    return;

  for (uint16_t i = x; i < x + length; i++)
  {
    drawPixel(i, y, color);
  }
}