#include <memory>
#include <png.h>
#include "Save.h"


namespace corona {

  typedef unsigned char byte;

  void PNG_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    File* file = (File*)png_get_io_ptr(png_ptr);
    if (file->write(data, length) != length) {
      png_error(png_ptr, "Write error");
    }
  }

  void PNG_flush(png_structp png_ptr) {
    // assume that files always flush
  }

  bool SavePNG(File* file, Image* source) {
    std::auto_ptr<Image> image(CloneImage(source, PF_R8G8B8A8));
    if (!image.get()) {
      return false;
    }

    // create write struct
    png_structp png_ptr = png_create_write_struct(
      PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
      return false;
    }

    // error handling!
    if (setjmp(png_jmpbuf(png_ptr))) {
      png_destroy_write_struct(&png_ptr, NULL);
      return false;
    }

    // create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
      png_destroy_write_struct(&png_ptr, NULL);
      return false;
    }

    int width  = image->getWidth();
    int height = image->getHeight();

    // set image characteristics
    png_set_write_fn(png_ptr, file, PNG_write, PNG_flush);
    png_set_IHDR(
      png_ptr, info_ptr,
      width, height,
      8,
      PNG_COLOR_TYPE_RGB_ALPHA,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT,
      PNG_FILTER_TYPE_DEFAULT);


    byte* pixels = (byte*)image->getPixels();

    // build rows
    void** rows = (void**)png_malloc(png_ptr, sizeof(void*) * height);
    for (int i = 0; i < height; ++i) {
      rows[i] = png_malloc(png_ptr, 4 * width);
      memcpy(rows[i], pixels, 4 * width);
      pixels += width * 4;      
    }
    png_set_rows(png_ptr, info_ptr, (png_bytepp)rows);
    info_ptr->valid |= PNG_INFO_IDAT;

    // actually write the image
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    return true;
  }

}
