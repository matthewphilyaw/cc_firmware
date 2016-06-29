#include "terminal.h"
#include "FIFO.h"
#include "mcu/green_led.h"
#include "mcu/usart2.h"

#define BAUDRATE 115200
#define CMD_LENGTH 20

static void process_command(void);
static void print_invalid(void);
static void print_header(void);
static void print_ok(void);
static void print_help(void);
static void print_prompt(void);
static void clear_scrn(void);

// read twice as many to ensure the command
// is captured
uint8_t cmd_buffer[CMD_LENGTH];
uint32_t num_bytes = 0;

static const char *INVALID = "Invalid Command\r";
static const char *OK = "OK\r";
static const char *HELP = "Commands:\r    l1 1|0 - toggle board Led\r    p - print header\r    h - print this help\r    c - clear screen\r";
static const char *PROMPT = "> ";
static const char *CLR = "\e[2J";

void Terminal_Init(void) {
  GreenLed.Init();
  SerialPort2.Open(BAUDRATE);
  print_header();
}

void Terminal_Process(void) {

  uint8_t buffer[CMD_LENGTH * 2];
  uint32_t num_read = SerialPort2.GetBytes(buffer, (CMD_LENGTH * 2));

  SerialPort2.SendArray(buffer, num_read);

  for (uint32_t i = 0; i < num_read; i++) {
    if ('\r' == buffer[i]) {
      process_command();
      num_bytes = 0;
    }

    if ((buffer[i] >= '0' && buffer[i] <= '9') ||
        (buffer[i] >= 'A' && buffer[i] <= 'Z') ||
        (buffer[i] >= 'a' && buffer[i] <= 'z')) {
      cmd_buffer[num_bytes] = buffer[i];
      num_bytes++;
    }
  }
}

void process_command() {
  if (num_bytes == 0) {
    print_invalid();
    return;
  }

  uint8_t *ptr = cmd_buffer;
  if (*ptr == 'l' || *ptr == 'L') {
    if (num_bytes < 2) 
    {
      print_invalid();
      return;
    }

    ptr++;
    if (*ptr == '1') {
      if (num_bytes < 3) {
        print_invalid();
        return;
      }

      ptr++;
      if (*ptr == '0') {
        GreenLed.Off();
        print_ok();
        return;
      }
      else if (*ptr == '1') {
        GreenLed.On();
        print_ok();
        return;
      }
      print_invalid();
      return;
    }
    print_invalid();
    return;
  }
  else if (*ptr == 'p' || *ptr == 'P') {
    print_header();
    return;
  }
  else if (*ptr == 'h' || *ptr == 'H') {
    print_help();
    return;
  }
  else if (*ptr == 'c' || *ptr == 'C') {
    clear_scrn();
    return;
  }
  print_invalid();
}

static void print_invalid(void) {
  SerialPort2.SendString(INVALID);
  print_prompt();
}

static void print_ok(void) {
  SerialPort2.SendString(OK);
  print_prompt();
}

static void print_help(void) {
  SerialPort2.SendString(HELP);
  print_prompt();
}

static void print_prompt(void) {
  SerialPort2.SendString(PROMPT);
}

static void clear_scrn(void) {
  SerialPort2.SendString(CLR);
  print_prompt();
}

static void print_header(void) {
  SerialPort2.SendString(CLR);
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
  SerialPort2.SendString("#################################################################\r");
  print_prompt();
}
