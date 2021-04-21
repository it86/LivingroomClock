#ifndef PANEL_H
#define PANEL_H

#include <Arduino.h>

class Drawable;
class ViewPort;
class Panel;

class Drawable
{
public:
  virtual ~Drawable() = default;

  virtual void drawPixel(const uint16_t &x, const uint16_t &y, const uint16_t &color) = 0;

  virtual void drawVerticalLine(const uint16_t &x, const uint16_t &y, const uint16_t &length, const uint16_t &color);
  virtual void drawHorizontalLine(const uint16_t &x, const uint16_t &y, const uint16_t &length, const uint16_t &color);

  virtual void clear() = 0;
};

class ViewPort : public Drawable
{
private:
  const uint16_t offset_x;
  const uint16_t offset_y;
  const uint16_t width;
  const uint16_t height;
  Panel *const panel;

public:
  ViewPort(const uint16_t &offset_x,
           const uint16_t &offset_y,
           const uint16_t &width,
           const uint16_t &height,
           Panel *const panel);

  void drawPixel(const uint16_t &x, const uint16_t &y, const uint16_t &color);

  void clear();
};

class Panel : public Drawable
{
  friend class ViewPort;

protected:
  const uint16_t offset_x;
  const uint16_t offset_y;
  const uint16_t width;
  const uint16_t height;
  const uint8_t pixel_size;

  uint16_t *const buffer;
  uint16_t background_color;

  bool modified;

  Panel(const uint16_t &offset_x,
        const uint16_t &offset_y,
        const uint16_t &width,
        const uint16_t &height,
        const uint8_t &pixel_size,
        const uint16_t &background_color = 0x00);

public:
  virtual ~Panel();

  const uint16_t &getOffsetX() const;
  const uint16_t &getOffsetY() const;

  const uint16_t &getPhysicalWidth() const;
  const uint16_t &getPhysicalHeight() const;

  const uint8_t &getPixelSize() const;

  const uint16_t *getBuffer() const;

  const bool isModified() const;
  void setModified(const bool modified);

  virtual const uint16_t getLogicalWidth() const = 0;
  virtual const uint16_t getLogicalHeight() const = 0;

  void clear();

  virtual void drawPixel(const uint16_t &x, const uint16_t &y, const uint16_t &color) = 0;

  ViewPort *createViewPort(const uint16_t &offset_x,
                           const uint16_t &offset_y,
                           const uint16_t &width,
                           const uint16_t &height);

protected:
  virtual void drawPixelAbsolutOffset(const uint16_t &x, const uint16_t &y,
                                      const uint16_t &color,
                                      const uint16_t &offset_x, const uint16_t &offset_y) = 0;
  void clear(const uint16_t &offset_x, const uint16_t &offset_y, const uint16_t &width, const uint16_t &height);
};

class PanelSizeOne : public Panel
{
public:
  PanelSizeOne(const uint16_t &offset_x,
               const uint16_t &offset_y,
               const uint16_t &width,
               const uint16_t &height,
               const uint16_t &background_color = 0x00);

  const uint16_t getLogicalWidth() const;
  const uint16_t getLogicalHeight() const;

  void drawPixel(const uint16_t &x, const uint16_t &y, const uint16_t &color);

protected:
  void drawPixelAbsolutOffset(const uint16_t &x, const uint16_t &y,
                              const uint16_t &color,
                              const uint16_t &offset_x, const uint16_t &offset_y);
};

class PanelSizeTwo : public Panel
{
public:
  PanelSizeTwo(const uint16_t &offset_x,
               const uint16_t &offset_y,
               const uint16_t &width,
               const uint16_t &height,
               const uint16_t &background_color = 0x00);

  const uint16_t getLogicalWidth() const;
  const uint16_t getLogicalHeight() const;

  void drawPixel(const uint16_t &x, const uint16_t &y, const uint16_t &color);

protected:
  void drawPixelAbsolutOffset(const uint16_t &x, const uint16_t &y,
                              const uint16_t &color,
                              const uint16_t &offset_x, const uint16_t &offset_y);
};

Panel *create_panel(const uint16_t &offset_x,
                    const uint16_t &offset_y,
                    const uint16_t &width,
                    const uint16_t &height,
                    const uint8_t &pixel_size,
                    const uint16_t &background_color = 0x0000);

#endif
