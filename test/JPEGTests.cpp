#include "JPEGTests.h"


void
JPEGTests::testLoader() {
  static string images[] = {
    "63",
    "63-1-subsampling",
    "63-floating",
    "63-high",
    "63-low",
    "63-progressive",
    "63-restart",
    "64",
    "comic-progressive",
  };
  static const int image_count = sizeof(images) / sizeof(*images);

  static const string img_prefix("images/jpeg/");
  static const string img_suffix(".jpeg");

  static const string ref_prefix("images/jpeg/ref/");
  static const string ref_suffix(".png");

  for (int i = 0; i < image_count; ++i) {
    AssertImagesEqual(img_prefix + images[i] + img_suffix,
                      ref_prefix + images[i] + ref_suffix);
  }
}


size_t getImageBufferSize(Image* image) {
  return GetPixelSize(image->getFormat()) *
         image->getWidth() * image->getHeight();
}


void
JPEGTests::testSaver() {
  auto_ptr<Image> black(CreateImage(173, 229, PF_R8G8B8));
  auto_ptr<Image> white(CreateImage(149, 512, PF_R8G8B8));

  memset(black->getPixels(), 0x00, getImageBufferSize(black.get()));
  memset(white->getPixels(), 0xFF, getImageBufferSize(white.get()));

  std::string black_filename = tmpnam(0);
  std::string white_filename = tmpnam(0);

  CPPUNIT_ASSERT(SaveImage(black_filename + ".jpeg",
                           FF_AUTODETECT, black.get()));
  CPPUNIT_ASSERT(SaveImage(white_filename, FF_JPEG, white.get()));

  auto_ptr<Image> black_loaded(OpenImage(black_filename + ".jpeg"));
  auto_ptr<Image> white_loaded(OpenImage(white_filename));

  CPPUNIT_ASSERT(black_loaded.get() != 0);
  CPPUNIT_ASSERT(white_loaded.get() != 0);

  remove(black_filename.c_str());
  remove(white_filename.c_str());

  AssertImagesEqual("Saved messed up JPEG", black.get(), black_loaded.get());
  AssertImagesEqual("Saved messed up JPEG", white.get(), white_loaded.get());
}


void
JPEGTests::testIncomplete() {
  auto_ptr<Image> image(OpenImage("images/jpeg/jack-incomplete.jpeg"));
  CPPUNIT_ASSERT(image.get() != 0);
}


Test*
JPEGTests::suite() {
  typedef TestCaller<JPEGTests> Caller;

  TestSuite* suite = new TestSuite();
  suite->addTest(new Caller("JPEG Loader", &JPEGTests::testLoader));
  suite->addTest(new Caller("JPEG Saver",  &JPEGTests::testSaver));
  suite->addTest(new Caller("Incomplete JPEG", &JPEGTests::testIncomplete));
  return suite;
}
