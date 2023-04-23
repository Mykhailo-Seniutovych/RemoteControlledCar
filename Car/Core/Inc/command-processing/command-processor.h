#ifndef INC_COMMAND_PROCESSING_COMMAND_PROCESSOR_H_
#define INC_COMMAND_PROCESSING_COMMAND_PROCESSOR_H_

#include "camera/camera-mount.h"
#include "command-processing/car-command.h"
#include "command-processing/command-receiver.h"
#include "driver/car-controller.h"
#include "led/led.h"
#include <stdint.h>

class CommandProcessor {
  public:
    CommandProcessor(CarController *carController, CameraMount *cameraMount, Led *led);
    void processCommand(CarCommand cmd);

  private:
    bool isMoving_ = false;
    uint16_t ignoredNoneCmdCount_;

    CarController *carController_;
    CameraMount *cameraMount_;
    Led *led_;
    CarCommand lastCommand_;

    void stopMovement();
};

#endif