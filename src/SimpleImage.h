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
   * The constructor takes a pixel buffer which it then owns and delete[]'s
   * when the image is destroyed.
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
                byte* pixels) {
      m_width  = width;
      m_height = height;
      m_format = format;
      m_pixels = pixels;
    }

    /**
     * Destroys the image, freeing the owned pixel buffer.
     */
    ~SimpleImage() {
      delete[] m_pixels;
    }

    void destroy() {
      delete this;
    }

    /**
     * Width accessor.
     *
     * @return  image width
     */
    int getWidth() {
      return m_width;
    }

    /**
     * Height accessor.
     *
     * @return  image height
     */
    int getHeight() {
      return m_height;
    }

    /**
     * Format accessor.
     *
     * @return  image format
     */
    PixelFormat getFormat() {
      return m_format;
    }

    /**
     * Pixel buffer accessor.
     *
     * @return  mutable pixel buffer
     */
    void* getPixels() {
      return m_pixels;
    }

  private:
    int         m_width;
    int         m_height;
    PixelFormat m_format;
    byte*       m_pixels;
  };

}


#endif
