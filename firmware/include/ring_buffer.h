#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__

#include "stdint.h"

namespace CentralCommand {

  template<class T, uint32_t Size>
  class RingBuffer {
    private:
      T buffer[Size];
      uint32_t read_pos;
      uint32_t write_pos;
    public:
      RingBuffer();
      bool read(T *dest);
      void write(const T &value);
      bool full;
  };

  template<class T, uint32_t Size>
  RingBuffer<T, Size>::RingBuffer():
    read_pos(0),
    write_pos(0),
    full(false) {
    // empty
  }


  template<class T, uint32_t Size>
  bool RingBuffer<T, Size>::read(T *dest) {
    // buffer empty
    if (read_pos == write_pos) {

      return false;
    }

    full = false;

    *dest = buffer[read_pos];
    read_pos = (read_pos + 1) % Size;

    return true;
  }

  template<class T, uint32_t Size>
  void RingBuffer<T, Size>::write(const T &value) {
    buffer[write_pos] = value;

    write_pos = (write_pos + 1) % Size;

    if (full) {
      read_pos = write_pos;
    }

    full = write_pos == read_pos;
  }
}

#endif /* ifndef RING_BUFFER_H */
