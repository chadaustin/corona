#include <memory>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>
#include "corona.h"
using namespace std;
using namespace CppUnit;
using namespace corona;


typedef unsigned char byte;


class ImageTestCase : public TestCase {
public:
  void AssertImagesEqual(
    string message,
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


class APITests : public TestCase {
public:
  void testBasicOperations() {

    // create an image and test the accessors
    int width = rand()  % 256;
    int height = rand() % 256;
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

  void testAPI() {
    for (int i = 0; i < 10; ++i) {
      testBasicOperations();
    }
  }

  static Test* suite() {
    typedef TestCaller<APITests> Caller;

    TestSuite* suite = new TestSuite();
    suite->addTest(new Caller("Basic API Tests", &APITests::testAPI));
    return suite;
  }
};


class BMPTests : public ImageTestCase {
public:
  void testLoader() {

    struct Comparison {
      const char* image;
      const char* reference;
    };

    static const string base = "images/bmpsuite/";
    static const string reference_base = "images/bmpsuite/reference/";

    static Comparison images[] = {
      { "g01bg.bmp",     "01bg.png"    },
      { "g01bw.bmp",     "01bw.png"    },
      { "g01p1.bmp",     "01p1.png"    },
      { "g01wb.bmp",     "01bw.png"    },
      { "g04.bmp",       "04.png"      },
      { "g04p4.bmp",     "04p4.png"    },
      { "g04rle.bmp",    "04.png"      },
      { "g08.bmp",       "08.png"      },
      { "g08offs.bmp",   "08.png"      },
      { "g08os2.bmp",    "08.png"      },
      { "g08p256.bmp",   "08.png"      },
      { "g08p64.bmp",    "08p64.png"   },
      { "g08pi256.bmp",  "08.png"      },
      { "g08pi64.bmp",   "08.png"      },
      { "g08res11.bmp",  "08.png"      },
      { "g08res21.bmp",  "08.png"      },
      { "g08res22.bmp",  "08.png"      },
      { "g08rle.bmp",    "08.png"      },
      { "g08s0.bmp",     "08.png"      },
      { "g08w124.bmp",   "08w124.png"  },
      { "g08w125.bmp",   "08w125.png"  },
      { "g08w126.bmp",   "08w126.png"  },
      { "g16bf555.bmp",  "16bf555.png" },
      { "g16bf565.bmp",  "16bf565.png" },
      { "g16def555.bmp", "16bf555.png" },
      { "g24.bmp",       "24.png"      },
      { "g32bf.bmp",     "24.png"      },
      { "g32def.bmp",    "24.png"      },
    };
    static const int image_count = sizeof(images) / sizeof(*images);

    for (int i = 0; i < image_count; ++i) {
      string left = base + images[i].image;
      string right = reference_base + images[i].reference;

      // base image
      auto_ptr<Image> img1(OpenImage(left.c_str(), FF_AUTODETECT, PF_R8G8B8));
      CPPUNIT_ASSERT_MESSAGE("opening " + left, img1.get() != 0);

      // reference image
      auto_ptr<Image> img2(OpenImage(right.c_str(), FF_AUTODETECT, PF_R8G8B8));
      CPPUNIT_ASSERT_MESSAGE("opening " + right, img2.get() != 0);

      AssertImagesEqual("comparing " + left, img1.get(), img2.get(), 3);
    }
  }

  static Test* suite() {
    typedef TestCaller<BMPTests> Caller;

    TestSuite* suite = new TestSuite();
    suite->addTest(new Caller("Test BMP Loader", &BMPTests::testLoader));
    return suite;
  }
};


class JPEGTests : public TestCase {
public:
  void testLoader() {
  }

  static Test* suite() {
    typedef TestCaller<JPEGTests> Caller;

    TestSuite* suite = new TestSuite();
    suite->addTest(new Caller("Test JPEG Loader", &JPEGTests::testLoader));
    return suite;
  }
};


class PCXTests : public TestCase {
public:
  void testLoader() {
  }

  static Test* suite() {
    typedef TestCaller<PCXTests> Caller;

    TestSuite* suite = new TestSuite();
    suite->addTest(new Caller("Test PCX Loader", &PCXTests::testLoader));
    return suite;
  }
};


class PNGTests : public ImageTestCase {
public:

  void testLoader() {

    static const string base = "images/pngsuite/";

    static const char* good[] = {
      "basi0g01.png",
      "basi0g02.png",
      "basi0g04.png",
      "basi0g08.png",
      "basi0g16.png",
      "basi2c08.png",
      "basi2c16.png",
      "basi3p01.png",
      "basi3p02.png",
      "basi3p04.png",
      "basi3p08.png",
      "basi4a08.png",
      "basi4a16.png",
      "basi6a08.png",
      "basi6a16.png",
      "basn0g01.png",
      "basn0g02.png",
      "basn0g04.png",
      "basn0g08.png",
      "basn0g16.png",
      "basn2c08.png",
      "basn2c16.png",
      "basn3p01.png",
      "basn3p02.png",
      "basn3p04.png",
      "basn3p08.png",
      "basn4a08.png",
      "basn4a16.png",
      "basn6a08.png",
      "basn6a16.png",
      "bgai4a08.png",
      "bgai4a16.png",
      "bgan6a08.png",
      "bgan6a16.png",
      "bgbn4a08.png",
      "bggn4a16.png",
      "bgwn6a08.png",
      "bgyn6a16.png",
      "ccwn2c08.png",
      "ccwn3p08.png",
      "cdfn2c08.png",
      "cdhn2c08.png",
      "cdsn2c08.png",
      "cdun2c08.png",
      "ch1n3p04.png",
      "ch2n3p08.png",
      "cm0n0g04.png",
      "cm7n0g04.png",
      "cm9n0g04.png",
      "cs3n2c16.png",
      "cs3n3p08.png",
      "cs5n2c08.png",
      "cs5n3p08.png",
      "cs8n2c08.png",
      "cs8n3p08.png",
      "ct0n0g04.png",
      "ct1n0g04.png",
      "ctzn0g04.png",
      "f00n0g08.png",
      "f00n2c08.png",
      "f01n0g08.png",
      "f01n2c08.png",
      "f02n0g08.png",
      "f02n2c08.png",
      "f03n0g08.png",
      "f03n2c08.png",
      "f04n0g08.png",
      "f04n2c08.png",
      "g03n0g16.png",
      "g03n2c08.png",
      "g03n3p04.png",
      "g04n0g16.png",
      "g04n2c08.png",
      "g04n3p04.png",
      "g05n0g16.png",
      "g05n2c08.png",
      "g05n3p04.png",
      "g07n0g16.png",
      "g07n2c08.png",
      "g07n3p04.png",
      "g10n0g16.png",
      "g10n2c08.png",
      "g10n3p04.png",
      "g25n0g16.png",
      "g25n2c08.png",
      "g25n3p04.png",
      "oi1n0g16.png",
      "oi1n2c16.png",
      "oi2n0g16.png",
      "oi2n2c16.png",
      "oi4n0g16.png",
      "oi4n2c16.png",
      "oi9n0g16.png",
      "oi9n2c16.png",
      "pngsuite_logo.png",
      "pp0n2c16.png",
      "pp0n6a08.png",
      "ps1n0g08.png",
      "ps1n2c16.png",
      "ps2n0g08.png",
      "ps2n2c16.png",
      "s01i3p01.png",
      "s01n3p01.png",
      "s02i3p01.png",
      "s02n3p01.png",
      "s03i3p01.png",
      "s03n3p01.png",
      "s04i3p01.png",
      "s04n3p01.png",
      "s05i3p02.png",
      "s05n3p02.png",
      "s06i3p02.png",
      "s06n3p02.png",
      "s07i3p02.png",
      "s07n3p02.png",
      "s08i3p02.png",
      "s08n3p02.png",
      "s09i3p02.png",
      "s09n3p02.png",
      "s32i3p04.png",
      "s32n3p04.png",
      "s33i3p04.png",
      "s33n3p04.png",
      "s34i3p04.png",
      "s34n3p04.png",
      "s35i3p04.png",
      "s35n3p04.png",
      "s36i3p04.png",
      "s36n3p04.png",
      "s37i3p04.png",
      "s37n3p04.png",
      "s38i3p04.png",
      "s38n3p04.png",
      "s39i3p04.png",
      "s39n3p04.png",
      "s40i3p04.png",
      "s40n3p04.png",
      "tbbn1g04.png",
      "tbbn2c16.png",
      "tbbn3p08.png",
      "tbgn2c16.png",
      "tbgn3p08.png",
      "tbrn2c08.png",
      "tbwn1g16.png",
      "tbwn3p08.png",
      "tbyn3p08.png",
      "tp0n1g08.png",
      "tp0n2c08.png",
      "tp0n3p08.png",
      "tp1n3p08.png",
      "z00n2c08.png",
      "z03n2c08.png",
      "z06n2c08.png",
      "z09n2c08.png",
    };
    static const int good_length = sizeof(good) / sizeof(*good);

    static const char* bad[] = {
      "x00n0g01.png",
      "xcrn0g04.png",
      "xlfn0g04.png",
    };
    static const int bad_length = sizeof(bad) / sizeof(*bad);

    
    for (int i = 0; i < good_length; ++i) {
      string fn = base + good[i];
      auto_ptr<Image> image(OpenImage(fn.c_str()));
      CPPUNIT_ASSERT_MESSAGE("should work - opening " + fn, image.get() != 0);
    }

    for (int i = 0; i < bad_length; ++i) {
      string fn = base + bad[i];
      auto_ptr<Image> image(OpenImage(fn.c_str()));
      CPPUNIT_ASSERT_MESSAGE("should fail - opening " + fn, image.get() == 0);
    }
  }

  void testWriter() {

    static const int width  = 256;
    static const int height = 256;

    // create an image and fill it with random data
    auto_ptr<Image> image(CreateImage(width, height, PF_R8G8B8A8));

    byte* pixels = (byte*)image->getPixels();
    for (int i = 0; i < width * height * 4; ++i) {
      *pixels++ = (byte)(rand() % 256);
    }

    // generate filename
    char* filename = tmpnam(0);
    CPPUNIT_ASSERT_MESSAGE("opening temporary file", filename != 0);

    // save image
    CPPUNIT_ASSERT(SaveImage(filename, FF_PNG, image.get()) == true);

    // load it back
    auto_ptr<Image> img2(OpenImage(filename, FF_AUTODETECT, PF_R8G8B8A8));
    CPPUNIT_ASSERT_MESSAGE("reloading image file", img2.get() != 0);

    AssertImagesEqual(
      "comparing saved with loaded",
      image.get(),
      img2.get(),
      4);

    // force pixel format conversion (don't destroy the old image)
    auto_ptr<Image> img3(OpenImage(filename, FF_AUTODETECT, PF_R8G8B8));
    CPPUNIT_ASSERT(SaveImage(filename, FF_PNG, img3.get()) == true);

    // remove the temporary file we made
    remove(filename);
  }

  static Test* suite() {
    typedef TestCaller<PNGTests> Caller;

    TestSuite* suite = new TestSuite();
    suite->addTest(new Caller("Test PNG Loader", &PNGTests::testLoader));
    suite->addTest(new Caller("Test PNG Writer", &PNGTests::testWriter));
    return suite;
  }
};


int main() {
  TextTestRunner runner;
  runner.addTest(APITests::suite());
  runner.addTest(BMPTests::suite());
  runner.addTest(JPEGTests::suite());
  runner.addTest(PCXTests::suite());
  runner.addTest(PNGTests::suite());
  runner.run();
}
