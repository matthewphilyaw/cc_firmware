/**
 * @file FIFO.c
 * @author Matthew Philyaw (matthew.philyaw@gmail.com)
 *
 * @brief FIFO implementation
 *
 */
#include "FIFO.h"

#define AccessBuffer(buffer, index, index_width) ((uint8_t *)(buffer))+((index) * (index_width))

/**
 * @brief Initialize FIFO Context
 *
 * @note It's recommend to use the generated macros for the target data type instead of using this directly
 */
int_fast8_t FIFO_Init(FIFOContext_TypeDef *ctx, uint32_t maxSize, uint32_t bufferWidth, void *buffer) {
  if (!buffer) {
    return -1;
  }

  ctx->MaxSize = maxSize;
  ctx->CurrentSize = 0;
  ctx->WritePos = 0;
  ctx->ReadPos = 0;
  ctx->BufferWidth = bufferWidth;
  ctx->Buffer = buffer;

  return 0;
}

/**
 * @brief Write value to FIFO
 *
 * Because both buffer and val are void* the use of setter funtions that are type
 * specific was needed.
 *
 * These setter functions are defined via ifdef switched on a per type basis
 * with the bodies defined in this module via macros.
 *
 * @note It's recommended to use the Api macros for a given data type instead of using this
 * directly. Those macros will handle choosing the right setter for the data type in use.
 */
int_fast8_t FIFO_Write(FIFOContext_TypeDef *ctx, void(* setFun)(void*, void*), void *val) {
  if (ctx->CurrentSize >= ctx->MaxSize) {
    return -1;
  }

  void *addr = AccessBuffer(ctx->Buffer, ctx->WritePos, ctx->BufferWidth);

  setFun(addr, val);

  ctx->CurrentSize++;
  ctx->WritePos++;

  if (ctx->WritePos >= ctx->MaxSize) {
    ctx->WritePos = 0;
  }

  return 0;
}

/**
 * @brief Read value to FIFO
 *
 * Because both buffer and val are void* the use of getter funtions that are type
 * specific was needed.
 *
 * These getter functions are defined via ifdef switched on a per type basis
 * with the bodies defined in this module via macros.
 *
 * @note It's recommended to use the Api macros for a given data type instead of using this
 * directly. Those macros will handle choosing the right getter for the data type in use.
 */
int_fast8_t FIFO_Read(FIFOContext_TypeDef *ctx, void(* getFun)(void*, void*), void *ret) {
  if (ctx->CurrentSize <= 0) {
    return -1;
  }

  void *addr = AccessBuffer(ctx->Buffer, ctx->ReadPos, ctx->BufferWidth);

  getFun(addr, ret);

  ctx->CurrentSize--;
  ctx->ReadPos++;

  if (ctx->ReadPos >= ctx->MaxSize) {
    ctx->ReadPos = 0;
  }

  return 0;
}

/**
 * @brief backing getter and setter function pointers that are enabled if the 
 * symbol is defined.
 */
#ifdef FIFO_INT8_T
CREATE_FIFO_SETTER_PAIR(int8_t);
#endif

#ifdef FIFO_INT16_T
CREATE_FIFO_SETTER_PAIR(int16_t);
#endif

#ifdef FIFO_INT32_T
CREATE_FIFO_SETTER_PAIR(int32_t);
#endif

#ifdef FIFO_INT64_T
CREATE_FIFO_SETTER_PAIR(int64_t);
#endif

#ifdef FIFO_UINT8_T
CREATE_FIFO_SETTER_PAIR(uint8_t);
#endif

#ifdef FIFO_UINT16_T
CREATE_FIFO_SETTER_PAIR(uint16_t);
#endif

#ifdef FIFO_UINT32_T
CREATE_FIFO_SETTER_PAIR(uint32_t);
#endif

#ifdef FIFO_UINT64_T
CREATE_FIFO_SETTER_PAIR(uint64_t);
#endif

#ifdef FIFO_INT_LEAST8_T
CREATE_FIFO_SETTER_PAIR(int_least8_t);
#endif

#ifdef FIFO_INT_LEAST16_T
CREATE_FIFO_SETTER_PAIR(int_least16_t);
#endif

#ifdef FIFO_INT_LEAST32_T
CREATE_FIFO_SETTER_PAIR(int_least32_t);
#endif

#ifdef FIFO_INT_LEAST64_T
CREATE_FIFO_SETTER_PAIR(int_least64_t);
#endif

#ifdef FIFO_UINT_LEAST8_T
CREATE_FIFO_SETTER_PAIR(uint_least8_t);
#endif

#ifdef FIFO_UINT_LEAST16_T
CREATE_FIFO_SETTER_PAIR(uint_least16_t);
#endif

#ifdef FIFO_UINT_LEAST32_T
CREATE_FIFO_SETTER_PAIR(uint_least32_t);
#endif

#ifdef FIFO_UINT_LEAST64_T
CREATE_FIFO_SETTER_PAIR(uint_least64_t);
#endif

#ifdef FIFO_INT_FAST8_T
CREATE_FIFO_SETTER_PAIR(int_fast8_t);
#endif

#ifdef FIFO_INT_FAST16_T
CREATE_FIFO_SETTER_PAIR(int_fast16_t);
#endif

#ifdef FIFO_INT_FAST32_T
CREATE_FIFO_SETTER_PAIR(int_fast32_t);
#endif

#ifdef FIFO_INT_FAST64_T
CREATE_FIFO_SETTER_PAIR(int_fast64_t);
#endif

#ifdef FIFO_UINT_FAST8_T
CREATE_FIFO_SETTER_PAIR(uint_fast8_t);
#endif

#ifdef FIFO_UINT_FAST16_T
CREATE_FIFO_SETTER_PAIR(uint_fast16_t);
#endif

#ifdef FIFO_UINT_FAST32_T
CREATE_FIFO_SETTER_PAIR(uint_fast32_t);
#endif

#ifdef FIFO_UINT_FAST64_T
CREATE_FIFO_SETTER_PAIR(uint_fast64_t);
#endif

#ifdef FIFO_INTPTR_T
CREATE_FIFO_SETTER_PAIR(intptr_t);
#endif

#ifdef FIFO_UINTPTR_T
CREATE_FIFO_SETTER_PAIR(uintptr_t);
#endif

#ifdef FIFO_INTMAX_T
CREATE_FIFO_SETTER_PAIR(intmax_t);
#endif

#ifdef FIFO_UINTMAX_T
CREATE_FIFO_SETTER_PAIR(uintmax_t);
#endif
