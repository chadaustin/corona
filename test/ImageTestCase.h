#ifndef IMAGE_TEST_CASE_H
#define IMAGE_TEST_CASE_H


#include "TestFramework.h"


class ImageTestCase : public CppUnit::TestCase {
public:
  struct Comparison {
    const char* image;
    const char* reference;
  };

  void AssertImagesEqual(
    const string& image_file,
    const string& reference_file)
  {
    auto_ptr<Image> img1(OpenImage(image_file.c_str(), PF_R8G8B8A8));
    CPPUNIT_ASSERT_MESSAGE("opening " + image_file, img1.get() != 0);

    auto_ptr<Image> img2(OpenImage(reference_file.c_str(), PF_R8G8B8A8));
    CPPUNIT_ASSERT_MESSAGE("opening " + reference_file, img2.get() != 0);

    AssertImagesEqual("testing " + image_file, img1.get(), img2.get(), 4);
  }

  void AssertImagesEqual(
    const string& message,
    Image* i1,
    Image* i2,
    int bytes_per_pixel)
  {
    // compare sizes
    CPPUNIT_ASSERT(i1->getWidth()  == i2->getWidth() &&
                   i1->getHeight() == i2->getHeight());
    
    // by this point, we can assume they have the same size
    int width  = i1->getWidth();
    int height = i1->getHeight();
    
    // compare formats
    CPPUNIT_ASSERT(i1->getFormat() == i2->getFormat());
    
    // compare pixel data
    int pixel_comparison = memcmp(i1->getPixels(),
                                  i2->getPixels(),
                                  width * height * bytes_per_pixel);
    CPPUNIT_ASSERT_MESSAGE(message, pixel_comparison == 0);
  }
};


#endif
