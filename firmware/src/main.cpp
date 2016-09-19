#include "config/stm32plus.h"
#include "config/timer.h"
#include "config/timing.h"

#include "terminal.h"

using namespace stm32plus;

/*
 * Main entry point
 */
int main() {
  MillisecondTimer::initialise();
  GpioA<DefaultDigitalOutputFeature<5> > pa;

  CentralCommand::Terminal terminal(115200, pa[5]);
  terminal.initialise();

  pa[5].set();
  for (;;) {
    terminal.run();
  }

  return 0;
}
