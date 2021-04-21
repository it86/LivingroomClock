#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Arduino.h>

#include "Panel.h"

class Component
{
protected:
  uint16_t color;

  bool visible_current;
  bool visible_next;

public:
  Component(const uint16_t &color);

  const uint16_t getColor() const;
  const bool isVisible() const;

  void setColor(const uint16_t &color);
  void setVisible(const bool &visible);

  virtual void draw(Drawable &drawable) = 0;
  virtual void drawNextFrame(Drawable &drawable) = 0;
};

class ColonDisplay : public Component
{
private:
  const uint8_t segment_length;

public:
  ColonDisplay(const uint8_t &segment_length = 3, const uint16_t &color = 0xFFFF);

  void draw(Drawable &drawable);
  void drawNextFrame(Drawable &drawable);
};

class SevenSegmentDisplay : public Component
{
public:
  static bool animationExists(uint8_t current_value, uint8_t next_value);
  static void doMorph(SevenSegmentDisplay &display, Drawable &drawable);
  static void drawStaticNumber(SevenSegmentDisplay &display, Drawable &drawable);

private:
  const bool displayZero;
  const uint8_t segment_length;

  uint8_t currentValue;
  uint8_t nextValue;

  uint8_t frame;

public:
  SevenSegmentDisplay(const uint8_t value, const bool displayZero = true, const uint8_t segment_length = 3, const uint16_t color = 0xFFFF);

  const uint8_t getSegmentLength() const;
  const uint8_t getFrame() const;

  void setValue(uint8_t value);

  void draw(Drawable &drawable);
  void drawNextFrame(Drawable &drawable);
};

#endif
