/**
 * @file
 * @todo  allow conversions from direct color images to
 *        palettized images
 */
#include <map>
#include <utility>
#include "corona.h"
#include "Debug.h"
#include "SimpleImage.h"
#include "Utility.h"


namespace corona {

  Image* ExpandPalette(Image* image) {
    COR_GUARD("ExpandPalette()");

    // assert isPalettized(image->getFormat())

    const int width                  = image->getWidth();
    const int height                 = image->getHeight();
    const byte* in                   = (byte*)image->getPixels();
    const PixelFormat palette_format = image->getPaletteFormat();
    const int pixel_size             = GetPixelSize(palette_format);
    const byte* palette              = (byte*)image->getPalette();

    byte* pixels = new byte[width * height * pixel_size];
    byte* out = pixels;
    for (int i = 0; i < width * height; ++i) {
      memcpy(out, palette + (*in) * pixel_size, pixel_size);
      out += pixel_size;
      ++in;
    }
    delete image;
    return new SimpleImage(width, height, palette_format, pixels);
  }


  struct FormatDesc {
    FormatDesc(int r, int g, int b, int a, bool ha) {
      r_shift = r;
      g_shift = g;
      b_shift = b;
      a_shift = a;
      has_alpha = ha;
    }

    // shifts are in bytes from the right
    // In the case of RGBA, r_shift is 0, g_shift is 1, ...
    int r_shift;
    int g_shift;
    int b_shift;
    int a_shift;
    bool has_alpha;
  };


  #define DEFINE_DESC(format, desc) \
    case format: { COR_LOG(#format); static FormatDesc d desc; return &d; }

  FormatDesc* GetDescription(PixelFormat format) {
    // assert isDirect(image->getFormat())

    switch (format) {
      DEFINE_DESC(PF_R8G8B8A8, (0, 1, 2, 3, true));
      DEFINE_DESC(PF_R8G8B8,   (0, 1, 2, 0, false));
      DEFINE_DESC(PF_B8G8R8A8, (2, 1, 0, 3, true));
      DEFINE_DESC(PF_B8G8R8,   (2, 1, 0, 0, false));
      default: return 0;
    }
  }


  Image* DirectConversion(Image* image, PixelFormat target_format) {
    COR_GUARD("DirectConversion()");

    // assert isDirect(image->getFormat())

    const int width                 = image->getWidth();
    const int height                = image->getHeight();
    const PixelFormat source_format = image->getFormat();
    const byte* in                  = (byte*)image->getPixels();

    if (source_format == target_format) {
        return image;
    }

    const FormatDesc* source_desc = GetDescription(source_format);
    const FormatDesc* target_desc = GetDescription(target_format);
    if (!source_desc || !target_desc) {
      delete image;
      return 0;
    }

    const int source_size = GetPixelSize(source_format);
    const int target_size = GetPixelSize(target_format);

    byte* out_pixels = new byte[width * height * target_size];
    byte* out = out_pixels;

    for (int i = 0; i < width * height; ++i) {
      out[target_desc->r_shift] = in[source_desc->r_shift];
      out[target_desc->g_shift] = in[source_desc->g_shift];
      out[target_desc->b_shift] = in[source_desc->b_shift];

      if (target_desc->has_alpha) {
        if (source_desc->has_alpha) {
          out[target_desc->a_shift] = in[source_desc->a_shift];
        } else {
          out[target_desc->a_shift] = 255;
        }
      }

      in  += source_size;
      out += target_size;
    }

    delete image;
    return new SimpleImage(width, height, target_format, out_pixels);
  }


  namespace hidden {

    COR_EXPORT(Image*, CorConvertImage)(
      Image* image,
      PixelFormat target_format)
    {
      COR_GUARD("CorConvertImage");

      // if we don't have an image, user doesn't care about format, or
      // the formats match, don't do any conversion.
      if (!image ||
          target_format == PF_DONTCARE ||
          target_format == image->getFormat())
      {
        return image;
      }

      COR_LOG("Doing the conversion...");

      // if we have a palettized image, convert it to a direct color
      // image and then convert that
      if (IsPalettized(image->getFormat())) {
        image = ExpandPalette(image);
      }

      return DirectConversion(image, target_format);
    }

  }

}
