#ifndef INC_CAMERA_CAMERA_MOUNT_H_
#define INC_CAMERA_CAMERA_MOUNT_H_

#include "camera/pwm-info.h"
#include <stm32f1xx_hal.h>

class CameraMount {
  public:
    CameraMount(PwmInfo *pwmInfo, float minPwm, float maxPwm);
    virtual void rotateRight(uint8_t angle);
    virtual void rotateLeft(uint8_t angle);
    virtual void rotateUp(uint8_t angle);
    virtual void rotateDown(uint8_t angle);

  private:
    PwmInfo *pwmInfo_;
    float minCCR_;
    float maxCCR_;
    float  oneDegreeCCRDelta_;
};
#endif