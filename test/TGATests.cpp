#include "TGATests.h"


void
TGATests::testLoader() {
  AssertImagesEqual("images/targa/test.tga",
                    "images/targa/reference/test.png");
}

Test*
TGATests::suite() {
  typedef TestCaller<TGATests> Caller;

  TestSuite* suite = new TestSuite();
  suite->addTest(new Caller("Test TGA Loader", &TGATests::testLoader));
  return suite;
}
