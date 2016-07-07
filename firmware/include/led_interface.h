/*
 * led_interface.h
 *
 *  Created on: Jun 9, 2016
 *      Author: matthewphilyaw
 */

#ifndef __LED_INTERFACE_H__
#define __LED_INTERFACE_H__

typedef struct {
  void (*On)(void);
  void (*Off)(void);
  void (*Toggle)(void);
  void (*Init)(void);
} LedInterface;

#endif /* LED_INTERFACE_H_ */
