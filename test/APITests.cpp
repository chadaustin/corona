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


Test*
APITests::suite() {
  typedef TestCaller<APITests> Caller;

  TestSuite* suite = new TestSuite();
  suite->addTest(new Caller("Basic API Tests", &APITests::testAPI));
  return suite;
}
