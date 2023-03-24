#include "camera/camera-mount.h"
#include "camera/pwm-info.h"

#define MAX_ANGLE 180

CameraMount::CameraMount(PwmInfo *pwmInfo, float minPwm, float maxPwm) : pwmInfo_(pwmInfo) {
    minCCR_ = minPwm * (*pwmInfo_->ARR);
    maxCCR_ = maxPwm * (*pwmInfo_->ARR);
    oneDegreeCCRDelta_ = (maxCCR_ - minCCR_) / MAX_ANGLE;
};

void CameraMount::rotateRight(uint8_t angle) {
    float ccrDelta = angle * oneDegreeCCRDelta_;
    uint32_t newCcr = *(pwmInfo_->horCCR) - ccrDelta;
    if (newCcr > minCCR_) {
        *(pwmInfo_->horCCR) = newCcr;
    }
}

void CameraMount::rotateLeft(uint8_t angle) {
    float ccrDelta = angle * oneDegreeCCRDelta_;
    uint32_t newCcr = *(pwmInfo_->horCCR) + ccrDelta;
    if (newCcr < maxCCR_) {
        *(pwmInfo_->horCCR) = newCcr;
    }
}

void CameraMount::rotateUp(uint8_t angle) {
    float ccrDelta = angle * oneDegreeCCRDelta_;
    uint32_t newCcr = *(pwmInfo_->vertCCR) - ccrDelta;
    if (newCcr > minCCR_) {
        *(pwmInfo_->vertCCR) = newCcr;
    }
}

void CameraMount::rotateDown(uint8_t angle) {
    float ccrDelta = angle * oneDegreeCCRDelta_;
    uint32_t newCcr = *(pwmInfo_->vertCCR) + ccrDelta;
    if (newCcr < maxCCR_) {
        *(pwmInfo_->vertCCR) = newCcr;
    }
}