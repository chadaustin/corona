#include "Open.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  //////////////////////////////////////////////////////////////////////////////

  Image* OpenTGA(File* file) {

    // read header
    byte header[18];
    if (file->read(header, 18) != 18) {
      return 0;
    }

    // decode header
    int id_length        = header[0];
    int cm_type          = header[1];
    int image_type       = header[2];
    int cm_first         = read16_le(header + 3);
    int cm_length        = read16_le(header + 5);
    int cm_entry_size    = header[7];  // in bits
    int x_origin         = read16_le(header + 8);
    int y_origin         = read16_le(header + 10);
    int width            = read16_le(header + 12);
    int height           = read16_le(header + 14);
    int pixel_depth      = header[16];
    int image_descriptor = header[17];
    
    bool mirrored = (image_descriptor & (1 << 4)) != 0;  // left-to-right?
    bool flipped  = (image_descriptor & (1 << 5)) != 0;  // bottom-to-top?

    /*
     * image types
     * 0  = no image data
     * 1  = uncompressed, color-mapped
     * 2  = uncompressed, true-color
     * 3  = uncompressed, black and white
     * 9  = RLE, color-mapped
     * 10 = RLE, true-color
     * 11 = RLE, black and white
     */

    // make sure we support the image
    if (image_type != 2 || (pixel_depth != 24 && pixel_depth != 32)) {
      return 0;
    }

    // skip image id
    byte unused[255];
    if (file->read(unused, id_length) != id_length) {
      return 0;
    }

    // skip color map
    if (cm_type != 0) {
      // allocate color map
      int cm_entry_bytes = (cm_entry_size + 7) / 8;
      int cm_size = cm_entry_bytes * cm_length;
      auto_array<byte> color_map(new byte[cm_size]);
      if (file->read(color_map, cm_size) != cm_size) {
        return 0;
      }
    }

    // read image data
    PixelFormat format;
    auto_array<byte> pixels;
    if (pixel_depth == 24) {

      format = PF_R8G8B8;
      int image_size = width * height * 3;
      pixels = new byte[image_size];
      if (file->read(pixels, image_size) != image_size) {
        return 0;
      }

      // swap blue and red
      byte* in = pixels;
      for (int i = 0; i < width * height; ++i) {
        byte r = in[0];
        byte g = in[1];
        byte b = in[2];
        *in++ = b;
        *in++ = g;
        *in++ = r;
      }

    } else if (pixel_depth == 32) {

      format = PF_R8G8B8A8;
      int image_size = width * height * 4;
      pixels = new byte[image_size];
      if (file->read(pixels, image_size) != image_size) {
        return 0;
      }

      // convert pixels from ARGB to RGBA
      byte* in = pixels;
      for (int i = 0; i < width * height; ++i) {
        byte a = in[0];
        byte r = in[1];
        byte g = in[2];
        byte b = in[3];
        *in++ = g;
        *in++ = b;
        *in++ = r;
        *in++ = a;
      }

    } else {
      return 0;
    }

    return new SimpleImage(width, height, format, pixels.release());
  }

  //////////////////////////////////////////////////////////////////////////////

}
