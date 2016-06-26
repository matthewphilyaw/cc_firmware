#include "common.h"
#include "FIFO.h"
#include "mcu/tick.h"
#include "mcu/green_led.h"
#include "mcu/usart2.h"

#define BAUDRATE 115200

static void PrintHeader(void);
void HardFault_Handler(void);

int main(void) {
  Tick_Init();
  GreenLed.Init();

  SerialPort2.Open(BAUDRATE);
  PrintHeader();

  GreenLed.On();
  uint8_t buf[USART2_MAX_SIZE];
  uint32_t numRead = 0;
  for (;;) {
    numRead = SerialPort2.GetByte(buf, USART2_MAX_SIZE);
    if (numRead > 0) {
      SerialPort2.SendArray(buf, numRead);
    }
    Tick_DelayMs(20);
  }
}

/**
 * @brief Print info header
 *
 * This function prints a info head to the serial device that contains the
 * firmware and hardware versions as well as last compile date.
 */
static void PrintHeader(void) {
  SerialPort2.SendString("\e[2J");
  SerialPort2.SendString("#################################################################\r");
  SerialPort2.SendString("    Firmware Version: ");
  SerialPort2.SendString(FIRMWARE_VERSION);
  SerialPort2.SendString("\r");
  SerialPort2.SendString("    Hardware Version: ");
  SerialPort2.SendString(HARDWARE_VERSION);
  SerialPort2.SendString("\r");
  SerialPort2.SendString("    Build Date: ");
  SerialPort2.SendString(COMPILED_DATA_TIME);
  SerialPort2.SendString("\r");
  SerialPort2.SendString("#################################################################\r\r");
}

/**
 * @brief Provides a implementation for the CMSIS hard fault handler
 */
void HardFault_Handler(void) {
  for(;;) {
    GreenLed.Toggle();
    for (int i = 0; i < 1000000; i++);
  }
}
