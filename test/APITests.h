#ifndef API_TESTS_H
#define API_TESTS_H


#include "TestFramework.h"


class APITests : public TestCase {
public:
  void testBasicOperations();
  void testAPI();
  void testMemory();
  static Test* suite();
};


#endif
