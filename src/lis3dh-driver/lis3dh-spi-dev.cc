#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <glog/logging.h>
#include <wiringPi.h>

#include "src/lis3dh-driver-ifc/lis3dh-spi-dev.hh"

using namespace std;

void initializeDriver() {
  auto result = wiringPiSetup();
  LOG_IF(FATAL, result == -1) << "Unable to setup wiringPI";
}

void initializeChipSelectPin(uint32_t selectPin) {
  // set as output and deselect
  int OUTPUT_MODE = 1;
  pinMode(selectPin, OUTPUT_MODE);
  digitalWrite(selectPin, 1);
}

int openSpiDeviceForLis3dh(string deviceName, uint32_t maxSpeedHz) {
  int fd = open(deviceName.c_str(), O_RDWR);
  LOG_IF(FATAL, fd < 0) << "Can't open device";

  uint8_t mode = SPI_CPHA | SPI_CPOL;
  int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
  LOG_IF(FATAL, ret == -1) << "Can't set spi mode";

  uint8_t bits = 8;
  ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
  LOG_IF(FATAL, ret == -1) << "Can't set bits per word";

  ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &maxSpeedHz);
  LOG_IF(FATAL, ret == -1) << "Can't set max speed hz";

  return fd;
}

void closeSpiDevice(int fd) {
  close(fd);
}
