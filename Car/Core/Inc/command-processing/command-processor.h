#ifndef INC_COMMAND_PROCESSING_COMMAND_PROCESSOR_H_
#define INC_COMMAND_PROCESSING_COMMAND_PROCESSOR_H_

#include "camera/camera-mount.h"
#include "command-processing/car-command.h"
#include "command-processing/command-reader.h"
#include <stdint.h>

class CommandProcessor {
  public:
    CommandProcessor(CameraMount *cameraMount, CommandReader *commandReader);
    virtual void processNextCommand();

  private:
    bool isCarMoving_ = false;
    bool isMountMoving_ = false;
    CameraMount *cameraMount_;
    CommandReader *commandReader_;
    CarCommand lastCommand_;
    uint16_t ignoredNoneCmdCount_;
};

#endif