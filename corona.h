/*
  Corona Image I/O Library
  (c) 2002 Chad Austin

  This API uses principles explained at
  http://aegisknight.org/cppinterface.html
*/


#ifndef CORONA_H
#define CORONA_H


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
    PF_DONTCARE = 0x0200,
    PF_R8G8B8A8 = 0x0201,
    PF_R8G8B8   = 0x0202,
  };


  class Image {
  public:

    // destroy the image object, freeing all associated memory
    virtual void destroy() = 0;

    // returns image dimensions
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    // pixel format
    virtual PixelFormat getFormat() = 0;

    // actual pixel data
    virtual void* getPixels() = 0;

    // "delete image" should actually call image->destroy(), thus putting the
    // burden of calling the destructor and freeing the memory on the image
    // object, and thus on the DLL.
    void operator delete(void* p) {
      if (p) {
        Image* i = static_cast<Image*>(p);
        i->destroy();
      }
    }
  };


  // represents a random-access file
  // always binary (no end-of-line translations)
  // XXXaegis semantics roughly analagous to C stdio files
  class File {
  protected:
    ~File() { }  // use close()

  public:
    enum SeekMode {
      BEGIN,
      CURRENT,
      END
    };

    // close the file and delete the File object
    virtual void close() = 0;

    // return the number of bytes read
    virtual int read(void* buffer, int size) = 0;

    // return the number of bytes written
    virtual int write(void* buffer, int size) = 0;


    // seek to a position in the file, return true if successful
    virtual bool seek(int position, SeekMode mode) = 0;

    // return current position in file
    virtual int tell() = 0;
  };


  class FileSystem {
  protected:
    ~FileSystem() { }  // use destroy()

  public:
    enum OpenMode {
      READ   = 0x0001,
      WRITE  = 0x0002,
      /* all files are binary */
    };

    // destroy the filesystem object
    virtual void destroy() = 0;

    // open a new file from the filesystem
    // return 0 on failure
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

  // return current API version string
  inline const char* GetVersion() {
    return hidden::CorGetVersion();
  }

  // open image from default filesystem
  // return 0 on failure
  inline Image* OpenImage(
    const char* filename,
    FileFormat file_format = FF_AUTODETECT,
    PixelFormat pixel_format = PF_DONTCARE)
  {
    return hidden::CorGuaranteeFormat(
      hidden::CorOpenImage(filename, file_format),
      pixel_format);
  }

  // open image from specified filesystem
  // return 0 on failure
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

  // open image from file
  // return 0 on failure
  inline Image* OpenImage(
    File* file,
    FileFormat file_format = FF_AUTODETECT,
    PixelFormat pixel_format = PF_DONTCARE)
  {
    return hidden::CorGuaranteeFormat(
      hidden::CorOpenImageFromFile(file, file_format),
      pixel_format);
  }

  // save image to default filesystem
  // return false on failure
  inline bool SaveImage(
    const char* filename,
    FileFormat file_format,
    Image* image)
  {
    return hidden::CorSaveImage(filename, file_format, image);
  }

  // save image to specified filesystem
  // return false on failure
  inline bool SaveImage(
    FileSystem* fs,
    const char* filename,
    FileFormat file_format,
    Image* image)
  {
    return hidden::CorSaveImageToFileSystem(fs, filename, file_format, image);
  }

  // save image to file
  // return false on failure
  inline bool SaveImage(
    File* file,
    FileFormat file_format,
    Image* image)
  {
    return hidden::CorSaveImageToFile(file, file_format, image);
  }

  // convert an image from its pixel format to the specified one.
  // if the formats are the same, returns 'source' directly
  // otherwise, destroys the old image and returns a new one of the
  // correct format
  inline Image* ConvertImage(Image* source, PixelFormat format) {
    return hidden::CorConvertImage(source, format);
  }

}


#endif
