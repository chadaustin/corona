#include "TestFramework.h"
#include "APITests.h"
#include "BMPTests.h"
#include "JPEGTests.h"
#include "PCXTests.h"
#include "PNGTests.h"
#include "TGATests.h"


int main() {
  TextTestRunner runner;
  runner.addTest(APITests::suite());
  runner.addTest(BMPTests::suite());
  runner.addTest(JPEGTests::suite());
  runner.addTest(PCXTests::suite());
  runner.addTest(PNGTests::suite());
  runner.addTest(TGATests::suite());
  runner.run();
}
