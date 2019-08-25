#include <glog/logging.h>

#include "src/lis3dh-driver-ifc/lis3dh-spi.hh"

void lis3dh_initialize(int fd, int selectPin, uint8_t sample_rate_flags) {
  LOG(WARNING) << "Pretending to initilize device selectPin: " << selectPin;
}

void lis3dh_self_check(int fd, int selectPin) {
}

Lis3dhStatus lis3dh_status(int fd, int selectPin) {
  return Lis3dhStatus {
    .overrun = false,
    .data_available = true,
  };
}

static int16_t randomReading() {
  return rand() % MAX_ACCEL_VAL_HIGHRES;
}

Accel3 lis3dh_sample_accel(int fd, int selectPin) {
  return {
    randomReading(),
    randomReading(),
    randomReading()
  };
}
