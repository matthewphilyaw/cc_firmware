#ifndef __TICK_H__
#define __TICK_H__

#include "common.h"

typedef struct {
  uint32_t StartMs; // do not modify directly. user Tick_DelayMs_NonBlocking
  uint32_t DelayMs; // Set desired delay
} TickType;

void 		    Tick_Init(void);
uint32_t 	  Tick_GetMs(void);
int_fast8_t Tick_DelayMs_NonBlocking(uint_fast8_t reset, TickType *config);
void 		    Tick_DelayMs(uint32_t delayMs);

#endif
