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
    AUTODETECT = 0x0100,
    PNG        = 0x0101,
    JPEG       = 0x0102,
    PCX        = 0x0103,
    BMP        = 0x0104,
  };

  enum PixelFormat {
    R8G8B8A8 = 0x0200,
    R8G8B8   = 0x0201,
  };


  class Image {
  public:
    virtual ~Image() { }
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    virtual PixelFormat getFormat() = 0;
    virtual void* getData() = 0;
  };

  class File {
  protected:
    ~File() { }
  public:
    enum SeekMode {
      BEGIN,
      CURRENT,
      END
    };

    virtual void close() = 0;
    virtual int read(void* buffer, int size) = 0;
    virtual bool seek(int position, SeekMode mode) = 0;
    virtual int tell() = 0;
  };

  class FileSystem {
  public:
    enum OpenMode {
      READ   = 0x0001,
      WRITE  = 0x0002,
      /* BINARY = 0x0004, binary mode is always enabled */
    };

    virtual ~FileSystem() { }
    virtual File* openFile(const char* filename, OpenMode mode) = 0;
  };


  // these are extern "C" so we don't mangle the names...
  // different compilers mangle names differently
  COR_FUNCTION(const char*, CorGetVersion());

  COR_FUNCTION(Image*, CorOpenImage(const char* filename,
                                    FileFormat file_format));

  COR_FUNCTION(Image*, CorOpenImageFileSystem(FileSystem* fs,
                                              const char* filename,
                                              FileFormat file_format));

  COR_FUNCTION(Image*, CorOpenImageFile(File* file,
                                        FileFormat file_format));


  // reduce namespace redundancy (i.e. corona::CorFunction)
  inline const char* GetVersion() {
    return CorGetVersion();
  }

  inline Image* OpenImage(const char* filename,
                          FileFormat file_format = AUTODETECT) {
    return CorOpenImage(filename, file_format);
  }

  inline Image* OpenImage(FileSystem* fs,
                          const char* filename,
                          FileFormat file_format = AUTODETECT) {
    return CorOpenImageFileSystem(fs, filename, file_format);
  }

  inline Image* OpenImage(File* file,
                          FileFormat file_format = AUTODETECT) {
    return CorOpenImageFile(file, file_format);
  }
}


#endif
