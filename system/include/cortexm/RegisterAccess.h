//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef CORTEXM_REGISTERACCESS_H_
#define CORTEXM_REGISTERACCESS_H_

// ----------------------------------------------------------------------------

namespace cortexm
{
  using index_t = uint_fast8_t;

  template<typename Value_T, const registerAddress_t OFFSET_T>
    class TReadWrite
    {
    public:
      using value_t = Value_T;
      static constexpr registerAddress_t m_offset = OFFSET_T;

      TReadWrite() = default;

      inline static void
      __attribute__((always_inline))
      write(registerAddress_t base, value_t value)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset);
        *ptr = value;
      }

      inline static value_t
      __attribute__((always_inline))
      read(registerAddress_t base)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset);
        return *ptr;
      }

      inline static void
      __attribute__((always_inline))
      writeBits(registerAddress_t base, value_t mask, value_t value,
          value_t shift = 0)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset);
        *ptr = (*ptr & ~(mask << shift)) | ((value & mask) << shift);
      }

      inline static value_t
      __attribute__((always_inline))
      readBits(registerAddress_t base, value_t mask, value_t shift = 0)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset);
        return ((*ptr & (mask << shift)) >> shift);
      }
    };

  template<typename Value_T, const registerAddress_t OFFSET_T>
    class TReadOnly
    {
    public:
      using value_t = Value_T;
      static constexpr registerAddress_t m_offset = OFFSET_T;

      TReadOnly() = default;

      inline static value_t
      __attribute__((always_inline))
      read(registerAddress_t base)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset);
        return *ptr;
      }

      inline static value_t
      __attribute__((always_inline))
      readBits(registerAddress_t base, value_t mask, value_t shift = 0)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset);
        return ((*ptr & (mask << shift)) >> shift);
      }
    };

  template<typename Value_T, const registerAddress_t OFFSET_T>
    class TWriteOnly
    {
    public:
      using value_t = Value_T;
      static constexpr registerAddress_t m_offset = OFFSET_T;

      TWriteOnly() = default;

      inline static void
      __attribute__((always_inline))
      write(registerAddress_t base, value_t value)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset);
        *ptr = value;
      }

      inline static void
      __attribute__((always_inline))
      writeBits(registerAddress_t base, value_t mask, value_t value,
          value_t shift = 0)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset);
        *ptr = (*ptr & ~(mask << shift)) | ((value & mask) << shift);
      }

    };

  template<typename Value_T, const registerAddress_t OFFSET_T,
      const index_t SIZE_T>
    class TReadWriteArray
    {
    public:
      using value_t = Value_T;
      static constexpr registerAddress_t m_offset = OFFSET_T;

      TReadWriteArray() = default;

      inline static void
      __attribute__((always_inline))
      write(registerAddress_t base, index_t index, value_t value)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset
            + index * sizeof(value_t));
        *ptr = value;
      }

      inline static value_t
      __attribute__((always_inline))
      read(registerAddress_t base, index_t index)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset
            + index * sizeof(value_t));
        return *ptr;
      }

      inline static void
      __attribute__((always_inline))
      writeBits(registerAddress_t base, index_t index, value_t mask,
          value_t value, value_t shift = 0)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset
            + index * sizeof(value_t));
        *ptr = (*ptr & ~(mask << shift)) | ((value & mask) << shift);
      }

      inline static value_t
      __attribute__((always_inline))
      readBits(registerAddress_t base, index_t index, value_t mask,
          value_t shift = 0)
      {
        value_t* ptr = reinterpret_cast<value_t*>(base + m_offset
            + index * sizeof(value_t));
        return ((*ptr & (mask << shift)) >> shift);
      }
    };

} // namespace cortexm

// ----------------------------------------------------------------------------

#endif /* REGISTERACCESS_H_ */
