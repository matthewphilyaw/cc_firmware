//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

/// \file
/// \brief Cortex-M bit-banding classes.

#ifndef CORTEXM_BIT_BAND_H_
#define CORTEXM_BIT_BAND_H_

// ----------------------------------------------------------------------------

#include "cortexm/Types.h"
#include "cortexm/MemoryMap.h"

// ----------------------------------------------------------------------------

namespace cortexm
{
  constexpr registerAddress_t PERIPHERAL_BASE =
      cortexm::MemoryMap::PERIPHERAL_BASE_ADDRESS;
  constexpr registerAddress_t PERIPHERAL_BITBAND_BASE =
      cortexm::MemoryMap::PERIPHERAL_BITBAND_BASE_ADDRESS;

  namespace bitband
  {

    using bitValue_t = uint_fast8_t;

    using bitWord_t = uint32_t;

  } // namespace bitband

  // --------------------------------------------------------------------------

  // \headerfile BitBand.h "hal/architecture/arm/cortexm/peripheral/include/BitBand.h"
  /// \ingroup arm_cm
  /// \nosubgrouping
  ///
  /// \brief Bit banding class (inline static version).
  ///
  /// \details
  /// This class has only static member functions. One trick is that
  /// the generic address can be an unsigned, a reference or a pointer,
  /// and is automatically casted to address_t.
  class BitBand
  {
  public:
    /// \name Constructors/destructor
    /// @{

    /// \brief Deleted constructor.
    BitBand(void) = delete;

    /// @} end of name Constructors/destructor

    /// \name Public member functions
    /// @{

    /// \brief Read one peripheral bit.
    ///
    /// \details
    /// Compute the bit-banding address and return the corresponding
    /// bit as a word.
    template<typename A_T>
      inline static bitband::bitValue_t
      __attribute__((always_inline))
      readPeripheralBitValue(A_T genericAddress, bitNumber_t bitNumber)
      {
        const registerAddress_t bitWordAddress =
            computePeripheralBitWordAddress(genericAddress, bitNumber);

        volatile bitband::bitWord_t* const bitWordPointer =
            reinterpret_cast<bitband::bitWord_t*>(bitWordAddress);

        // return the entire word, but only the least
        // significant bit is meaningful, all other are 0
        return static_cast<bitband::bitValue_t>(*bitWordPointer);
      }

    /// \brief Write one peripheral bit.
    ///
    /// \details
    /// Compute the bit-banding address and write the corresponding
    /// bit as a word.
    template<typename A_T>
      inline static void
      __attribute__((always_inline))
      writePeripheralBitValue(A_T genericAddress, bitNumber_t bitNumber,
          bitband::bitValue_t value)
      {
        const registerAddress_t bitWordAddress =
            computePeripheralBitWordAddress(genericAddress, bitNumber);

        volatile bitband::bitWord_t* const BitWordPointer =
            reinterpret_cast<bitband::bitWord_t*>(bitWordAddress);

        // only the least significant bit is used, all other are ignored
        *BitWordPointer = static_cast<bitband::bitWord_t>(value);
      }

    /// \brief Set one peripheral bit to 1.
    ///
    /// \details
    /// Compute the bit-banding address and set the corresponding
    /// bit to 1.
    template<typename A_T>
      inline static void
      __attribute__((always_inline))
      setPeripheralBit(A_T genericAddress, bitNumber_t bitNumber)
      {
        writePeripheralBitValue(genericAddress, bitNumber, 1);
      }

    /// \brief Clear one peripheral bit to 0.
    ///
    /// \details
    /// Compute the bit-banding address and secleart the corresponding
    /// bit to 0.
    template<typename A_T>
      inline static void
      __attribute__((always_inline))
      clearPeripheralBit(A_T genericAddress, bitNumber_t bitNumber)
      {
        writePeripheralBitValue(genericAddress, bitNumber, 0);
      }

    /// @} end of name Public member functions

  private:
    /// \name Private member functions
    /// @{

    /// \brief Compute the peripheral bit band address.
    ///
    /// \details
    /// Each peripheral bit has a separate word in the bit-band space.
    template<typename A_T>
      inline static registerAddress_t
      __attribute__((always_inline))
      computePeripheralBitWordAddress(A_T genericAddress, bitNumber_t bitNumber)
      {
        // The generic address can be an unsigned, a reference or a pointer,
        // so the first step is to cast it to an address_t (an unsigned).
        registerAddress_t registerAddress =
            reinterpret_cast<registerAddress_t>(genericAddress);

        // Then compute the offset in the peripheral address space
        registerAddress_t const peripheralOffset = registerAddress
            - PERIPHERAL_BASE;

        // Each bit will have its own location, so, since there are 8 bits
        // in a byte, first multiply the address by 8 and add the bit number
        // and then multiply by 4 to make each address an word offset.
        registerAddress_t const bitWordOffset = ((peripheralOffset * 8)
            + bitNumber) * 4;

        // Finally add the bit-banding base address and return the result
        return PERIPHERAL_BITBAND_BASE + bitWordOffset;
      }

    /// @} end of name Private member functions
  };

// ----------------------------------------------------------------------------

}// namespace cortexm

// ----------------------------------------------------------------------------

#endif // CORTEXM_BIT_BAND_H_
