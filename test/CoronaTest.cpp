#include <memory>
#include <string>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TextTestRunner.h>
#include "corona.h"
using namespace std;
using namespace CppUnit;
using namespace corona;


class APITests : public TestCase {
public:
  static Test* suite() {
    TestSuite* suite = new TestSuite();
    // add tests
    return suite;
  }
};


class BMPTests : public TestCase {
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

      // compare sizes
      CPPUNIT_ASSERT(img1->getWidth() == img2->getWidth() &&
                     img1->getHeight() == img2->getHeight());

      // by this point, we can assume they have the same size
      int width  = img1->getWidth();
      int height = img1->getHeight();

      // compare formats
      CPPUNIT_ASSERT(img1->getFormat() == img2->getFormat());

      // compare pixel data
      int comparison = memcmp(img1->getPixels(),
                              img2->getPixels(),
                              width * height * 3);
      CPPUNIT_ASSERT_MESSAGE("pixels wrong in " + left, comparison == 0);
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


class PNGTests : public TestCase {
public:
  void testLoader() {
  }

  void testWriter() {
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
