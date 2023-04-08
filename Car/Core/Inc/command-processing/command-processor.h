#ifndef INC_COMMAND_PROCESSING_COMMAND_PROCESSOR_H_
#define INC_COMMAND_PROCESSING_COMMAND_PROCESSOR_H_

#include "camera/camera-mount.h"
#include "command-processing/car-command.h"
#include "command-processing/command-reader.h"
#include "driver/driver.h"
#include "led/led.h"
#include <stdint.h>

class CommandProcessor {
  public:
    CommandProcessor(Driver *driver, CameraMount *cameraMount, CommandReader *commandReader, Led *led);
    virtual void processNextCommand();

  private:
    bool isCarMoving_ = false;
    bool isMountMoving_ = false;
    uint16_t ignoredNoneCmdCount_;

    Driver *driver_;
    CameraMount *cameraMount_;
    CommandReader *commandReader_;
    Led *led_;
    CarCommand lastCommand_;
};

#endif