#include "common.h"
#include "FIFO.h"
#include "mcu/tick.h"
#include "mcu/green_led.h"

int main(void) {
  Tick_Init();
  GreenLed.Init();

  for (;;) {
    GreenLed.Toggle();
    Tick_DelayMs(500);
  }
}
