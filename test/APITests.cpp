#include "APITests.h"


void
APITests::testBasicOperations() {
  // create an image and test the accessors
  int width  = rand() % 32;
  int height = rand() % 32;
  PixelFormat format = PF_R8G8B8A8;
  int bpp = 4;

  auto_ptr<Image> image(CreateImage(width, height, format));
  CPPUNIT_ASSERT(image->getWidth()  == width);
  CPPUNIT_ASSERT(image->getHeight() == height);
  CPPUNIT_ASSERT(image->getFormat() == format);

  // verify that the image is black
  byte* pixels = (byte*)image->getPixels();
  for (int i = 0; i < width * height * bpp; ++i) {
    CPPUNIT_ASSERT(pixels[i] == 0);
  }

  // fill the image with random pixels
  for (int i = 0; i < width * height * bpp; ++i) {
    pixels[i] = rand() % 256;
  }

  // clone the image (use same pixel format)
  auto_ptr<Image> identical_clone(CloneImage(image.get()));
  CPPUNIT_ASSERT(image->getWidth()  == identical_clone->getWidth());
  CPPUNIT_ASSERT(image->getHeight() == identical_clone->getHeight());
  CPPUNIT_ASSERT(image->getFormat() == identical_clone->getFormat());
  CPPUNIT_ASSERT(memcmp(image->getPixels(),
                        identical_clone->getPixels(),
                        width * height * bpp) == 0);

  // clone the image, removing the alpha channel
  auto_ptr<Image> other_clone(CloneImage(identical_clone.get(), PF_R8G8B8));
  CPPUNIT_ASSERT(image->getWidth()  == other_clone->getWidth());
  CPPUNIT_ASSERT(image->getHeight() == other_clone->getHeight());
  CPPUNIT_ASSERT(other_clone->getFormat() == PF_R8G8B8);
  byte* image_p = (byte*)image->getPixels();
  byte* other_p = (byte*)other_clone->getPixels();
  for (int i = 0; i < width * height; ++i) {
    CPPUNIT_ASSERT(*image_p++ == *other_p++);
    CPPUNIT_ASSERT(*image_p++ == *other_p++);
    CPPUNIT_ASSERT(*image_p++ == *other_p++);
    ++image_p;  // skip alpha
  }
}


void
APITests::testAPI() {
  for (int i = 0; i < 10; ++i) {
    testBasicOperations();
  }
}


/// Basically the same as SimpleImage, but it sets a boolean flag when
/// the image is deleted.
class TestDeletionImage : public DLLImplementation<Image> {
public:
  TestDeletionImage(
    int width, int height,
    PixelFormat format, byte* pixels,
    byte* palette, int palette_size, PixelFormat palette_format,
    bool& deleted)
  : m_deleted(deleted)
  {
    m_width          = width;
    m_height         = height;
    m_format         = format;
    m_pixels         = pixels;
    m_palette        = palette;
    m_palette_size   = palette_size;
    m_palette_format = palette_format;
  }

  ~TestDeletionImage() {
    delete[] m_pixels;
    delete[] m_palette;
    m_deleted = true;
  }

  int COR_CALL getWidth() {
    return m_width;
  }

  int COR_CALL getHeight() {
    return m_height;
  }

  PixelFormat COR_CALL getFormat() {
    return m_format;
  }

  void* COR_CALL getPixels() {
    return m_pixels;
  }

  void* COR_CALL getPalette() {
    return m_palette;
  }

  int COR_CALL getPaletteSize() {
    return m_palette_size;
  }

  PixelFormat COR_CALL getPaletteFormat() {
    return m_palette_format;
  }

private:
  int         m_width;
  int         m_height;
  PixelFormat m_format;
  byte*       m_pixels;
  byte*       m_palette;
  int         m_palette_size;
  PixelFormat m_palette_format;
  bool&       m_deleted;
};



void
APITests::testMemory() {
  {
    // ConvertImage() should delete the image if it successfully
    // converts.
    bool convert_image_deleted = false;
    Image* image = new TestDeletionImage(
      16, 16, PF_I8, new byte[256], new byte[256 * 4], 256, PF_R8G8B8,
      convert_image_deleted);
    delete ConvertImage(image, PF_R8G8B8A8);
    CPPUNIT_ASSERT(convert_image_deleted == true);
  }

  {
    // ConvertImage() should also delete the image if it cannot
    // convert.
    bool convert_image_deleted = false;
    Image* image = new TestDeletionImage(
      16, 16, PF_I8, new byte[256], new byte[256 * 4], 256, PF_R8G8B8,
      convert_image_deleted);
    delete ConvertImage(image, PF_I8);
    CPPUNIT_ASSERT(convert_image_deleted == true);
  }

  {
    bool convert_palette_deleted = false;
    Image* image = new TestDeletionImage(
      16, 16, PF_I8, new byte[256], new byte[256 * 4], 256, PF_R8G8B8,
      convert_palette_deleted);
    delete ConvertPalette(image, PF_R8G8B8A8);
    CPPUNIT_ASSERT(convert_palette_deleted == true);
  }

  {
    bool convert_palette_deleted = false;
    Image* image = new TestDeletionImage(
      16, 16, PF_I8, new byte[256], new byte[256 * 4], 256, PF_R8G8B8,
      convert_palette_deleted);
    delete ConvertPalette(image, PF_I8);
    CPPUNIT_ASSERT(convert_palette_deleted == true);
  }
}


Test*
APITests::suite() {
  typedef TestCaller<APITests> Caller;

  TestSuite* suite = new TestSuite();
  suite->addTest(new Caller("Basic API Tests",   &APITests::testAPI));
  suite->addTest(new Caller("Memory Management", &APITests::testMemory));
  return suite;
}
