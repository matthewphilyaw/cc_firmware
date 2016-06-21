//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

/// \file
/// \brief Cortex-M memory map.

#ifndef CORTEXM_MEMORY_MAP_H_
#define CORTEXM_MEMORY_MAP_H_

// ----------------------------------------------------------------------------

#include "cortexm/Types.h"

// ----------------------------------------------------------------------------

namespace cortexm
{
  // --------------------------------------------------------------------------

  /// \ingroup cortexm
  // \headerfile MemoryMap.h "hal/architecture/arm/cortexm/stm32f/stm32f4/include/MemoryMap.h"
  /// \nosubgrouping
  ///
  /// \brief Cortex-M memory map.

  class MemoryMap
  {
  public:
    /// Peripheral base address in the alias region
    static constexpr cortexm::address_t PERIPHERAL_BASE_ADDRESS = 0x40000000UL;

    /// Peripheral base address in the bitband region
    static constexpr cortexm::address_t PERIPHERAL_BITBAND_BASE_ADDRESS =
        0x42000000UL;

    // TODO: expand
  };

// ----------------------------------------------------------------------------
}// namespace cortexm

// ----------------------------------------------------------------------------

#endif // CORTEXM_MEMORY_MAP_H_
