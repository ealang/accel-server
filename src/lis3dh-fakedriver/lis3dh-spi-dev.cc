#include <glog/logging.h>

#include "src/lis3dh-driver-ifc/lis3dh-spi-dev.hh"

int openSpiDeviceForLis3dh(std::string deviceName, uint32_t maxSpeedHz) {
  LOG(WARNING) << "Pretending to open device " << deviceName;
  return 0;
}

void closeSpiDevice(int fd) {
}
