#include "corona.h"
#include "DefaultFileSystem.h"
#include "Open.h"


#define COR_EXPORT(ret, name) \
  ret COR_CALL name


namespace corona {

  //////////////////////////////////////////////////////////////////////////////

  COR_EXPORT(const char*, CorGetVersion)() {
    return "0.0.1";
  }

  //////////////////////////////////////////////////////////////////////////////

  COR_EXPORT(Image*, CorOpenImage)(const char* filename,
                                   FileFormat file_format) {
    return CorOpenImageFileSystem(g_DefaultFileSystem, filename, file_format);
  }

  //////////////////////////////////////////////////////////////////////////////

  COR_EXPORT(Image*, CorOpenImageFileSystem)(FileSystem* fs,
                                            const char* filename,
                                            FileFormat file_format) {
    File* file = fs->openFile(filename, FileSystem::READ);
    if (!file) {
      return 0;
    }

    Image* image = CorOpenImageFile(file, file_format);
    file->close();
    return image;
  }

  //////////////////////////////////////////////////////////////////////////////

  COR_EXPORT(Image*, CorOpenImageFile)(File* file, FileFormat file_format) {
    file->seek(0, File::BEGIN);
    switch (file_format) {
      case AUTODETECT: {
        Image* image = CorOpenImageFile(file, PNG);
        if (image) { return image; }
        image = CorOpenImageFile(file, JPEG);
        if (image) { return image; }
        image = CorOpenImageFile(file, PCX);
        if (image) { return image; }
        image = CorOpenImageFile(file, BMP);
        if (image) { return image; }
        return 0;
      }
        
      case PNG:  return OpenPNG(file);
      case JPEG: return OpenJPEG(file);
      case PCX:  return OpenPCX(file);
      case BMP:  return OpenBMP(file);
      default:   return 0;
    }
  }

  //////////////////////////////////////////////////////////////////////////////

}
