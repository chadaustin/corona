#include <stdio.h>
#include "DefaultFileSystem.h"
#include "Utility.h"


namespace corona {

  class CFile : public File {
  public:
    CFile(FILE* file) {
      m_file = file;
    }

    ~CFile() {
      fclose(m_file);
    }

    void close() {
      delete this;
    }

    int read(void* buffer, int size) {
      return fread(buffer, 1, size, m_file);
    }

    int write(const void* buffer, int size) {
      return fwrite(buffer, 1, size, m_file);
    }

    bool seek(int position, SeekMode mode) {
      int m;
      switch (mode) {
        case BEGIN:   m = SEEK_SET; break;
        case CURRENT: m = SEEK_CUR; break;
        case END:     m = SEEK_END; break;
        default: return false;
      }
      return fseek(m_file, position, m) == 0;
    }

    int tell() {
      return ftell(m_file);
    }

  private:
    FILE* m_file;
  };


  class CFileSystem : public FileSystem {
  public:
    void destroy() {
      // singleton!  no need to clean it up!
    }

    File* openFile(const char* filename, OpenMode mode) {
      char mode_str[4];
      char* c = mode_str;
      if (mode == READ) {
        *c++ = 'r';
      } else if (mode == WRITE) {
        *c++ = 'w';
      }
      *c++ = 'b';

      FILE* file = fopen(filename, mode_str);
      return (file ? new CFile(file) : 0);
    }
  } g_default_fs;


  FileSystem* GetDefaultFileSystem() {
    return &g_default_fs;
  }
}
