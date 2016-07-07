/**
 * @file serial_interface.h
 * @author Matthew Philyaw (matthew.philyaw@gmail.com)
 *
 * @brief Header for the SerialInterface struct and result codes
 *
 * Any function that can return errors will return a SerialResult_t
 * enum that can then be used to determine the exact nature of the error
 */
#ifndef __SERIAL_INTERFACE__
#define __SERIAL_INTERFACE__

#include "common.h"

/**
 * @brief SerialResult_t contains possible error codes the interface will return
 *
 * The codes listed below represent the possible error states
 * certain functions can return and should be checked by the caller
 * where SERIAL_SUCCESS is 0 (logical false). Thus errors can be detected
 * by checking if SerialResult_t is true.
 *
 * Example:
 * @code
 * uint8_t byte;
 * SerialResult_t err = SomeSerialInstance.GetByte(&byte);
 *
 * if (err) {
 *   switch(err) {
 *     case SERIAL_FAIL:
 *       // do something
 *       break;
 *     case SERIAL_NO_DATA:
 *       // do something
 *       break;
 *     ...
 *   }
 * }
 * @endcode
 */
typedef enum {
  SERIAL_SUCCESS = 0,             /**< Operation succeeded */
  SERIAL_FAIL = -1,               /**< Operation fail without a specific case */
  SERIAL_CLOSED = -2,             /**< Serial interface is closed */
  SERIAL_NO_DATA = -3,            /**< No data available on interface */

  SERIAL_OVER_RUN = -4,           /**< Over run detected */
  SERIAL_FRAMING_ERROR = -5,      /**< Framing error detected*/
  SERIAL_PARITY_ERROR = -6,       /**< Parity error detected */
  SERIAL_NOISE_ERROR = - 7,       /**< Noise error detected */
  SERIAL_LINE_BREAK_ERROR = -8,   /**< Line break detected */
  SERIAL_INVALID_PARAMETER = -9   /**< Invalid parameter passed to a serial interface function */
} SerialResult_t;

/**
 * @brief SerialInterface struct represents an interface to USART device
 */
typedef struct {
  uint_fast8_t   (*IsOpen)(void);                                       /**< Check if interface is open */
  uint_fast8_t   (*RxBufferHasData)(void);                              /**< Check if data is available @note other functions may return this */
  SerialResult_t (*Open)(uint32_t baudrate);                            /**< Open interface and initialize with specified baudrate */
  void           (*Close)(void);                                        /**< Closes interface */
  SerialResult_t (*SendByte)(uint8_t);                                  /**< Send one byte over the interface */
  SerialResult_t (*SendString)(const char *source);                     /**< Send a string over the interface */
  SerialResult_t (*SendArray)(const uint8_t *source, uint32_t length);  /**< Send an array of bytes over the interface */
  int32_t (*GetBytes)(uint8_t *destination, uint32_t length);                      /**< Retrieve one byte from the interface */
  SerialResult_t (*GetLastError)(void);
} SerialInterface;

#endif
