/**
 * @file FIFO.h
 * @author Matthew Philyaw (matthew.philyaw@gmail.com)
 *
 * @brief FIFO setup and utilization
 *
 * Below are a set of ifdefs that allow selective inclusion of FIFO api
 * macros for a given types. One must define the symbol for the type to make the
 * macos available for use.
 *
 * These symbols also enable the generation of other components needed by these calls
 * namel the set/gen Funs in the FIFO.c
 *
 * To enable a particular set of functions for example take the following:
 * @code
 * // common.h
 * #define FIFO_UINT8_T
 * #define FIFO_UINT32_T
 *
 * //usage
 * FIFO_Init_uint8_t(ctx, MAX_SIZE, buffer)
 * FIFO_Init_uint32_t(ctx32, MAX_SIZE, buffer)
 *
 * uint8_t foo = 12;
 * uint8_t out;
 * FIFO_Write_uint8_t(ctx, foo); // only available due to defining the symbol
 * FIFO_Read_uint8_t(ctx, out);
 *
 * // or
 *
 * uint32_t bar = 12;
 * uint32_t out_bar;
 * FIFO_Write_uint8_t(ctx32, bar);
 * FIFO_Read_uint8_t(ctx32, out_bar);
 *
 * // out == foo
 * @endcode
 */
#ifndef __FIFO_H__
#define __FIFO_H__

#include "common.h"
#include <stdint.h>

/**
 * @brief FIFO Context
 *
 * This context is initialized by the FIFO_Iinit api and serves as
 * the context to each API call. This enables multiple FIFO contexts to
 * be used by this one API.
 */
typedef struct {
  uint32_t MaxSize;     /**< Maximum size of buffer @note it's on the caller to actually allocate buffer*/
  uint32_t CurrentSize; /**< Current size in use */
  uint32_t WritePos;    /**< Current write position in the buffer */
  uint32_t ReadPos;     /**< Current read position in the buffer */
  uint32_t BufferWidth; /**< The datat type size of the buffer */
  void *Buffer;         /**< void pointer to the data buffer to use for this context */
} FIFOContext_TypeDef;

int FIFO_Init(FIFOContext_TypeDef *ctx, uint32_t maxSize, uint32_t bufferWidth, void *buffer);
int FIFO_Write(FIFOContext_TypeDef *ctx, void(* setFun)(void*, void*), void *val);
int FIFO_Read(FIFOContext_TypeDef *ctx, void(* retFun)(void*, void*), void *ret);

/**********************************************************************
*      Macros for generating type spefic FIFO get/set functions      *
**********************************************************************/

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define CREATE_FIFO_SETTER_PAIR(type) \
void TOKENPASTE2(FIFO_Set_, type)(void *addr, void *val) { *((type *)addr) = *((type *)val); }\
void TOKENPASTE2(FIFO_Get_, type)(void *addr, void *val) { *((type *)val) = *((type *)addr); }

#define CREATE_FIFO_SETTER_PROTO(type) \
void TOKENPASTE2(FIFO_Set_, type)(void *addr, void *val); \
void TOKENPASTE2(FIFO_Get_, type)(void *addr, void *val)

/**********************************************************************
*                ifdefs to include specific api macro                *
**********************************************************************/
/**
 * @brief ifdefs for type specific FIFO impl
 */
#ifdef FIFO_INT8_T
CREATE_FIFO_SETTER_PROTO(int8_t);
#define FIFO_Init_int8_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int8_t), &(buffer))
#define FIFO_Write_int8_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int8_t, &(val))
#define FIFO_Read_int8_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int8_t, &(ret))
#endif

#ifdef FIFO_INT16_T
CREATE_FIFO_SETTER_PROTO(int16_t);
#define FIFO_Init_int16_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int16_t), &(buffer))
#define FIFO_Write_int16_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int16_t, &(val))
#define FIFO_Read_int16_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int16_t, &(ret))
#endif

#ifdef FIFO_INT32_T
CREATE_FIFO_SETTER_PROTO(int32_t);
#define FIFO_Init_int32_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int32_t), &(buffer))
#define FIFO_Write_int32_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int32_t, &(val))
#define FIFO_Read_int32_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int32_t, &(ret))
#endif

#ifdef FIFO_INT64_T
CREATE_FIFO_SETTER_PROTO(int64_t);
#define FIFO_Init_int64_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int64_t), &(buffer))
#define FIFO_Write_int64_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int64_t, &(val))
#define FIFO_Read_int64_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int64_t, &(ret))
#endif

#ifdef FIFO_UINT8_T
CREATE_FIFO_SETTER_PROTO(uint8_t);
#define FIFO_Init_uint8_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint8_t), &(buffer))
#define FIFO_Write_uint8_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint8_t, &(val))
#define FIFO_Read_uint8_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint8_t, &(ret))
#endif

#ifdef FIFO_UINT16_T
CREATE_FIFO_SETTER_PROTO(uint16_t);
#define FIFO_Init_uint16_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint16_t), &(buffer))
#define FIFO_Write_uint16_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint16_t, &(val))
#define FIFO_Read_uint16_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint16_t, &(ret))
#endif

#ifdef FIFO_UINT32_T
CREATE_FIFO_SETTER_PROTO(uint32_t);
#define FIFO_Init_uint32_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint32_t), &(buffer))
#define FIFO_Write_uint32_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint32_t, &(val))
#define FIFO_Read_uint32_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint32_t, &(ret))
#endif

#ifdef FIFO_UINT64_T
CREATE_FIFO_SETTER_PROTO(uint64_t);
#define FIFO_Init_uint64_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint64_t), &(buffer))
#define FIFO_Write_uint64_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint64_t, &(val))
#define FIFO_Read_uint64_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint64_t, &(ret))
#endif

#ifdef FIFO_INT_LEAST8_T
CREATE_FIFO_SETTER_PROTO(int_least8_t);
#define FIFO_Init_int_least8_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int_least8_t), &(buffer))
#define FIFO_Write_int_least8_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int_least8_t, &(val))
#define FIFO_Read_int_least8_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int_least8_t, &(ret))
#endif

#ifdef FIFO_INT_LEAST16_T
CREATE_FIFO_SETTER_PROTO(int_least16_t);
#define FIFO_Init_int_least16_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int_least16_t), &(buffer))
#define FIFO_Write_int_least16_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int_least16_t, &(val))
#define FIFO_Read_int_least16_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int_least16_t, &(ret))
#endif

#ifdef FIFO_INT_LEAST32_T
CREATE_FIFO_SETTER_PROTO(int_least32_t);
#define FIFO_Init_int_least32_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int_least32_t), &(buffer))
#define FIFO_Write_int_least32_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int_least32_t, &(val))
#define FIFO_Read_int_least32_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int_least32_t, &(ret))
#endif

#ifdef FIFO_INT_LEAST64_T
CREATE_FIFO_SETTER_PROTO(int_least64_t);
#define FIFO_Init_int_least64_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int_least64_t), &(buffer))
#define FIFO_Write_int_least64_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int_least64_t, &(val))
#define FIFO_Read_int_least64_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int_least64_t, &(ret))
#endif

#ifdef FIFO_UINT_LEAST8_T
CREATE_FIFO_SETTER_PROTO(uint_least8_t);
#define FIFO_Init_uint_least8_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint_least8_t), &(buffer))
#define FIFO_Write_uint_least8_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint_least8_t, &(val))
#define FIFO_Read_uint_least8_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint_least8_t, &(ret))
#endif

#ifdef FIFO_UINT_LEAST16_T
CREATE_FIFO_SETTER_PROTO(uint_least16_t);
#define FIFO_Init_uint_least16_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint_least16_t), &(buffer))
#define FIFO_Write_uint_least16_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint_least16_t, &(val))
#define FIFO_Read_uint_least16_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint_least16_t, &(ret))
#endif

#ifdef FIFO_UINT_LEAST32_T
CREATE_FIFO_SETTER_PROTO(uint_least32_t);
#define FIFO_Init_uint_least32_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint_least32_t), &(buffer))
#define FIFO_Write_uint_least32_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint_least32_t, &(val))
#define FIFO_Read_uint_least32_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint_least32_t, &(ret))
#endif

#ifdef FIFO_UINT_LEAST64_T
CREATE_FIFO_SETTER_PROTO(uint_least64_t);
#define FIFO_Init_uint_least64_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint_least64_t), &(buffer))
#define FIFO_Write_uint_least64_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint_least64_t, &(val))
#define FIFO_Read_uint_least64_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint_least64_t, &(ret))
#endif

#ifdef FIFO_INT_FAST8_T
CREATE_FIFO_SETTER_PROTO(int_fast8_t);
#define FIFO_Init_int_fast8_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int_fast8_t), &(buffer))
#define FIFO_Write_int_fast8_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int_fast8_t, &(val))
#define FIFO_Read_int_fast8_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int_fast8_t, &(ret))
#endif

#ifdef FIFO_INT_FAST16_T
CREATE_FIFO_SETTER_PROTO(int_fast16_t);
#define FIFO_Init_int_fast16_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int_fast16_t), &(buffer))
#define FIFO_Write_int_fast16_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int_fast16_t, &(val))
#define FIFO_Read_int_fast16_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int_fast16_t, &(ret))
#endif

#ifdef FIFO_INT_FAST32_T
CREATE_FIFO_SETTER_PROTO(int_fast32_t);
#define FIFO_Init_int_fast32_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int_fast32_t), &(buffer))
#define FIFO_Write_int_fast32_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int_fast32_t, &(val))
#define FIFO_Read_int_fast32_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int_fast32_t, &(ret))
#endif

#ifdef FIFO_INT_FAST64_T
CREATE_FIFO_SETTER_PROTO(int_fast64_t);
#define FIFO_Init_int_fast64_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(int_fast64_t), &(buffer))
#define FIFO_Write_int_fast64_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_int_fast64_t, &(val))
#define FIFO_Read_int_fast64_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_int_fast64_t, &(ret))
#endif

#ifdef FIFO_UINT_FAST8_T
CREATE_FIFO_SETTER_PROTO(uint_fast8_t);
#define FIFO_Init_uint_fast8_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint_fast8_t), &(buffer))
#define FIFO_Write_uint_fast8_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint_fast8_t, &(val))
#define FIFO_Read_uint_fast8_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint_fast8_t, &(ret))
#endif

#ifdef FIFO_UINT_FAST16_T
CREATE_FIFO_SETTER_PROTO(uint_fast16_t);
#define FIFO_Init_uint_fast16_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint_fast16_t), &(buffer))
#define FIFO_Write_uint_fast16_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint_fast16_t, &(val))
#define FIFO_Read_uint_fast16_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint_fast16_t, &(ret))
#endif

#ifdef FIFO_UINT_FAST32_T
CREATE_FIFO_SETTER_PROTO(uint_fast32_t);
#define FIFO_Init_uint_fast32_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint_fast32_t), &(buffer))
#define FIFO_Write_uint_fast32_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint_fast32_t, &(val))
#define FIFO_Read_uint_fast32_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint_fast32_t, &(ret))
#endif

#ifdef FIFO_UINT_FAST64_T
CREATE_FIFO_SETTER_PROTO(uint_fast64_t);
#define FIFO_Init_uint_fast64_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uint_fast64_t), &(buffer))
#define FIFO_Write_uint_fast64_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uint_fast64_t, &(val))
#define FIFO_Read_uint_fast64_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uint_fast64_t, &(ret))
#endif

#ifdef FIFO_INTPTR_T
CREATE_FIFO_SETTER_PROTO(intptr_t);
#define FIFO_Init_intptr_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(intptr_t), &(buffer))
#define FIFO_Write_intptr_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_intptr_t, &(val))
#define FIFO_Read_intptr_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_intptr_t, &(ret))
#endif

#ifdef FIFO_UINTPTR_T
CREATE_FIFO_SETTER_PROTO(uintptr_t);
#define FIFO_Init_uintptr_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uintptr_t), &(buffer))
#define FIFO_Write_uintptr_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uintptr_t, &(val))
#define FIFO_Read_uintptr_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uintptr_t, &(ret))
#endif

#ifdef FIFO_INTMAX_T
CREATE_FIFO_SETTER_PROTO(intmax_t);
#define FIFO_Init_intmax_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(intmax_t), &(buffer))
#define FIFO_Write_intmax_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_intmax_t, &(val))
#define FIFO_Read_intmax_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_intmax_t, &(ret))
#endif

#ifdef FIFO_UINTMAX_T
CREATE_FIFO_SETTER_PROTO(uintmax_t);
#define FIFO_Init_uintmax_t(ctx, maxSize, buffer) FIFO_Init(&(ctx), (maxSize), sizeof(uintmax_t), &(buffer))
#define FIFO_Write_uintmax_t(ctx, val) FIFO_Write(&(ctx), FIFO_Set_uintmax_t, &(val))
#define FIFO_Read_uintmax_t(ctx, ret) FIFO_Read(&(ctx), FIFO_Get_uintmax_t, &(ret))
#endif

#endif /* ifndef FIFO_H */
