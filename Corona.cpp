#include "corona.h"
#include "DefaultFileSystem.h"
#include "Open.h"
#include "Save.h"
#include "SimpleImage.h"


#define COR_EXPORT(ret, name) \
  ret COR_CALL name


namespace corona {
  namespace hidden {


  typedef unsigned char byte;

  //////////////////////////////////////////////////////////////////////////////

  COR_EXPORT(const char*, CorGetVersion)() {
    return "0.0.1";
  }

  //////////////////////////////////////////////////////////////////////////////

  COR_EXPORT(Image*, CorOpenImage)(
    const char* filename,
    FileFormat file_format)
  {
    return CorOpenImageFromFileSystem(
      GetDefaultFileSystem(),
      filename,
      file_format);
  }

  //////////////////////////////////////////////////////////////////////////////

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

  //////////////////////////////////////////////////////////////////////////////

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
        return 0;
      }
        
      case FF_PNG:  return OpenPNG(file);
      case FF_JPEG: return OpenJPEG(file);
      case FF_PCX:  return OpenPCX(file);
      case FF_BMP:  return OpenBMP(file);
      default:      return 0;
    }
  }

  //////////////////////////////////////////////////////////////////////////////

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

  //////////////////////////////////////////////////////////////////////////////

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

  //////////////////////////////////////////////////////////////////////////////

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
      default:      return false;
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  COR_EXPORT(Image*, CorConvertImage)(Image* image, PixelFormat format) {
    PixelFormat source = image->getFormat();
    if (source == format) {
      return image;
    }

    int width = image->getWidth();
    int height = image->getHeight();
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

  //////////////////////////////////////////////////////////////////////////////

  COR_EXPORT(Image*, CorGuaranteeFormat)(Image* image, PixelFormat format) {
    if (format == PF_DONTCARE) {
      return image;
    }

    if (!image) {
      return 0;
    } else {
      return CorConvertImage(image, format);
    }
  }

  //////////////////////////////////////////////////////////////////////////////

  } // end hidden namespace
} // end corona namespace
