// format information gleaned from http://www.daubnet.com/formats/BMP.html

#include "corona.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  typedef unsigned char byte;

  struct RGB {
    byte red;
    byte green;
    byte blue;
  };


  struct Header {
    int file_size;
    int data_offset;
    int width;
    int height;
    int bpp;
    int compression;

    int pitch;  // number of bytes in each scanline
    int image_size;

    auto_array<RGB> palette;
    int palette_size;
  };


  inline read16(byte* c) {
    return (c[1] << 8) + c[0];
  }

  inline read32(byte* c) {
    return (read16(c + 2) << 16) + read16(c);
  }


  bool   ReadHeader(File* file, Header& h);
  bool   ReadInfoHeader(File* file, Header& h);
  bool   ReadPalette(File* file, Header& h);
  Image* DecodeBitmap(File* file, Header& h);

  
  Image* OpenBMP(File* file) {
    Header h;
    if (ReadHeader(file, h) &&
        ReadInfoHeader(file, h) &&
        ReadPalette(file, h)) {

      return DecodeBitmap(file, h);

    } else {

      return 0;

    }
  }


  bool ReadHeader(File* file, Header& h) {
    byte header[14];
    if (file->read(header, 14) != 14) {
      return false;
    }

    // check signature
    if (header[0] != 'B' || header[1] != 'M') {
      return false;
    }

    h.file_size   = read32(header + 2);
    h.data_offset = read32(header + 10);
    return true;
  }


  bool ReadInfoHeader(File* file, Header& h) {
    byte header[40];
    if (file->read(header, 40) != 40) {
      return false;
    }

    int size        = read32(header);
    int width       = read32(header + 4);
    int height      = read32(header + 8);
    int planes      = read16(header + 12);
    int bpp         = read16(header + 14);
    int compression = read32(header + 16);
    int image_size  = read32(header + 20);
    int x_per_m     = read32(header + 24);
    int y_per_m     = read32(header + 28);
    int colors_used = read32(header + 32);
    int colors_imp  = read32(header + 36);

    // sanity check the info header
    if (size != 40 || planes != 1) {
      return false;
    }

    // adjust image_size (if compression == 0, manually calculate image size)
    int line_size = 0;
    if (compression == 0) {
      line_size = (width * bpp + 7) / 8;
      line_size = (line_size + 3) / 4 * 4;  // 32-bit-aligned
      image_size = line_size * height;
    }

    h.width       = width;
    h.height      = height;
    h.bpp         = bpp;
    h.compression = compression;
    h.pitch       = line_size;
    h.image_size  = image_size;

    return true;
  }


  bool ReadPalette(File* file, Header& h) {
    if (h.bpp <= 8) {
      h.palette_size = 1 << h.bpp;
    } else {
      h.palette_size = 0;
    }
    h.palette = new RGB[h.palette_size];

    // read the BMP color table
    const int buffer_size = h.palette_size * 3;
    auto_array<byte> buffer(new byte[buffer_size]);
    if (file->read(buffer, buffer_size) != buffer_size) {
      return false;
    }

    byte* in = buffer;
    RGB* out = h.palette;
    for (int i = 0; i < h.palette_size; ++i) {
      out->blue  = *in++;
      out->green = *in++;
      out->red   = *in++;
      ++in;  // skip alpha
      ++out;
    }

    return true;
  }


  Image* DecodeBitmap(File* file, Header& h) {

    if (!file->seek(h.data_offset, File::BEGIN)) {
      return 0;
    }

    // the raster data stored in the file
    auto_array<byte> raster_data(new byte[h.image_size]);
    if (file->read(raster_data, h.image_size) != h.image_size) {
      return 0;
    }

    // the output pixel buffer (parameter to new SimpleImage)
    auto_array<byte> pixels(new byte[h.width * h.height * 3]);

    if (h.bpp == 1 && h.compression == 0) {

      for (int i = 0; i < h.height; ++i) {
        byte* in = raster_data + i * h.pitch;
        byte* out = pixels + (h.height - i - 1) * h.width * 3;

        int mask = 128;
        for (int j = 0; j < h.width; ++j) {
          *out++ = h.palette[(*in & mask) > 0].red;
          *out++ = h.palette[(*in & mask) > 0].green;
          *out++ = h.palette[(*in & mask) > 0].blue;

          mask >>= 1;
          if (mask == 0) {
            ++in;
            mask = 128;
          }
        }
      }

    } else if (h.bpp == 4 && h.compression == 0) {

      for (int i = 0; i < h.height; ++i) {
        byte* in = raster_data + i * h.pitch;
        byte* out = pixels + (h.height - i - 1) * h.width * 3;

        for (int j = 0; j < h.width / 2; ++j) {
          *out++ = h.palette[*in >> 4].red;
          *out++ = h.palette[*in >> 4].green;
          *out++ = h.palette[*in >> 4].blue;

          *out++ = h.palette[*in & 0x0F].red;
          *out++ = h.palette[*in & 0x0F].green;
          *out++ = h.palette[*in & 0x0F].blue;

          ++in;
        }

        if (h.width % 2) {
          *out++ = h.palette[*in >> 4].red;
          *out++ = h.palette[*in >> 4].green;
          *out++ = h.palette[*in >> 4].blue;
        }
      }

    } else if (h.bpp == 4 && h.compression == 2) {

      // ugh
      // I really don't want to implement this
      return 0;

    } else if (h.bpp == 8 && h.compression == 0) {

      for (int i = 0; i < h.height; ++i) {
        byte* in = raster_data + i * h.pitch;
        byte* out = pixels + (h.height - i - 1) * h.width * 3;

        for (int j = 0; j < h.width; ++j) {
          *out++ = h.palette[*in].red;
          *out++ = h.palette[*in].green;
          *out++ = h.palette[*in].blue;
          ++in;
        }
      }

    } else if (h.bpp == 8 && h.compression == 1) {

      return 0;

    } else if (h.bpp == 16 && h.compression == 0) {

      for (int i = 0; i < h.height; ++i) {
        byte* in = raster_data + i * h.pitch;
        byte* out = pixels + (h.height - i - 1) * h.width * 3;

        for (int j = 0; j < h.width; ++j) {
          int clr = (in[1] << 8) + in[0];
          in += 2;

          *out++ = ((clr >> 10) & 0x1F) << 3;
          *out++ = ((clr >> 5) & 0x1F)  << 3;
          *out++ = (clr & 0x1F)         << 3;
        }
      }

    } else if (h.bpp == 24 && h.compression == 0) {

      for (int i = 0; i < h.height; ++i) {
        byte* in = raster_data + i * h.pitch;
        byte* out = pixels + (h.height - i - 1) * h.width * 3;

        for (int j = 0; j < h.width; ++j) {
          *out++ = *in++;
          *out++ = *in++;
          *out++ = *in++;
        }
      }

    } else if (h.bpp == 32 && h.compression == 0) {

      for (int i = 0; i < h.height; ++i) {
        byte* in = raster_data + i * h.pitch;
        byte* out = pixels + (h.height - i - 1) * h.width * 3;

        for (int j = 0; j < h.width; ++j) {
          *out++ = *in++;
          *out++ = *in++;
          *out++ = *in++;
          ++in;  // skip alpha
        }
      }

    } else {  // UNKNOWN FORMAT

      return 0;

    }

    return new SimpleImage(h.width, h.height, R8G8B8, pixels.release());
  }

}
