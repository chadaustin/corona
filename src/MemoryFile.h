#ifndef MEMORY_FILE_H
#define MEMORY_FILE_H


#include "corona.h"
#include "Types.h"
#include "Utility.h"


namespace corona {

  class MemoryFile : public DLLImplementation<File> {
  public:
    MemoryFile(void* buffer, int size) {
      m_buffer = new byte[size];
      memcpy(m_buffer, buffer, size);

      m_size = size;
      m_position = 0;
    }

    ~MemoryFile() {
      delete[] m_buffer;
    }

    int read(void* buffer, int size) {
      int real_read = Min((m_size - m_position), size);
      memcpy(buffer, m_buffer + m_position, real_read);
      m_position += real_read;
      return real_read;
    }

    int write(const void* buffer, int size) {
      // readonly
      return 0;
    }

    bool seek(int position, SeekMode mode) {
      int real_pos;
      switch (mode) {
        case BEGIN:   real_pos = position; break;
        case CURRENT: real_pos = m_position + position; break;
        case END:     real_pos = m_size + position; break;
        default:      return false;
      }

      if (real_pos < 0 || real_pos > m_size) {
        m_position = 0;
        return false;
      } else {
        m_position = real_pos;
        return true;
      }
    }

    int tell() {
      return m_position;
    }

  private:
    byte* m_buffer;
    int m_size;
    int m_position;
  };

}


#endif
