#include "camera/camera-mount.h"
#include "camera/mount-pwm.h"

#define MAX_ANGLE 180

CameraMount::CameraMount(MountPwm *pwmInfo, float minPwm, float maxPwm) : pwm_(pwmInfo) {
    minCCR_ = minPwm * (*pwm_->ARR);
    maxCCR_ = maxPwm * (*pwm_->ARR);
    oneDegreeCCRDelta_ = (maxCCR_ - minCCR_) / MAX_ANGLE;
};

void CameraMount::rotateRight(uint8_t angle) {
    float ccrDelta = angle * oneDegreeCCRDelta_;
    uint32_t newCcr = *(pwm_->horCCR) - ccrDelta;
    if (newCcr > minCCR_) {
        *(pwm_->horCCR) = newCcr;
    }
}

void CameraMount::rotateLeft(uint8_t angle) {
    float ccrDelta = angle * oneDegreeCCRDelta_;
    uint32_t newCcr = *(pwm_->horCCR) + ccrDelta;
    if (newCcr < maxCCR_) {
        *(pwm_->horCCR) = newCcr;
    }
}

void CameraMount::rotateUp(uint8_t angle) {
    float ccrDelta = angle * oneDegreeCCRDelta_;
    uint32_t newCcr = *(pwm_->vertCCR) - ccrDelta;
    if (newCcr > minCCR_) {
        *(pwm_->vertCCR) = newCcr;
    }
}

void CameraMount::rotateDown(uint8_t angle) {
    float ccrDelta = angle * oneDegreeCCRDelta_;
    uint32_t newCcr = *(pwm_->vertCCR) + ccrDelta;
    if (newCcr < maxCCR_) {
        *(pwm_->vertCCR) = newCcr;
    }
}