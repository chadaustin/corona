#ifndef CORONA_OPEN_H
#define CORONA_OPEN_H


#include "corona.h"


namespace corona {
  inline Image* OpenBMP (File* file) { return 0; }
  Image* OpenJPEG(File* file);
  Image* OpenPCX (File* file);
  inline Image* OpenPNG (File* file) { return 0; }
}


#endif
