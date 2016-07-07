//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef CORTEXM_TYPEDEFS_H_
#define CORTEXM_TYPEDEFS_H_

// ----------------------------------------------------------------------------

#include <stdint.h>

// ----------------------------------------------------------------------------

namespace cortexm
{
  // --------------------------------------------------------------------------

  // General Cortex-M definitions
  using address_t = uint32_t;
  using registerAddress_t = address_t;

  using reg32_t = uint32_t;
  using reg16_t = uint16_t;
  using reg8_t = uint8_t;

  using registerMask_t = reg32_t;

  // 0-31
  using bitNumber_t = uint_fast8_t;

// ----------------------------------------------------------------------------
}// namespace cortexm

// ----------------------------------------------------------------------------

#endif // CORTEXM_TYPEDEFS_H_
