#include "config/stm32plus.h"
#include "config/usart.h"

#include "common.h"
#include "terminal.h"

using namespace stm32plus;

#define CLR "\e[2J"
#define PROMPT "> "
#define INVALID "Invalid Command\r"
#define OK_STRING "OK\r"

static const char *info_text[] {
  CLR,
  "********************************************************************************\r",
  "    Firmware Version: ",
  FIRMWARE_VERSION,
  "\r    Hardware Version: ",
  HARDWARE_VERSION,
  "\r    Build Date:       ",
  COMPILED_DATA_TIME
  "\r********************************************************************************\r",
};

static const char *help_text[] {
  "h|?    - prints this text\r",
  "p      - prints header and clears screen\r",
  "c      - clears screen\r",
  "l1 1|0 - turns l1 on (1) or off(0)\r",
};

namespace CentralCommand {
  Terminal::Terminal(uint32_t baudrate, const GpioPinRef &ledRef):
    usart(baudrate),
    led(ledRef),
    current_state(TerminalState::PRINT_INFO),
    next_state(TerminalState::INVALID_COMMAND)
  {
    //empty
  }

  void Terminal::initialise() {
    usart.UsartInterruptEventSender.insertSubscriber(
      UsartInterruptEventSourceSlot::bind(
        this,
        &Terminal::on_interrupt)
    );

    usart.enableInterrupts(Usart2InterruptFeature::RECEIVE);
  }

  void Terminal::run() {
    (this->*(state_map[current_state]))();
  }

  void Terminal::led_on() {
    led.set();
    current_state = Terminal::OK;
  }

  void Terminal::led_off() {
    led.reset();
    current_state = Terminal::OK;
  }

  void Terminal::print_info() {
    for (uint32_t i = 0; i < (sizeof(info_text) / sizeof(info_text[0])); i++) {
      const char *l = info_text[i];

      while (*l != '\0') {
        usart.send(*l++);
      }
    }
    current_state = Terminal::PRINT_PROMPT;
  }

  void Terminal::print_help() {
    for (uint32_t i = 0; i < (sizeof(help_text) / sizeof(help_text[0])); i++) {
      const char *l = help_text[i];

      while (*l != '\0') {
        usart.send(*l++);
      }
    }
    current_state = Terminal::PRINT_PROMPT;
  }

  void Terminal::print_prompt() {
    const char *prompt = PROMPT;

    while(*prompt != '\0') {
      usart.send(*prompt++);
    }

    current_state = Terminal::PARSE_COMMAND;
    next_state = Terminal::INVALID_COMMAND;
  }

  void Terminal::clear_screen() {
      const char *clr = CLR;

      while(*clr != '\0') {
        usart.send(*clr++);
      }

      current_state = Terminal::PRINT_PROMPT;
  }

  void Terminal::ok() {
    const char *ok = OK_STRING;

    while(*ok != '\0') {
      usart.send(*ok++);
    }

    current_state = Terminal::PRINT_PROMPT;
  }

  void Terminal::invalid_command() {
    const char *invalid_command = INVALID;

    while(*invalid_command != '\0') {
      usart.send(*invalid_command++);
    }

    current_state = Terminal::PRINT_PROMPT;
  }

  void Terminal::parse_command() {
    uint8_t data = 0;

    if (!buffer.read(&data)) {
      return;
    }

    usart.send(data);


    if (data == 'p') {
      next_state = Terminal::PRINT_INFO;
    }
    else if (data == '?' || data == 'h') {
      next_state = Terminal::PRINT_HELP;
    }
    else if (data == 'c') {
      next_state = Terminal::CLEAR_SCREEN;
    }
    else if (data == 'l') {
      while(!buffer.read(&data));
      usart.send(data);

      if (data == '1') {
        while(!buffer.read(&data));
        usart.send(data);

        if (data == ' ') {
          while(!buffer.read(&data));
          usart.send(data);

          if (data == '1') {
            next_state = Terminal::LED_ON;
          }
          else if (data == '0') {
            next_state = Terminal::LED_OFF;
          }
          else {
            next_state = Terminal::INVALID_COMMAND;
          }
        }
        else {
          next_state = Terminal::INVALID_COMMAND;
        }
      }
      else {
        next_state = Terminal::INVALID_COMMAND;
      }
    } 
    else if (data == '\r') {
      current_state = next_state;
      return;
    }
    else {
      next_state = Terminal::INVALID_COMMAND;
    }

    if (data == '\r') {
      current_state = next_state;
    }
  }

  void Terminal::on_interrupt(UsartEventType e) {
    if (e != UsartEventType::EVENT_RECEIVE) {
      return;
    }

    uint8_t data = usart.receive();
    buffer.write(data);
  }

  Terminal::StateFunc Terminal::state_map[] {
    &Terminal::led_on,
    &Terminal::led_off,
    &Terminal::print_info,
    &Terminal::print_help,
    &Terminal::print_prompt,
    &Terminal::clear_screen,
    &Terminal::parse_command,
    &Terminal::ok,
    &Terminal::invalid_command
  };
}
