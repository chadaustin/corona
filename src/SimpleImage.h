#ifndef CORONA_SIMPLE_IMAGE_H
#define CORONA_SIMPLE_IMAGE_H


#include "corona.h"
#include "Types.h"
#include "Utility.h"


namespace corona {

  /**
   * Basic, flat, simple image.  Has a width, a height, a pixel
   * format, and a 2D array of pixels (one-byte packing).
   *
   * The constructor takes a pixel buffer (and optionally a palette)
   * which it then owns and delete[]'s when the image is destroyed.
   */

  class SimpleImage : public DefaultDelete<Image> {
  public:

    /**
     * Creates a new image, setting all properties.
     *
     * @param width   width of the new image
     * @param height  height of the new image
     * @param format  format that the pixels are stored in
     * @param pixels  pixel buffer that the SimpleImage takes ownership of.
     *                it should be width*height*sizeof(pixel) bytes.
     */
    SimpleImage(int width,
                int height,
                PixelFormat format,
                byte* pixels,
                byte* palette = 0,
                int palette_size = 0,
                PixelFormat palette_format = PF_DONTCARE) {

      m_width          = width;
      m_height         = height;
      m_format         = format;
      m_pixels         = pixels;
      m_palette        = palette;
      m_palette_size   = palette_size;
      m_palette_format = palette_format;
    }

    /**
     * Destroys the image, freeing the owned pixel buffer and palette.
     */
    ~SimpleImage() {
      delete[] m_pixels;
      delete[] m_palette;
    }

    void destroy() {
      delete this;
    }

    int getWidth() {
      return m_width;
    }

    int getHeight() {
      return m_height;
    }

    PixelFormat getFormat() {
      return m_format;
    }

    void* getPixels() {
      return m_pixels;
    }

    void* getPalette() {
      return m_palette;
    }

    int getPaletteSize() {
      return m_palette_size;
    }

    PixelFormat getPaletteFormat() {
      return m_palette_format;
    }

  private:
    int         m_width;
    int         m_height;
    PixelFormat m_format;
    byte*       m_pixels;
    byte*       m_palette;
    int         m_palette_size;
    PixelFormat m_palette_format;
  };

}


#endif
