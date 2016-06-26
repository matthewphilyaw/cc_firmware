#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>

#include "stm32f0xx.h"

#define TRUE 1
#define FALSE 0

#define FIRMWARE_VERSION "00.001D"
#define HARDWARE_VERSION "01"
#define COMPILED_DATA_TIME "[" __DATE__ " " __TIME__ "]"

#define EN_DEBUG_INTERFACE

#define USART_OVER_SAMPLE_16
#define FIFO_UINT8_T

/**
 * @brief usart2 max buffer
 */
#define  USART2_MAX_SIZE 2048

#endif /* ifndef COMMON_H */
