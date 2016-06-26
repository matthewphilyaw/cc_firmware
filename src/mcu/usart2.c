/**
 * @file usart3.c
 * @author Matthew Philyaw (matthew.philyaw@gmail.com)
 *
 * @brief USART Driver implementation for SerialPort3
 */
#include "common.h"
#include "mcu/usart2.h"
#include "FIFO.h"

#define GPIO_AFRL_AFRL2_0 ((uint32_t) 0x00000100)
#define GPIO_AFRL_AFRL3_0 ((uint32_t) 0x00001000)
/**
 * @brief buffer config section
 */
static uint8_t buffer[USART2_MAX_SIZE];
static FIFOContext_TypeDef fifoContext;

/**
 * @brief Internal flag for open status
 */
static uint_fast8_t IsOpenFlag = FALSE;
static SerialResult_t lastError;

static void EnableISR() {
  NVIC_EnableIRQ(USART2_IRQn);
  NVIC_SetPriority(USART2_IRQn, 0);
}

static void DisableISR() {
  NVIC_DisableIRQ(USART2_IRQn);
}

/**
 * @brief Return the status of the Interface
 *
 * @return
 * 1 -> Open\n
 * 0 -> Closed
 */
static uint_fast8_t IsOpen(void) {
  return IsOpenFlag;
}

/**
 * @brief Closes open interface
 */
static void Close(void) {
  USART2->CR1 &= ~(1);
  DisableISR();
}

/**
 * @brief Checks if write is in progress
 *
 * @return
 * 1 -> Write in progress\n
 * 0 -> Interface ready
 */
static uint_fast8_t IsWriteBusy(void) {
  return !(USART2->ISR & USART_ISR_TXE);
}

/**
 * @brief Checks if there is data to read
 *
 * @return
 * 1 -> Data to read\n
 * 0 -> No data to read
 */
static uint_fast8_t RxBufferHasData(void) {
  return USART2->ISR & USART_ISR_RXNE;
}

static SerialResult_t GetLastError() {
  return lastError;
}

/**
 * @brief Open serial interface with specified baudrate
 * @param baudrate is the desired baudrate
 *
 * @return
 * SERIAL_FAIL    -> interface is open, nothing to do\n
 * SERIAL_SUCCESS -> interface was opened and initialized
 */
static SerialResult_t Open(uint32_t baudrate) {
  if (IsOpenFlag) {
    return SERIAL_FAIL;
  }

  FIFO_Init_uint8_t(fifoContext, USART2_MAX_SIZE, buffer);

  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
  GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
  GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3);
  GPIOA->AFR[0] |= (GPIO_AFRL_AFRL2_0 | GPIO_AFRL_AFRL3_0);


  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
  USART2->CR1 = 0;
  USART2->CR1 &= ~USART_CR1_M;    // 8 bit data
  USART2->CR2 &= ~USART_CR2_STOP; // 1 stop bit

  uint16_t baudTemp = 0;
#ifdef USART_OVER_SAMPLE_16
  USART2->CR1 &= ~USART_CR1_OVER8;
  baudTemp = (SystemCoreClock) / (baudrate);
#else
  USART2->CR1 |= USART_CR1_OVER8;
  baudTemp = (2 * SystemCoreClock) / (baudrate);
  baudTemp = ((baudTemp & 0xFFFFFFF0) | ((baudTemp >> 1) & 0x00000007))
#endif
  USART2->BRR = baudTemp;

  USART2->CR3 |= USART_CR3_EIE;
  USART2->CR1 |= USART_CR1_RXNEIE | USART_CR1_PEIE;
  USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

  EnableISR();

  IsOpenFlag = TRUE;
  return SERIAL_SUCCESS;
}

/**
 * @brief Send one byte over the interface
 * @param source is the byte to send
 *
 * @return
 * SERIAL_CLOSED -> Interface is closed and nothing is done\n
 * SERIAL_SUCCESS -> Byte has been sent
 */
static SerialResult_t SendByte(uint8_t source) {
  if (!IsOpenFlag) {
    return SERIAL_CLOSED;
  }

  while (IsWriteBusy());

  USART2->TDR = source;

  return SERIAL_SUCCESS;
}

void USART2_IRQHandler() {
  // Alwasy read the byte, even if we throw it away
  uint8_t data = USART2->RDR;

  if (USART2->ISR & USART_ISR_FE) {
    USART2->ICR |= USART_ICR_FECF;
    lastError = SERIAL_FRAMING_ERROR;
    return;
  }
  else if (USART2->ISR & USART_ISR_PE) {
    USART2->ICR |= USART_ICR_PECF;
    lastError = SERIAL_PARITY_ERROR;
    return;
  }
  else if (USART2->ISR & USART_ISR_NE) {
    USART2->ICR |= USART_ICR_NCF;
    lastError = SERIAL_NOISE_ERROR;
    return;
  }
  else if (USART2->ISR & USART_ISR_ORE) {
    USART2->ICR |= USART_ICR_ORECF;
    lastError = SERIAL_OVER_RUN;
  }
  else {
    lastError = SERIAL_SUCCESS;
  }

  FIFO_Write_uint8_t(fifoContext, data);
}


static int32_t GetByte(uint8_t *destination, uint32_t length) {
  if (!IsOpenFlag) {
    return SERIAL_CLOSED;
  }

  if (!destination) {
    return SERIAL_INVALID_PARAMETER;
  }

  if (length == 0) {
    return 0;
  }

  DisableISR();
  uint32_t minReadLength = (fifoContext.CurrentSize < length) ? fifoContext.CurrentSize : length;

  if (minReadLength > 0) {
    for (uint32_t i = 0; i < minReadLength; i++) {
      FIFO_Read_uint8_t(fifoContext, destination[i]);
    }
  }
  EnableISR();
  return minReadLength;
}

/**
 * @brief Send string over interface
 * @param source pointer to string
 *
 * @return
 * SERIAL_CLOSED -> Interface is closed and nothing is done\n
 * SERIAL_INVALID_PARAMETER -> source pointer is a null pointer\n
 * SERIAL_FAIL -> SendByte failed\n
 * SERIAL_SUCCESS -> String has been sent
 */
static SerialResult_t SendString(const char *source) {
  if (!IsOpenFlag) {
    return SERIAL_CLOSED;
  }

  if (!source) {
    return SERIAL_INVALID_PARAMETER;
  }

  while(*source) {
    if (SendByte(*source)) {
      return SERIAL_FAIL;
    }
    source++;
  }

  return SERIAL_SUCCESS;
}

/**
 * @brief Send string over interface
 * @param source pointer to byte array
 * @param length of the byte array
 *
 * @return
 * SERIAL_CLOSED -> Interface is closed and nothing is done\n
 * SERIAL_INVALID_PARAMETER -> source pointer is a null pointer\n
 * SERIAL_FAIL -> SendByte failed\n
 * SERIAL_SUCCESS -> String has been sent
 */
static SerialResult_t SendArray(const uint8_t *source, uint32_t length) {
  if (!IsOpenFlag) {
    return SERIAL_CLOSED;
  }

  if (!source) {
    return SERIAL_INVALID_PARAMETER;
  }

  for ( ; length; length--) {
    if (SendByte(*source)) {
      return SERIAL_FAIL;
    }
    source++;
  }

  return SERIAL_SUCCESS;
}

/**
 * @brief Initializing SerialPort3
 */
SerialInterface SerialPort2 = {
  IsOpen,
  RxBufferHasData,
  Open,
  Close,
  SendByte,
  SendString,
  SendArray,
  GetByte,
  GetLastError
};
