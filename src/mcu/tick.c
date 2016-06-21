/**
 * \file
 * \brief Mili-Second systick implementation
 *
 * \author Matthew Philyaw (matthew.philyaw@gmail.com)
 */
#include<mcu/tick.h>

/**
 * \brief Define the mili-second resolution
 */
#define TIMER_FREQUENCY_HZ 1000

static volatile uint32_t TickCounter;

/**
 * \brief Initialize systick
 */
void Tick_Init(void) {
  SysTick_Config(SystemCoreClock / TIMER_FREQUENCY_HZ);
  NVIC_SetPriority(SysTick_IRQn, 0);
}

/**
 * \brief Get systick
 */
uint32_t Tick_GetMs(void) {
  return TickCounter;
}

/**
 * \brief Blocking mili-second resolution delay
 */
void Tick_DelayMs(uint32_t delayMs) {
  uint32_t startTickValue = TickCounter;

  // loop until tick counter is >= start tick
  while((TickCounter - startTickValue) < delayMs);
}

/**
 * \brief Non-Blocking mili-second
 */
int_fast8_t Tick_DelayMs_NonBlocking(uint_fast8_t reset, TickType *config) {
  uint32_t lapsedTick = 0;

  if (!config) {
    return -1; ///< invalid pointer
  }

  if (reset) {
    config->StartMs = Tick_GetMs();
    return FALSE;
  }

  lapsedTick = (Tick_GetMs() - config->StartMs);

  if (lapsedTick < config->DelayMs) {
    return FALSE;
  }

  return TRUE;
}

/**
 * \brief systick interrupt handler
 */
void SysTick_Handler(void)
{
    TickCounter++;
}
