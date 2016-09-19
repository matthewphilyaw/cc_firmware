/**
 * @file terminal.h
 * @author Matthew Philyaw (matthew.philyaw@gmail.com)
 *
 * @brief Simply terminal program
 */

#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include "config/stm32plus.h"
#include "config/usart.h"
#include "ring_buffer.h"

using namespace stm32plus;

namespace CentralCommand {
  class Terminal {
    private:
      Usart2<Usart2InterruptFeature> usart;
      GpioPinRef led;
      RingBuffer<uint8_t, 500> buffer;

      void led_on();
      void led_off();
      void print_info();
      void print_help();
      void print_prompt();
      void clear_screen();
      void parse_command();
      void ok();
      void invalid_command();

      enum TerminalState {
        LED_ON,
        LED_OFF,
        PRINT_INFO,
        PRINT_HELP,
        PRINT_PROMPT,
        CLEAR_SCREEN,
        PARSE_COMMAND,
        OK,
        INVALID_COMMAND
      };

      TerminalState current_state;
      TerminalState next_state;

      typedef void (Terminal::*StateFunc)(void);
      static StateFunc state_map[];

    public:
      Terminal(uint32_t baudrate, const GpioPinRef &ledRef);
      void initialise();
      void run();
      void on_interrupt(UsartEventType e);
  };
}

#endif /* ifndef TERMINAL_H */
