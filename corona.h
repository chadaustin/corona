#ifndef CORONA_H
#define CORONA_H


/* extern C */
#ifndef __cplusplus
#error Corona requires C++
#endif
  

/* calling convention */
#ifdef _WIN32
#  define COR_CALL __stdcall
#else
#  define COR_CALL
#endif


#define COR_FUNCTION(ret, decl) extern "C" ret COR_CALL decl


namespace corona {

  enum FileFormat {
    FF_AUTODETECT = 0x0100,
    FF_PNG        = 0x0101,
    FF_JPEG       = 0x0102,
    FF_PCX        = 0x0103,
    FF_BMP        = 0x0104,
  };

  enum PixelFormat {
    PF_DONTCARE = 0x0200,  // unused?
    PF_R8G8B8A8 = 0x0201,
    PF_R8G8B8   = 0x0202,
  };


  class Image {
  public:
    virtual void destroy() = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual PixelFormat getFormat() = 0;
    virtual void* getPixels() = 0;

    // "delete image" should actually call image->destroy(), thus putting the
    // burden of calling the destructor and freeing the memory on the image
    // object, and thus on the DLL.
    void operator delete(void* p) {
      Image* i = static_cast<Image*>(p);
      i->destroy();
    }
  };

  class File {
  protected:
    ~File() { }  // use close()

  public:
    enum SeekMode {
      BEGIN,
      CURRENT,
      END
    };

    virtual void close() = 0;
    virtual int read(void* buffer, int size) = 0;
    virtual int write(void* buffer, int size) = 0;
    virtual bool seek(int position, SeekMode mode) = 0;
    virtual int tell() = 0;
  };

  class FileSystem {
  protected:
    ~FileSystem() { }  // use destroy()

  public:
    enum OpenMode {
      READ   = 0x0001,
      WRITE  = 0x0002,
      /* BINARY = 0x0004, binary mode is always enabled */
    };

    virtual void destroy() = 0;
    virtual File* openFile(const char* filename, OpenMode mode) = 0;
  };


  // "hidden" methods...  don't call these
  namespace hidden {

    // these are extern "C" so we don't mangle the names...
    // different compilers mangle names differently

    COR_FUNCTION(const char*, CorGetVersion());

    // loading

    COR_FUNCTION(Image*, CorOpenImage(
      const char* filename,
      FileFormat file_format));

    COR_FUNCTION(Image*, CorOpenImageFromFileSystem(
      FileSystem* fs,
      const char* filename,
      FileFormat file_format));

    COR_FUNCTION(Image*, CorOpenImageFromFile(
      File* file,
      FileFormat file_format));

    // saving

    COR_FUNCTION(bool, CorSaveImage(
      const char* filename,
      FileFormat file_format,
      Image* image));

    COR_FUNCTION(bool, CorSaveImageToFileSystem(
      FileSystem* fs,
      const char* filename,
      FileFormat file_format,
      Image* image));

    COR_FUNCTION(bool, CorSaveImageToFile(
      File* file,
      FileFormat file_format,
      Image* image));

    // conversion

    COR_FUNCTION(Image*, CorConvertImage(
      Image* image,
      PixelFormat format));

    COR_FUNCTION(Image*, CorGuaranteeFormat(
      Image* image,
      PixelFormat format));
  }


  /** PUBLIC API **/

  inline const char* GetVersion() {
    return hidden::CorGetVersion();
  }

  inline Image* OpenImage(
    const char* filename,
    FileFormat file_format = FF_AUTODETECT,
    PixelFormat pixel_format = PF_DONTCARE)
  {
    return hidden::CorGuaranteeFormat(
      hidden::CorOpenImage(filename, file_format),
      pixel_format);
  }

  inline Image* OpenImage(
    FileSystem* fs,
    const char* filename,
    FileFormat file_format = FF_AUTODETECT,
    PixelFormat pixel_format = PF_DONTCARE)
  {
    return hidden::CorGuaranteeFormat(
      hidden::CorOpenImageFromFileSystem(fs, filename, file_format),
      pixel_format);
  }

  inline Image* OpenImage(
    File* file,
    FileFormat file_format = FF_AUTODETECT,
    PixelFormat pixel_format = PF_DONTCARE)
  {
    return hidden::CorGuaranteeFormat(
      hidden::CorOpenImageFromFile(file, file_format),
      pixel_format);
  }

  inline bool SaveImage(
    const char* filename,
    FileFormat file_format,
    Image* image)
  {
    return hidden::CorSaveImage(filename, file_format, image);
  }

  inline bool SaveImage(
    FileSystem* fs,
    const char* filename,
    FileFormat file_format,
    Image* image)
  {
    return hidden::CorSaveImageToFileSystem(fs, filename, file_format, image);
  }

  inline bool SaveImage(
    File* file,
    FileFormat file_format,
    Image* image)
  {
    return hidden::CorSaveImageToFile(file, file_format, image);
  }
}


#endif
