#ifndef PALETTE_TESTS_H
#define PALETTE_TESTS_H


#include "TestFramework.h"


class PaletteTests : public TestCase {
public:
  void testAPI();
  void testImages();
  static Test* suite();
};


#endif
