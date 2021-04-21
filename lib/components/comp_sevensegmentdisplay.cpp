#include "components.h"

const uint8_t seg_mask_a = 1 << 7;
const uint8_t seg_mask_b = 1 << 6;
const uint8_t seg_mask_c = 1 << 5;
const uint8_t seg_mask_d = 1 << 4;
const uint8_t seg_mask_e = 1 << 3;
const uint8_t seg_mask_f = 1 << 2;
const uint8_t seg_mask_g = 1 << 1;

const uint8_t digitSegMasks[] = {
    0b11111100, // 0 ABCDEF_-
    0b01100000, // 1 _BC____-
    0b11011010, // 2 AB_DE_G-
    0b11110010, // 3 ABCD__G-
    0b01100110, // 4 _BC__FG-
    0b10110110, // 5 A_CD_FG-
    0b10111110, // 6 A_CDEFG-
    0b11100000, // 7 ABC____-
    0b11111110, // 8 ABCDEFG-
    0b11110110, // 9 ABCD_FG-
};

uint8_t digitToSegmentMask(uint8_t digit)
{
  uint8_t seg_mask = 0x00;

  if (digit < 10)
  {
    seg_mask = digitSegMasks[digit];
  }

  return seg_mask;
}

void drawSegmentA(SevenSegmentDisplay &display, Drawable &drawable)
{
  drawable.drawHorizontalLine(1,
                              0,
                              display.getSegmentLength(),
                              display.getColor());
}

void drawSegmentB(SevenSegmentDisplay &display, Drawable &drawable)
{
  drawable.drawVerticalLine(display.getSegmentLength() + 1,
                            1,
                            display.getSegmentLength(),
                            display.getColor());
}

void drawSegmentC(SevenSegmentDisplay &display, Drawable &drawable)
{
  drawable.drawVerticalLine(display.getSegmentLength() + 1,
                            display.getSegmentLength() + 2,
                            display.getSegmentLength(),
                            display.getColor());
}

void drawSegmentD(SevenSegmentDisplay &display, Drawable &drawable)
{
  drawable.drawHorizontalLine(1,
                              2 * display.getSegmentLength() + 2,
                              display.getSegmentLength(),
                              display.getColor());
}

void drawSegmentE(SevenSegmentDisplay &display, Drawable &drawable)
{
  drawable.drawVerticalLine(0,
                            display.getSegmentLength() + 2,
                            display.getSegmentLength(),
                            display.getColor());
}

void drawSegmentF(SevenSegmentDisplay &display, Drawable &drawable)
{
  drawable.drawVerticalLine(0,
                            1,
                            display.getSegmentLength(),
                            display.getColor());
}

void drawSegmentG(SevenSegmentDisplay &display, Drawable &drawable)
{
  drawable.drawHorizontalLine(1,
                              display.getSegmentLength() + 1,
                              display.getSegmentLength(),
                              display.getColor());
}

void drawSegments(SevenSegmentDisplay &display, Drawable &drawable, uint8_t seg_mask)
{
  if ((seg_mask & seg_mask_a) != 0)
    drawSegmentA(display, drawable);
  if ((seg_mask & seg_mask_b) != 0)
    drawSegmentB(display, drawable);
  if ((seg_mask & seg_mask_c) != 0)
    drawSegmentC(display, drawable);
  if ((seg_mask & seg_mask_d) != 0)
    drawSegmentD(display, drawable);
  if ((seg_mask & seg_mask_e) != 0)
    drawSegmentE(display, drawable);
  if ((seg_mask & seg_mask_f) != 0)
    drawSegmentF(display, drawable);
  if ((seg_mask & seg_mask_g) != 0)
    drawSegmentG(display, drawable);
}

bool morph0To1(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentB(display, drawable);
  drawSegmentC(display, drawable);

  drawable.drawVerticalLine(display.getFrame() + 1, 1, display.getSegmentLength(), display.getColor());
  drawable.drawVerticalLine(display.getFrame() + 1, display.getSegmentLength() + 2, display.getSegmentLength(), display.getColor());

  if (display.getFrame() + 1 < display.getSegmentLength())
  {
    drawable.drawHorizontalLine(display.getFrame() + 2, 0, display.getSegmentLength() - display.getFrame() - 1, display.getColor());
    drawable.drawHorizontalLine(display.getFrame() + 2, 2 * display.getSegmentLength() + 2, display.getSegmentLength() - display.getFrame() - 1, display.getColor());
  }

  return false;
}

bool morph1To2(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentB(display, drawable);

  drawable.drawVerticalLine(display.getSegmentLength() - display.getFrame(), display.getSegmentLength() + 2, display.getSegmentLength(), display.getColor());

  if (display.getFrame() > 0)
  {
    drawable.drawHorizontalLine(display.getSegmentLength() - display.getFrame() + 1, 0, display.getFrame(), display.getColor());
    drawable.drawHorizontalLine(display.getSegmentLength() - display.getFrame() + 1, display.getSegmentLength() + 1, display.getFrame(), display.getColor());
    drawable.drawHorizontalLine(display.getSegmentLength() - display.getFrame() + 1, 2 * display.getSegmentLength() + 2, display.getFrame(), display.getColor());
  }

  return false;
}

bool morph2To3(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentA(display, drawable);
  drawSegmentB(display, drawable);
  drawSegmentD(display, drawable);
  drawSegmentG(display, drawable);

  drawable.drawVerticalLine(display.getFrame() + 1, display.getSegmentLength() + 2, display.getSegmentLength(), display.getColor());

  return false;
}

bool morph3To4(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentB(display, drawable);
  drawSegmentC(display, drawable);
  drawSegmentG(display, drawable);

  drawable.drawVerticalLine(0, 1, display.getFrame() + 1, display.getColor());
  drawable.drawHorizontalLine(1, 0, display.getSegmentLength() - display.getFrame() - 1, display.getColor());

  drawable.drawHorizontalLine(display.getFrame() + 2, 2 * display.getSegmentLength() + 2, display.getSegmentLength() - display.getFrame() - 1, display.getColor());

  return false;
}

bool morph4To5(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentC(display, drawable);
  drawSegmentF(display, drawable);
  drawSegmentG(display, drawable);

  drawable.drawVerticalLine(0, 1, display.getSegmentLength() - display.getFrame() - 1, display.getColor());
  drawable.drawHorizontalLine(1, 0, display.getFrame() + 1, display.getColor());

  drawable.drawVerticalLine(display.getSegmentLength() + 1, 2 + display.getFrame(), display.getSegmentLength() - display.getFrame() - 1, display.getColor());
  drawable.drawHorizontalLine(display.getSegmentLength() - display.getFrame(), 2 * display.getSegmentLength() + 2, display.getFrame() + 1, display.getColor());

  return false;
}

bool morph5To6(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentA(display, drawable);
  drawSegmentC(display, drawable);
  drawSegmentD(display, drawable);
  drawSegmentF(display, drawable);
  drawSegmentG(display, drawable);

  drawable.drawVerticalLine(0, display.getSegmentLength() + 2, display.getFrame() + 1, display.getColor());

  return false;
}

bool morph5To0(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentA(display, drawable);
  drawSegmentC(display, drawable);
  drawSegmentD(display, drawable);
  drawSegmentF(display, drawable);

  drawable.drawVerticalLine(display.getSegmentLength() + 1, 1, display.getFrame() + 1, display.getColor());

  drawable.drawVerticalLine(0, display.getSegmentLength() + 2, display.getFrame() + 1, display.getColor());
  drawable.drawHorizontalLine(1, display.getSegmentLength() + 1, display.getSegmentLength() - display.getFrame() - 1, display.getColor());

  return false;
}

bool morph6To7(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentA(display, drawable);
  drawSegmentC(display, drawable);

  drawable.drawVerticalLine(display.getFrame() + 1, 1, display.getSegmentLength(), display.getColor());
  drawable.drawVerticalLine(display.getFrame() + 1, display.getSegmentLength() + 2, display.getSegmentLength(), display.getColor());

  if (display.getFrame() + 1 < display.getSegmentLength())
  {
    drawable.drawHorizontalLine(display.getFrame() + 2, display.getSegmentLength() + 1, display.getSegmentLength() - display.getFrame() - 1, display.getColor());
    drawable.drawHorizontalLine(display.getFrame() + 2, 2 * display.getSegmentLength() + 2, display.getSegmentLength() - display.getFrame() - 1, display.getColor());
  }

  return false;
}

bool morph7To8(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentA(display, drawable);
  drawSegmentB(display, drawable);
  drawSegmentC(display, drawable);

  drawable.drawVerticalLine(display.getSegmentLength() - display.getFrame(), 1, display.getSegmentLength(), display.getColor());
  drawable.drawVerticalLine(display.getSegmentLength() - display.getFrame(), display.getSegmentLength() + 2, display.getSegmentLength(), display.getColor());

  if (display.getFrame() > 0)
  {
    drawable.drawHorizontalLine(display.getSegmentLength() + 1 - display.getFrame(), display.getSegmentLength() + 1, display.getFrame(), display.getColor());
    drawable.drawHorizontalLine(display.getSegmentLength() + 1 - display.getFrame(), 2 * display.getSegmentLength() + 2, display.getFrame(), display.getColor());
  }

  return false;
}

bool morph8To9(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentA(display, drawable);
  drawSegmentB(display, drawable);
  drawSegmentC(display, drawable);
  drawSegmentD(display, drawable);
  drawSegmentF(display, drawable);
  drawSegmentG(display, drawable);

  drawable.drawVerticalLine(0, display.getSegmentLength() + 2, display.getSegmentLength() - display.getFrame() - 1, display.getColor());

  return false;
}

bool morph9To0(SevenSegmentDisplay &display, Drawable &drawable)
{
  if (display.getFrame() == display.getSegmentLength())
  {
    return true;
  }

  drawable.clear();
  drawSegmentA(display, drawable);
  drawSegmentB(display, drawable);
  drawSegmentC(display, drawable);
  drawSegmentD(display, drawable);
  drawSegmentF(display, drawable);

  drawable.drawVerticalLine(0, display.getSegmentLength() + 2, display.getFrame() + 1, display.getColor());
  drawable.drawHorizontalLine(1, display.getSegmentLength() + 1, display.getSegmentLength() - display.getFrame() - 1, display.getColor());

  return false;
}

bool SevenSegmentDisplay::animationExists(uint8_t current_value, uint8_t next_value)
{
  if (current_value == 0 && next_value == 1)
    return true;

  if (current_value == 1 && next_value == 2)
    return true;

  if (current_value == 2 && next_value == 3)
    return true;

  if (current_value == 3 && next_value == 4)
    return true;

  if (current_value == 4 && next_value == 5)
    return true;

  if (current_value == 5 && next_value == 6)
    return true;

  if (current_value == 5 && next_value == 0)
    return true;

  if (current_value == 6 && next_value == 7)
    return true;

  if (current_value == 7 && next_value == 8)
    return true;

  if (current_value == 8 && next_value == 9)
    return true;

  if (current_value == 9 && next_value == 0)
    return true;

  return false;
}

void SevenSegmentDisplay::doMorph(SevenSegmentDisplay &display, Drawable &drawable)
{
  bool morph_complete = false;

  if (display.currentValue == 0 && display.nextValue == 1)
  {
    morph_complete = morph0To1(display, drawable);
  }
  else if (display.currentValue == 1 && display.nextValue == 2)
  {
    morph_complete = morph1To2(display, drawable);
  }
  else if (display.currentValue == 2 && display.nextValue == 3)
  {
    morph_complete = morph2To3(display, drawable);
  }
  else if (display.currentValue == 3 && display.nextValue == 4)
  {
    morph_complete = morph3To4(display, drawable);
  }
  else if (display.currentValue == 4 && display.nextValue == 5)
  {
    morph_complete = morph4To5(display, drawable);
  }
  else if (display.currentValue == 5 && display.nextValue == 6)
  {
    morph_complete = morph5To6(display, drawable);
  }
  else if (display.currentValue == 5 && display.nextValue == 0)
  {
    morph_complete = morph5To0(display, drawable);
  }
  else if (display.currentValue == 6 && display.nextValue == 7)
  {
    morph_complete = morph6To7(display, drawable);
  }
  else if (display.currentValue == 7 && display.nextValue == 8)
  {
    morph_complete = morph7To8(display, drawable);
  }
  else if (display.currentValue == 8 && display.nextValue == 9)
  {
    morph_complete = morph8To9(display, drawable);
  }
  else if (display.currentValue == 9 && display.nextValue == 0)
  {
    morph_complete = morph9To0(display, drawable);
  }

  if (morph_complete)
  {
    display.currentValue = display.nextValue;
    display.frame = 0;

    drawStaticNumber(display, drawable);
  }
  else
  {
    display.frame += 1;
  }
}

void SevenSegmentDisplay::drawStaticNumber(SevenSegmentDisplay &display, Drawable &drawable)
{
  uint8_t seg_mask = digitToSegmentMask(display.nextValue);

  drawable.clear();
  drawSegments(display, drawable, seg_mask);
}

SevenSegmentDisplay::SevenSegmentDisplay(const uint8_t value, const bool displayZero, const uint8_t segment_length, uint16_t color)
    : Component(color),
      displayZero(displayZero), segment_length(segment_length), currentValue(value - 1), nextValue(value), frame(0)
{
}

const uint8_t SevenSegmentDisplay::getSegmentLength() const
{
  return segment_length;
}

const uint8_t SevenSegmentDisplay::getFrame() const
{
  return frame;
}

void SevenSegmentDisplay::setValue(uint8_t value)
{
  if (value == nextValue)
    return;

  if (currentValue != nextValue)
  {
    currentValue = nextValue;
    frame = 0;
  }

  this->nextValue = value;
}

void SevenSegmentDisplay::draw(Drawable &drawable)
{
  // TODO: implement
  drawStaticNumber(*this, drawable);
}

void SevenSegmentDisplay::drawNextFrame(Drawable &drawable)
{
  if (currentValue == nextValue)
  {
    return;
  }

  if (!displayZero && nextValue == 0)
  {
    currentValue = nextValue;
    frame = 0;
    drawable.clear();
    return;
  }

  // Wenn keine Animation bekannt ist, dann die neue Zahl direkt Zeichnen
  if (!animationExists(currentValue, nextValue))
  {
    currentValue = nextValue;
    drawStaticNumber(*this, drawable);
  }

  doMorph(*this, drawable);
}
