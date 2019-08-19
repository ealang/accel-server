#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <glog/logging.h>

#include "src/lis3dh-driver-ifc/lis3dh-spi-dev.hh"

using namespace std;

int openSpiDeviceForLis3dh(string deviceName, uint32_t maxSpeedHz) {
  int fd = open(deviceName.c_str(), O_RDWR);
  if (fd < 0) {
    LOG(FATAL) << "Can't open device";
  }

  uint8_t mode = SPI_CPHA | SPI_CPOL;
  int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
  if (ret == -1) {
    LOG(FATAL) << "Can't set spi mode";
  }

  uint8_t bits = 8;
  ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
  if (ret == -1) {
    LOG(FATAL) << "Can't set bits per word";
  }

  ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &maxSpeedHz);
  if (ret == -1) {
    LOG(FATAL) << "Can't set max speed hz";
  }

  return fd;
}

void closeSpiDevice(int fd) {
  close(fd);
}
