#include <png.h>
#include "Open.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  typedef unsigned char byte;


  //////////////////////////////////////////////////////////////////////////////

  void PNG_read_function(png_structp png_ptr,
                         png_bytep data,
                         png_size_t length) {
    File* file = (File*)png_get_io_ptr(png_ptr);
    int read = file->read(data, length);
    memset(data + read, 0, length - read);  // fill the rest with zeroes
  }

  //////////////////////////////////////////////////////////////////////////////

  Image* OpenPNG(File* file) {

    // verify PNG signature
    byte sig[8];
    file->read(sig, 8);
    if (png_sig_cmp(sig, 0, 8)) {
      return 0;
    }

    // read struct
    png_structp png_ptr = png_create_read_struct(
      PNG_LIBPNG_VER_STRING,
      NULL, NULL, NULL);
    if (!png_ptr) {
      return 0;
    }

    // info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return 0;
    }

    // read the image
    png_set_read_fn(png_ptr, file, PNG_read_function);
    png_set_sig_bytes(png_ptr, 8);  // we already read 8 bytes for the sig
    int png_transform = PNG_TRANSFORM_STRIP_16;
    png_transform    |= PNG_TRANSFORM_PACKING;
    png_transform    |= PNG_TRANSFORM_PACKSWAP;
    png_read_png(png_ptr, info_ptr, png_transform, NULL);

    if (!png_get_rows(png_ptr, info_ptr)) {
      png_destroy_info_struct(png_ptr, &info_ptr);
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return 0;
    }

    int width  = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    byte* pixels = 0;  // allocate when we know the format
    PixelFormat format;

    // decode based on pixel format
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int num_channels = png_get_channels(png_ptr, info_ptr);
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    // 32-bit RGBA
    if (bit_depth == 8 && num_channels == 4) {

      format = R8G8B8A8;
      pixels = new byte[width * height * 4];
      for (int i = 0; i < height; ++i) {
        memcpy(pixels + i * width * 4, row_pointers[i], width * 4);
      }

    // 24-bit RGB
    } else if (bit_depth == 8 && num_channels == 3) {

      format = R8G8B8;
      pixels = new byte[width * height * 3];
      for (int i = 0; i < height; ++i) {
        memcpy(pixels + i * width * 3, row_pointers[i], width * 3);
      }

    // palettized or greyscale with alpha
    } else if (bit_depth == 8 && num_channels == 2) {

      format = R8G8B8A8;
      pixels = new byte[width * height * 4];
      byte* out = pixels;

      // try to get the palette
      png_colorp palette;
      int num_palette;
      png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

      if (num_palette < 256) {    // greyscale
        for (int i = 0; i < height; ++i) {
          byte* in = row_pointers[i];
          for (int j = 0; j < width; ++j) {
            *out++ = *in;   // red
            *out++ = *in;   // green
            *out++ = *in++; // blue
            *out++ = *in++; // alpha
          }
        }
      } else {                   // palettized
        for (int i = 0; i < height; ++i) {
          byte* in = row_pointers[i];
          for (int j = 0; j < width; ++j) {
            *out++ = palette[*in].red;
            *out++ = palette[*in].green;
            *out++ = palette[*in].blue;
            ++in;
            *out++ = *in++;    // alpha
          }
        }
      }

    // palettized or greyscale, no alpha
    } else if (bit_depth == 8 && num_channels == 1) {

      format = R8G8B8A8;
      pixels = new byte[width * height * 4];
      byte* out = pixels;

      // try to get the palette
      png_colorp palette;
      int num_palette = 0;
      png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

      // get the transparent palette flags
      png_bytep trans;
      int num_trans = 0;
      png_color_16p trans_values; // these aren't used
      png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);

      // create a transparency table
      int alphas_size = Max(num_palette, 256);
      auto_array<byte> alphas(new byte[alphas_size]);
      for (int i = 0; i < alphas_size; ++i) {
        alphas[i] = 255;
      }
      for (int i = 0; i < Min(alphas_size, num_trans); ++i) {
        alphas[i] = 0;
      }

      if (num_palette < 256) {     // greyscale
        for (int i = 0; i < height; ++i) {
          byte* row = row_pointers[i];
          for (int j = 0; j < width; j++) {
            *out++ = *row; // red
            *out++ = *row; // green
            *out++ = *row; // blue
            *out++ = alphas[int(*row)];
            ++row;
          }
        }
      } else {                    // palettized
        for (int i = 0; i < height; i++) {
          byte* row = row_pointers[i];
          for (int j = 0; j < width; j++) {
            *out++ = palette[*row].red;
            *out++ = palette[*row].green;
            *out++ = palette[*row].blue;
            *out++ = alphas[int(*row)];
            ++row;
          }
        }
      }
    
    // 4-bit palettized
    } else if (bit_depth == 4 && num_channels == 1) {

      format = R8G8B8;
      pixels = new byte[width * height * 3];
      byte* out = pixels;

      // try to read the palette
      png_colorp palette;
      int num_palette = 0;
      png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

      if (num_palette < 16) {  // greyscale

        for (int i = 0; i < height; i++) {
          byte* row = row_pointers[i];
          for (int j = 0; j < width / 2; j++) {
          
            *out++ = (*row & 0xF0);
            *out++ = (*row & 0xF0);
            *out++ = (*row & 0xF0);

            *out++ = (*row & 0x0F) << 4;
            *out++ = (*row & 0x0F) << 4;
            *out++ = (*row & 0x0F) << 4;

            ++row;
          }

          if (width % 2) {
            byte p = *row >> 4;
            *out++ = (*row & 0xF0);
            *out++ = (*row & 0xF0);
            *out++ = (*row & 0xF0);
          }
        }

      } else {
        for (int i = 0; i < height; i++) {
          byte* row = row_pointers[i];
          for (int j = 0; j < width / 2; j++) {
          
            byte p1 = *row >> 4;
            *out++ = palette[p1].red;
            *out++ = palette[p1].green;
            *out++ = palette[p1].blue;

            byte p2 = *row & 0xF;
            *out++ = palette[p2].red;
            *out++ = palette[p2].green;
            *out++ = palette[p2].blue;

            ++row;
          }

          if (width % 2) {
            byte p = *row >> 4;
            *out++ = palette[p].red;
            *out++ = palette[p].green;
            *out++ = palette[p].blue;
          }
        }
      }

    // 1-bit palettized
    } else if (bit_depth == 1 && num_channels == 1) {

      format = R8G8B8;
      pixels = new byte[width * height * 3];
      byte* out = pixels;

      // try to read the palette
      png_colorp palette;
      int num_palette;
      png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);

      if (num_palette < 2) {  // black and white
        for (int i = 0; i < height; i++) {

          int mask = 1;
          byte* row = row_pointers[i];

          for (int j = 0; j < width; j++) {

            *out++ = ((*row & mask) > 0) * 255;
            *out++ = ((*row & mask) > 0) * 255;
            *out++ = ((*row & mask) > 0) * 255;

            mask <<= 1;
            if (mask == 256) {
              row++;
              mask = 1;
            }
          }
        }
      } else {                // palettized two-color
        for (int i = 0; i < height; i++) {

          int mask = 1;
          byte* row = row_pointers[i];

          for (int j = 0; j < width; j++) {

            *out++ = palette[(*row & mask) > 0].red;
            *out++ = palette[(*row & mask) > 0].green;
            *out++ = palette[(*row & mask) > 0].blue;

            mask <<= 1;
            if (mask == 256) {
              row++;
              mask = 1;
            }
          }
        }
      }

    } else {  // unknown format
      png_destroy_info_struct(png_ptr, &info_ptr);
      png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
      return 0;
    }

    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return new SimpleImage(width, height, format, pixels);
  }

  //////////////////////////////////////////////////////////////////////////////

}
