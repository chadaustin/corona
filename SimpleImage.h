#ifndef CORONA_SIMPLE_IMAGE_H
#define CORONA_SIMPLE_IMAGE_H


#include "corona.h"
#include "Types.h"
#include "Utility.h"


namespace corona {

  class SimpleImage : public DefaultDelete<Image> {
  public:
    SimpleImage(int width,
                int height,
                PixelFormat format,
                byte* pixels) {
      m_width  = width;
      m_height = height;
      m_format = format;
      m_pixels = pixels;
    }

    ~SimpleImage() {
      delete[] m_pixels;
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

  private:
    int         m_width;
    int         m_height;
    PixelFormat m_format;
    byte*       m_pixels;
  };

}


#endif
