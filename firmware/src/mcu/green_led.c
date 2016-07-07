/**
 * \file
 * \author Matthew Philyaw
 * \brief Led Driver
 *
 * Green Led on the Nucleo F446ZE board
 *
 */
#include <mcu/green_led.h>

/**
 * \brief GPIO defines for driving the output
 */
#define LED_PIN_BS  GPIO_BSRR_BS_5
#define LED_PIN_BRR GPIO_BRR_BR_5
#define LED_PIN_ODR GPIO_ODR_5


/**
 * \brief Turn Led on
 */
static void Led_On(void) {
    GPIOA->BSRR |= LED_PIN_BS;
}

/**
 * \brief Turn Led off
 */
static void Led_Off(void) {
  GPIOA->BRR |= LED_PIN_BRR;
}

/**
 * \brief Toggle Led
 */
static void Led_Toggle(void) {
  if (GPIOA->ODR & LED_PIN_ODR) {
    Led_Off();
    return;
  }

  Led_On();
}

/**
 * \brief Init Led
 *
 * This will be specific to the Led in use
 */
static void Led_Init(void) {
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

  GPIOA->MODER   &= ~GPIO_MODER_MODER5;
  GPIOA->MODER   |= GPIO_MODER_MODER5_0;

  GPIOA->OTYPER  &= ~GPIO_OTYPER_OT_5;
  GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR5;
  GPIOA->PUPDR   &= ~GPIO_PUPDR_PUPDR5;

  Led_Off();
}

/**
 * \brief Setup LedInterface Interface
 */
LedInterface GreenLed = {
    Led_On,
    Led_Off,
    Led_Toggle,
    Led_Init
};
