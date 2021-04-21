#ifndef VIDEO_CODEC_H
#define VIDEO_CODEC_H

#ifndef VC_PIXEL_ROWS
#define VC_PIXEL_ROWS 32
#endif

#ifndef VC_PIXEL_COLUMNS
#define VC_PIXEL_COLUMNS 64
#endif

#include "Arduino.h"
#include "PxMatrix.h"

namespace VideoCodec
{

  typedef struct vc_color_s
  {
    byte r;
    byte g;
    byte b;
  } vc_color_t;

  typedef struct vc_image_s
  {
    byte        bits;
    vc_color_t *colors;
    byte       *pixels;
  } vc_image_t;

  typedef struct vc_frame_s
  {
    byte       format;
    uint16_t   duration;
    vc_image_t image;
  } vc_frame_t;

  namespace Helper
  {

    class ByteArrayPosition
    {
    private:
      uint16_t pos_byte;
      uint8_t  pos_bit;

    public:
      ByteArrayPosition(const unsigned int &pos_byte, const byte &pos_bit)
      {
        this->pos_byte = pos_byte;
        this->pos_bit = pos_bit;
      }

      ByteArrayPosition(const ByteArrayPosition &other)
      {
        this->pos_byte = other.pos_byte;
        this->pos_bit = other.pos_bit;
      }

      const unsigned int get_pos_byte() const
      {
        return this->pos_byte;
      }

      const byte get_pos_bit() const
      {
        return this->pos_bit;
      }

      bool operator<(const ByteArrayPosition &other) const
      {
        if (this->pos_byte == other.pos_byte)
        {
          return this->pos_bit < other.pos_bit;
        }

        return this->pos_byte < other.pos_byte;
      }

      bool operator>(const ByteArrayPosition &other) const
      {
        if (this->pos_byte == other.pos_byte)
        {
          return this->pos_bit > other.pos_bit;
        }

        return this->pos_byte > other.pos_byte;
      }

      bool operator==(const ByteArrayPosition &other) const
      {
        return this->pos_byte == other.pos_byte && this->pos_bit == other.pos_bit;
      }

      bool operator!=(const ByteArrayPosition &other) const
      {
        return this->pos_byte != other.pos_byte || this->pos_bit != other.pos_bit;
      }

      ByteArrayPosition &operator+=(const byte rhs)
      {
        this->pos_bit += rhs;

        this->pos_byte += pos_bit / 8;
        this->pos_bit %= 8;

        return *this;
      }
    };

    uint8_t extract_value(byte *data, const unsigned int pos_byte, const byte pos_bit, const byte num_bits);

    void decode_frame(const vc_image_t *frame, PxMATRIX &matrix);

  }

}

#endif
