#ifndef MEMORY_FILE_H
#define MEMORY_FILE_H


#include "corona.h"
#include "Types.h"
#include "Utility.h"


namespace corona {

  inline int getNextPowerOfTwo(int value) {
    int i = 1;
    while (i < value) {
      i *= 2;
    }
    return i;
  }

  class MemoryFile : public DLLImplementation<File> {
  public:
    MemoryFile(const void* buffer, int size) {
      m_capacity = getNextPowerOfTwo(size);
      m_size = size;
      m_buffer = new byte[m_capacity];
      memcpy(m_buffer, buffer, size);

      m_position = 0;
    }

    ~MemoryFile() {
      delete[] m_buffer;
    }

    int COR_CALL read(void* buffer, int size) {
      int real_read = std::min((m_size - m_position), size);
      memcpy(buffer, m_buffer + m_position, real_read);
      m_position += real_read;
      return real_read;
    }

    int COR_CALL write(const void* buffer, int size) {
      ensureSize(m_position + size);
      memcpy(m_buffer + m_position, buffer, size);
      m_position += size;
      return size;
    }

    bool COR_CALL seek(int position, SeekMode mode) {
      int real_pos;
      switch (mode) {
        case BEGIN:   real_pos = position;              break;
        case CURRENT: real_pos = m_position + position; break;
        case END:     real_pos = m_size + position;     break;
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

    int COR_CALL tell() {
      return m_position;
    }

  private:
    void ensureSize(int min_size) {
      bool realloc_needed = false;
      while (m_capacity < min_size) {
        m_capacity *= 2;
        realloc_needed = true;
      }

      if (realloc_needed) {
        byte* new_buffer = new byte[m_capacity];
        memcpy(new_buffer, m_buffer, m_size);
        delete[] m_buffer;
        m_buffer = new_buffer;
      }

      m_size = min_size;
    }

    byte* m_buffer;
    int m_position;
    int m_size;

    /// The actual size of m_buffer.  Always a power of two.
    int m_capacity;
  };

}


#endif
