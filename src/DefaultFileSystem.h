#ifndef CORONA_DEFAULT_FILE_SYSTEM_H
#define CORONA_DEFAULT_FILE_SYSTEM_H


#include "corona.h"


namespace corona {
  /**
   * Returns a default File implementation.
   *
   * @param  filename   name of the file
   * @param  writeable  whether the file can be written to
   */
  File* OpenDefaultFile(const char* filename, bool writeable);
}


#endif
