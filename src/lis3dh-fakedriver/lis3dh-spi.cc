#include <glog/logging.h>

#include "src/lis3dh-driver-ifc/lis3dh-spi.hh"

void lis3dhInitialize(int fd, uint32_t selectPin, uint8_t sampleRateFlags) {
  LOG(WARNING) << "Pretending to initilize device selectPin: " << selectPin;
}

void lis3dhSelfCheck(int fd, uint32_t selectPin) { }

Lis3dhStatus lis3dhStatus(int fd, uint32_t selectPin) {
  return Lis3dhStatus {
    .overrun = false,
    .dataAvailable = true,
  };
}

static float randomReading() {
  return rand() / static_cast<float>(RAND_MAX);
}

Accel3 lis3dhSampleAccel(int fd, uint32_t selectPin) {
  return {
    randomReading(),
    randomReading(),
    randomReading()
  };
}
