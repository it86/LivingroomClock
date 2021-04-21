#include "video_codec.h"

uint8_t VideoCodec::Helper::extract_value(byte *data, const unsigned int pos_byte, const byte pos_bit, const byte num_bits)
{
  unsigned int _pos_byte = pos_byte;
  byte _pos_bit = pos_bit;
  byte _result = 0;

  byte _bit_mask;
  byte _bit_value;

  for (uint8_t c = 0; c < num_bits; c++)
  {
    _bit_mask = 0b00000001 << _pos_bit;                     // prepare bit_mask
    _bit_value = (data[_pos_byte] & _bit_mask) >> _pos_bit; // extract value
    _result |= _bit_value << c;                             // set bit in result

    _pos_bit++; // increment position

    if (_pos_bit == 8)
    {
      _pos_byte++;
      _pos_bit = 0;
    }
  }

  return _result;
}

void VideoCodec::Helper::decode_frame(const VideoCodec::vc_image_t *frame, PxMATRIX &matrix)
{
  uint16_t bits = matrix.height() * matrix.width() * frame->bits;

  uint16_t end_byte = bits / 8;
  uint8_t end_bit = bits % 8;

  VideoCodec::Helper::ByteArrayPosition pos(0, 0);
  VideoCodec::Helper::ByteArrayPosition end(end_byte, end_bit);

  matrix.clearDisplay();

  for (uint8_t row = 0; row < matrix.height(); row++)
  {
    for (uint8_t column = 0; column < matrix.width(); column++)
    {
      if (!(pos < end))
      {
        continue;
      }

      uint8_t idx = Helper::extract_value(frame->pixels, pos.get_pos_byte(), pos.get_pos_bit(), frame->bits);
      pos += frame->bits;
      vc_color_t color = frame->colors[idx];

      if (color.r == 0 && color.g == 0 && color.b == 0)
      {
        continue;
      }

      uint16_t color_conv = matrix.color565(color.r, color.g, color.b);

      matrix.drawPixel(column, row, color_conv);
    }
  }
}