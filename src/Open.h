#ifndef CORONA_OPEN_H
#define CORONA_OPEN_H


#include "corona.h"


namespace corona {
  Image* OpenBMP (File* file); // OpenBMP.cpp
  Image* OpenJPEG(File* file); // OpenJPEG.cpp
  Image* OpenPCX (File* file); // OpenPCX.cpp
  Image* OpenPNG (File* file); // OpenPNG.cpp
}


#endif
