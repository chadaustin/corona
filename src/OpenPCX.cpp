#include "Open.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  /*
  struct PCX_HEADER {
    byte manufacturer;
    byte version;
    byte encoding;
    byte bits_per_pixel;
    word xmin;
    word ymin;
    word xmax;
    word ymax;
    word hdpi;
    word vdpi;
    byte colormap[48];
    byte reserved;
    byte num_planes;
    word bytes_per_line;
    word palette_info;
    word h_screen_size;
    word v_screen_size;
    byte filler[54];
  };
  */


  //////////////////////////////////////////////////////////////////////////////

  struct RGB {
    byte red;
    byte green;
    byte blue;
  };


  inline int read16(byte* c) {
    return c[0] + (c[1] << 8);
  }

  //////////////////////////////////////////////////////////////////////////////

  bool ReadScanline(File* file, int scansize, byte* scanline)
  {
    byte* out = scanline;

    while ((out - scanline) < scansize) {

      byte data;
      int read = file->read(&data, 1);
      if (read != 1) {
        return false;
      }

      if ((data & 0xC0) == 0xC0) {

        int numbytes = data & 0x3F;
        read = file->read(&data, 1);
        if (read != 1) {
          return false;
        }

        while (numbytes--) {
          *out++ = data;
        }

      } else {
        *out++ = data;
      }
    }

    return true;
  }

  //////////////////////////////////////////////////////////////////////////////

  Image* OpenPCX(File* file) {

    // read the header block
    byte pcx_header[128];
    int read = file->read(pcx_header, 128);
    if (read != 128) {
      return 0;
    }

    // parse the header...
    int bpp            = pcx_header[3];
    int xmin           = read16(pcx_header + 4);
    int ymin           = read16(pcx_header + 6);
    int xmax           = read16(pcx_header + 8);
    int ymax           = read16(pcx_header + 10);
    int num_planes     = pcx_header[65];
    int bytes_per_line = read16(pcx_header + 66);

    // create the image structure
    int width  = xmax - xmin + 1;
    int height = ymax - ymin + 1;
    auto_array<byte> pixels(new byte[width * height * 3]);

    // decode the pixel data

    if (num_planes == 1) {               // 256 colors

      RGB palette[256];
      auto_array<byte> image(new byte[bytes_per_line * height]);

      // read all of the scanlines
      for (int iy = 0; iy < height; ++iy) {
        if (!ReadScanline(file, bytes_per_line, image + iy * bytes_per_line)) {
          return 0;
        }
      }

      // read palette
      // one byte padding :P
      byte dummy;
      read = file->read(&dummy, 1);
      read += file->read(palette, 3 * 256);
      if (read != 1 + 3 * 256) {
        return 0;
      }

      // convert palettized image to RGB image
      byte* out = pixels;
      for (int iy = 0; iy < height; ++iy) {
        byte* in = image + iy * bytes_per_line;
        for (int ix = 0; ix < width; ++ix) {
          *out++ = palette[*in].red;
          *out++ = palette[*in].green;
          *out++ = palette[*in].blue;
          ++in;
        }
      }

    } else if (num_planes == 3) { // 24-bit color

      auto_array<byte> scanline(new byte[3 * bytes_per_line]);

      byte* out = pixels;
      for (int iy = 0; iy < height; ++iy) {
        ReadScanline(file, 3 * bytes_per_line, scanline);

        byte* r = scanline;
        byte* g = scanline + bytes_per_line;
        byte* b = scanline + bytes_per_line * 2;
        for (int ix = 0; ix < width; ++ix) {
          *out++ = *r++;
          *out++ = *g++;
          *out++ = *b++;
        }
      }

    } else {
      return 0;
    }

    return new SimpleImage(width, height, PF_R8G8B8, pixels.release());
  }

  //////////////////////////////////////////////////////////////////////////////

}
