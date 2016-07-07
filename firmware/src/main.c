#include "common.h"
#include "mcu/tick.h"
#include "terminal.h"

int main(void) {
  Tick_Init();
  Terminal_Init();
  for (;;) {
    Terminal_Process();
  }
}

/**
 * @brief Provides a implementation for the CMSIS hard fault handler
 */
void HardFault_Handler(void) {
  for(;;) {
    for (int i = 0; i < 1000000; i++);
  }
}
