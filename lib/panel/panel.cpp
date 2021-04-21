#include "panel.h"

//**********************************************************************************************************************
//* Panel                                                                                                              *
//**********************************************************************************************************************
Panel::Panel(const uint16_t &offset_x, const uint16_t &offset_y, const uint16_t &width, const uint16_t &height, const uint8_t &pixel_size, const uint16_t &background_color)
    : offset_x(offset_x), offset_y(offset_y), width(width), height(height), pixel_size(pixel_size),
      buffer(new uint16_t[width * height]), background_color(background_color),
      modified(false)
{
  // call clear, so that the panel is filled with its background color
  clear();
}

Panel::~Panel()
{
  delete buffer;
}

const uint16_t &Panel::getOffsetX() const
{
  return offset_x;
}

const uint16_t &Panel::getOffsetY() const
{
  return offset_y;
}

const uint16_t &Panel::getPhysicalWidth() const
{
  return width;
}

const uint16_t &Panel::getPhysicalHeight() const
{
  return height;
}

const uint8_t &Panel::getPixelSize() const
{
  return pixel_size;
}

const uint16_t *Panel::getBuffer() const
{
  return buffer;
}

const bool Panel::isModified() const
{
  return modified;
}

void Panel::setModified(const bool modified)
{
  this->modified = modified;
}

void Panel::clear()
{
  for (uint16_t i = 0; i < width * height; i++)
  {
    buffer[i] = background_color;
  }

  modified = true;
}

void Panel::clear(const uint16_t &offset_x, const uint16_t &offset_y, const uint16_t &width, const uint16_t &height)
{
  for (uint16_t y = offset_y; y < offset_y + height; y++)
  {
    for (uint16_t x = offset_x; x < offset_x + width; x++)
    {
      buffer[y * this->width + x] = background_color;
    }
  }

  modified = true;
}

ViewPort *Panel::createViewPort(const uint16_t &offset_x,
                                const uint16_t &offset_y,
                                const uint16_t &width,
                                const uint16_t &height)
{
  return new ViewPort(offset_x, offset_y, width, height, this);
}

//**********************************************************************************************************************
//* PanelSizeOne                                                                                                       *
//**********************************************************************************************************************
PanelSizeOne::PanelSizeOne(const uint16_t &offset_x, const uint16_t &offset_y, const uint16_t &width, const uint16_t &height, const uint16_t &background_color)
    : Panel(offset_x, offset_y, width, height, 1, background_color)
{
}

const uint16_t PanelSizeOne::getLogicalWidth() const
{
  return width;
}

const uint16_t PanelSizeOne::getLogicalHeight() const
{
  return height;
}

void PanelSizeOne::drawPixel(const uint16_t &x, const uint16_t &y, const uint16_t &color)
{
  buffer[y * width + x] = color;

  modified = true;
}

void PanelSizeOne::drawPixelAbsolutOffset(const uint16_t &x, const uint16_t &y,
                                          const uint16_t &color,
                                          const uint16_t &offset_x, const uint16_t &offset_y)
{
  drawPixel(offset_x + x, offset_y + y, color);

  modified = true;
}

//**********************************************************************************************************************
//* PanelSizeTwo                                                                                                       *
//**********************************************************************************************************************
PanelSizeTwo::PanelSizeTwo(const uint16_t &offset_x, const uint16_t &offset_y, const uint16_t &width, const uint16_t &height, const uint16_t &background_color)
    : Panel(offset_x, offset_y, width, height, 2, background_color)
{
}

const uint16_t PanelSizeTwo::getLogicalWidth() const
{
  return width >> 1;
}

const uint16_t PanelSizeTwo::getLogicalHeight() const
{
  return height >> 1;
}

void PanelSizeTwo::drawPixel(const uint16_t &x, const uint16_t &y, const uint16_t &color)
{
  uint16_t idx = y * pixel_size * width + x * pixel_size;

  buffer[idx] = color;
  buffer[idx + 1] = color;
  buffer[idx + width] = color;
  buffer[idx + width + 1] = color;

  modified = true;
}

void PanelSizeTwo::drawPixelAbsolutOffset(const uint16_t &x, const uint16_t &y,
                                          const uint16_t &color,
                                          const uint16_t &offset_x, const uint16_t &offset_y)
{
  uint16_t idx = (y * pixel_size + offset_y) * width + (x * pixel_size + offset_x);

  buffer[idx] = color;
  buffer[idx + 1] = color;
  buffer[idx + width] = color;
  buffer[idx + width + 1] = color;

  modified = true;
}

//**********************************************************************************************************************
//* Factory                                                                                                            *
//**********************************************************************************************************************
Panel *create_panel(const uint16_t &offset_x,
                    const uint16_t &offset_y,
                    const uint16_t &width,
                    const uint16_t &height,
                    const uint8_t &pixel_size,
                    const uint16_t &background_color)
{
  if (pixel_size == 1)
  {
    return new PanelSizeOne(offset_x,
                            offset_y,
                            width,
                            height,
                            background_color);
  }
  else if (pixel_size == 2)
  {
    return new PanelSizeTwo(offset_x,
                            offset_y,
                            width,
                            height,
                            background_color);
  }
  else
  {
    return NULL;
  }
}
