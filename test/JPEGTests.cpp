#include "JPEGTests.h"


void
JPEGTests::testLoader() {
}


Test*
JPEGTests::suite() {
  typedef TestCaller<JPEGTests> Caller;

  TestSuite* suite = new TestSuite();
  suite->addTest(new Caller("Test JPEG Loader", &JPEGTests::testLoader));
  return suite;
}
