#ifndef CORONA_SAVE_H
#define CORONA_SAVE_H


#include "corona.h"


namespace corona {
#ifndef NO_JPEG
  bool SaveJPEG(File* file, Image* image); // SaveJPEG.cpp
#endif
#ifndef NO_PNG
  bool SavePNG(File* file, Image* image); // SavePNG.cpp
#endif
  bool SaveTGA(File* file, Image* image); // SaveTGA.cpp
}


#endif
