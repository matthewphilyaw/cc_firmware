/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for the HAL_PCD_DataInStageCallback IRQ handler
     */

    struct DeviceSdkDataInStageInterruptEvent : UsbEventDescriptor {

      uint8_t endpointNumber;

      DeviceSdkDataInStageInterruptEvent(uint8_t epnum)
        : UsbEventDescriptor(EventType::DEVICE_IRQ_DATA_IN_STAGE),
          endpointNumber(epnum) {
      }
    };
  }
}
