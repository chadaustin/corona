#include <string.h>
#include "corona.h"
#include "DefaultFileSystem.h"
#include "Open.h"
#include "Save.h"
#include "SimpleImage.h"
#include "Utility.h"


#define COR_EXPORT(ret, name)  ret COR_CALL name


namespace corona {
  namespace hidden {

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(const char*, CorGetVersion)() {
      return "1.0.0";
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*, CorCreateImage)(
      int width,
      int height,
      PixelFormat format)
    {
      int pixel_size = GetPixelSize(format);
      if (pixel_size == 0) {
        // unsupported pixel format???
        return 0;
      }

      int size = width * height * pixel_size;
      byte* pixels = new byte[size];
      memset(pixels, 0, size);
      return new SimpleImage(width, height, format, pixels);
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*, CorCloneImage)(
      Image* source,
      PixelFormat format)
    {
      if (!source) {
        // we need an image to clone :)
        return 0;
      }

      int width = source->getWidth();
      int height = source->getHeight();
      PixelFormat source_format = source->getFormat();

      int pixel_size = GetPixelSize(source_format);
      if (pixel_size == 0) {
        // unknown pixel size?
        return 0;
      }

      // duplicate the image
      int image_size = width * height * pixel_size;
      byte* pixels = new byte[image_size];
      memcpy(pixels, source->getPixels(), image_size);
      Image* image = new SimpleImage(width, height, source_format, pixels);
      
      return ConvertImage(image, format);
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*, CorOpenImage)(
      const char* filename,
      FileFormat file_format)
    {
      return CorOpenImageFromFileSystem(
        GetDefaultFileSystem(),
        filename,
        file_format);
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*, CorOpenImageFromFileSystem)(
      FileSystem* fs,
      const char* filename,
      FileFormat file_format)
    {
      File* file = fs->openFile(filename, FileSystem::READ);
      if (!file) {
        return 0;
      }

      Image* image = CorOpenImageFromFile(file, file_format);
      file->close();
      return image;
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*, CorOpenImageFromFile)(
      File* file,
      FileFormat file_format)
    {
      file->seek(0, File::BEGIN);
      switch (file_format) {
        case FF_AUTODETECT: {
          Image* image = CorOpenImageFromFile(file, FF_PNG);
          if (image) { return image; }
          image = CorOpenImageFromFile(file, FF_JPEG);
          if (image) { return image; }
          image = CorOpenImageFromFile(file, FF_PCX);
          if (image) { return image; }
          image = CorOpenImageFromFile(file, FF_BMP);
          if (image) { return image; }
          image = CorOpenImageFromFile(file, FF_TGA);
          if (image) { return image; }
          return 0;
        }
        
        case FF_PNG:  return OpenPNG(file);
        case FF_JPEG: return OpenJPEG(file);
        case FF_PCX:  return OpenPCX(file);
        case FF_BMP:  return OpenBMP(file);
        case FF_TGA:  return OpenTGA(file);
        default:      return 0;
      }
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(bool, CorSaveImage)(
      const char* filename,
      FileFormat file_format,
      Image* image)
    {
      return CorSaveImageToFileSystem(
        GetDefaultFileSystem(),
        filename,
        file_format,
        image);
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(bool, CorSaveImageToFileSystem)(
      FileSystem* fs,
      const char* filename,
      FileFormat file_format,
      Image* image)
    {
      File* file = fs->openFile(filename, FileSystem::WRITE);
      if (!file) {
        return 0;
      }

      bool success = CorSaveImageToFile(file, file_format, image);
      file->close();
      return success;
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(bool, CorSaveImageToFile)(
      File* file,
      FileFormat file_format,
      Image* image)
    {
      switch (file_format) {
        case FF_PNG:  return SavePNG(file, image);
        case FF_JPEG: return false;
        case FF_PCX:  return false;
        case FF_BMP:  return false;
        case FF_TGA:  return false;
        default:      return false;
      }
    }

    ///////////////////////////////////////////////////////////////////////////

    COR_EXPORT(Image*, CorConvertImage)(Image* image, PixelFormat format) {

      if (!image ||                       // no image to convert?
          format == PF_DONTCARE ||        // user doesn't care about format?
          format == image->getFormat()) { // formats match?

        // don't convert anything! :)
        return image;
      }

      // store source attributes
      int width = image->getWidth();
      int height = image->getHeight();
      PixelFormat source = image->getFormat();

      // do the conversion
      byte* in = (byte*)image->getPixels();
      byte* pixels;
      if (source == PF_R8G8B8A8 && format == PF_R8G8B8) {
        pixels = new byte[width * height * 3];
        byte* out = pixels;
        for (int i = 0; i < width * height; ++i) {
          *out++ = *in++;
          *out++ = *in++;
          *out++ = *in++;
          ++in;           // ignore alpha
        }
      } else if (source == PF_R8G8B8 && format == PF_R8G8B8A8) {
        pixels = new byte[width * height * 4];
        byte* out = pixels;
        for (int i = 0; i < width * height; ++i) {
          *out++ = *in++;
          *out++ = *in++;
          *out++ = *in++;
          *out++ = 255;   // assume opaque
        }
      } else {
        // unknown conversion...
        delete image;  // should we really delete the source?
        return 0;
      }

      // delete the source
      delete image;
      return new SimpleImage(width, height, format, pixels);
    }

    ///////////////////////////////////////////////////////////////////////////

  }
}
