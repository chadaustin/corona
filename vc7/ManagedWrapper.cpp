#using <mscorlib.dll>

using namespace System;
using namespace System::Text;

[assembly: System::Reflection::AssemblyVersion("0.2.0.1")];
[assembly: System::Reflection::AssemblyKeyFileAttribute("Corona.snk")];


namespace C {
  #include "../src/corona.h"
  #include "../src/Utility.h"
  #include <string.h>
}

namespace Corona {
  public __value enum FileFormat {
    Auto  = 0x0100,
    PNG   = 0x0101,
    JPEG  = 0x0102,
    PCX   = 0x0103,
    BMP   = 0x0104,
    TGA   = 0x0105,
    GIF   = 0x0106,
  };

  public __value enum PixelFormat
  {
    DontCare  = 0x0200,
    R8G8B8A8  = 0x0201,
    R8G8B8    = 0x0202,
    PF_I8     = 0x0203,
  };

  public __gc class Image : public System::IDisposable {
  private:
    bool m_Disposed;
    C::corona::Image __nogc* m_CorImage;

  private:
    //Constructs an Image from a corimage
    Image(C::corona::Image __nogc* corimage) {
      Init(corimage);
    }
    
    //internal initialization from a corimage
    void Init(C::corona::Image __nogc* corimage) {
      m_Disposed = false;
      m_CorImage = corimage;
    }

  public:
    ~Image() {
      Dispose();
    }

    void Dispose() {
      if (!m_Disposed) {
        m_Disposed = true;
        GC::SuppressFinalize(this);
        delete m_CorImage;
        m_CorImage = 0;
      }
    }

    // copy constructor
    Image(Image __gc* img) {
      if (!img) {
        throw new ArgumentNullException(
          "Null Corona.Image passed to a Corona.Image copy constructor");
      }

      if (img->m_Disposed) {
        throw new ObjectDisposedException(
          "Disposed Corona.Image passed to Corona.Image copy constructor");
      }

      Init(C::corona::CloneImage(img->m_CorImage, img->m_CorImage->getFormat()));
    }

    // copy constructor forcing to a specified PixelFormat
    Image(Image __gc* img, Corona::PixelFormat pixel_format) {
      if (!img) {
        throw new ArgumentNullException(
          "Null Corona.Image passed to a Corona.Image copy constructor");
      }

      if (img->m_Disposed) {
        throw new ObjectDisposedException(
          "Disposed Corona.Image passed to Corona.Image copy constructor");
      }

      Init(C::corona::CloneImage(
        img->m_CorImage,
        static_cast<C::corona::PixelFormat>(pixel_format)));
    }

  public:
    __property int get_Width() { return m_CorImage->getWidth(); }
    __property int get_Height() { return m_CorImage->getHeight(); }
    __property Corona::PixelFormat get_PixelFormat() { return static_cast<Corona::PixelFormat>(m_CorImage->getFormat()); }
    __property int get_PaletteSize() { return m_CorImage->getPaletteSize(); }
    __property Corona::PixelFormat get_PaletteFormat() { return static_cast<Corona::PixelFormat>(m_CorImage->getPaletteFormat()); }
    __property void __nogc* get_Pixels() { return m_CorImage->getPixels(); }
    __property void __nogc* get_Palette() { return m_CorImage->getPixels(); }
    __property int get_PixelSize() { return GetFormatSize(PixelFormat); }
    __property int get_PaletteElementSize() { return GetFormatSize(PaletteFormat); }

  public:
    static int GetFormatSize(Corona::PixelFormat pixel_format) {
      return C::corona::GetBytesPerPixel(
        static_cast<C::corona::PixelFormat>(pixel_format));
    }
  
    static Image __gc* Create(int width, int height, Corona::PixelFormat pixel_format) {
      C::corona::Image __nogc* newimage = C::corona::CreateImage(
        width,
        height,
        static_cast<C::corona::PixelFormat>(pixel_format));
      return (newimage ? __gc new Image(newimage) : 0);
    }

    static Image __gc* Open(
      String __gc* filename,
      FileFormat file_format,
      Corona::PixelFormat pixel_format)
    {
      char strbuf __gc[] = Encoding::ASCII->GetBytes(filename);
      char __pin* pstr = &strbuf[0];
      char* str = static_cast<char*>(pstr);

      C::corona::Image __nogc* newimage = C::corona::OpenImage(
        str,
        static_cast<C::corona::PixelFormat>(pixel_format),
        static_cast<C::corona::FileFormat>(file_format));
      return (newimage ? __gc new Image(newimage) : 0);
    }

  public:
    bool Save(String __gc* filename, Corona::FileFormat file_format) {
      char strbuf __gc[] = Encoding::ASCII->GetBytes(filename);
      char __pin* pstr = &strbuf[0];
      char* str = static_cast<char*>(pstr);

      return C::corona::SaveImage(
        str,
        static_cast<C::corona::FileFormat>(file_format),
        m_CorImage);
    }

    Image __gc* Convert(Corona::PixelFormat pixel_format) {
      C::corona::Image __nogc* newimage = C::corona::ConvertImage(
        m_CorImage,
        static_cast<C::corona::PixelFormat>(pixel_format));
      return (newimage ? __gc new Image(newimage) : 0);
    }

    Image __gc* Clone(Corona::PixelFormat pixel_format) {
      return new Image(this, pixel_format);
    }

    Image __gc* Clone() {
      return new Image(this);
    }

    Byte GrabPixels2d() __gc[,] {
      Byte arr __gc[,] = static_cast<Byte __gc[,]>(
        Array::CreateInstance(__typeof(Byte), Width * PixelSize, Height));
      Byte __pin* arrptr = &arr[0,0];
      C::memcpy(arrptr, Pixels, Width * Height * PixelSize);
      return arr;
    }

    Byte GrabPixels() __gc[] {
      Byte arr __gc[] = __gc new Byte[Width * Height * PixelSize];
      Byte __pin* arrptr = &arr[0];
      C::memcpy(arrptr, Pixels, Width * Height * PixelSize);
      return arr;
    }

    Byte GrabPalette() __gc[] {
      Byte arr __gc[] = __gc new Byte[PaletteSize * PaletteElementSize];
      Byte __pin* arrptr = &arr[0];
      C::memcpy(arrptr, Palette, PaletteSize * PaletteElementSize);
      return arr;
    }    
  };
}
