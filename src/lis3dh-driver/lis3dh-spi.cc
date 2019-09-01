#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include <glog/logging.h>
#include <wiringPi.h>

#include "src/lis3dh-driver-ifc/lis3dh-spi.hh"
#include "src/lis3dh-driver/lis3dh-registers.hh"

constexpr uint32_t IDENTITY = 0x33;
constexpr uint32_t READ_OP = 0x80;
constexpr uint32_t READ_AUTO_INC_OP = READ_OP | 0x40;

// hard-coded scale setting
constexpr int32_t SCALE = 2;
constexpr uint32_t CTRL_REG1_SCALE_FLAG = 0;  // ±2g
constexpr uint32_t MAX_ACCEL_VAL_HIGHRES = ((1 << 15) - 1) & ~0xF;

static void transaction(int fd, uint32_t selectPin, uint8_t *tx, uint32_t size, uint8_t *rx) {
  spi_ioc_transfer tr = {
    .tx_buf = (unsigned long)tx,
    .rx_buf = (unsigned long)rx,
    .len = size,
    .speed_hz = 0,
    .delay_usecs = 0,
    .bits_per_word = 8,
    .cs_change = 0,
  };

  digitalWrite(selectPin, 0);
  int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  digitalWrite(selectPin, 1);

  LOG_IF(ERROR, ret < 1) << "can't send spi message";
}

static void writeReg(int fd, uint32_t selectPin, uint8_t reg, uint8_t value) {
  uint8_t rx[2];
  uint8_t tx[2] = {reg, value};
  transaction(fd, selectPin, tx, 2, rx);
}

static uint8_t readReg8(int fd, uint32_t selectPin, uint8_t reg) {
  uint8_t rx[2];
  uint8_t tx[2] = {
    static_cast<uint8_t>(reg | READ_OP), 0
  };
  transaction(fd, selectPin, tx, 2, rx);
  return rx[1];
}

static uint16_t readReg16(int fd, uint32_t selectPin, uint8_t reg) {
  uint8_t rx[3] = {0, 0, 0};
  uint8_t tx[3] = {
    static_cast<uint8_t>(reg | READ_AUTO_INC_OP), 0, 0
  };
  transaction(fd, selectPin, tx, 3, rx);
  return static_cast<uint16_t>(rx[1]) | static_cast<uint16_t>(rx[2]) << 8;
}

void lis3dhInitialize(int fd, uint32_t selectPin, uint8_t sampleRateFlags) {
  uint8_t axes_en_flags = 0x7; // enable all axes
  uint8_t reg_1_val = sampleRateFlags << 4 | axes_en_flags;
  writeReg(fd, selectPin, LIS3DH_CTRL_REG1, reg_1_val);

  uint8_t scale_flag = CTRL_REG1_SCALE_FLAG;
  uint8_t high_res_flag = 1; // enable high res
  uint8_t reg_4_val = scale_flag << 4 | high_res_flag << 3;
  writeReg(fd, selectPin, LIS3DH_CTRL_REG4, reg_4_val);
}

void lis3dhSelfCheck(int fd, uint32_t selectPin) {
  uint8_t readIdentity = readReg8(fd, selectPin, LIS3DH_WHO_AM_I);
  LOG_IF(
    FATAL,
    readIdentity != IDENTITY
  ) << "failed to communicate with lis3dh, unexpected who_ami_i";
}

Lis3dhStatus lis3dhStatus(int fd, uint32_t selectPin) {
  uint8_t reg = readReg8(fd, selectPin, LIS3DH_STATUS_REG2);
  return {
    .overrun = static_cast<bool>(reg & 0x80),
    .dataAvailable = static_cast<bool>(reg & 0x8),
  };
}

static float accelToFloat(uint16_t accel) {
  return static_cast<float>(
      static_cast<int16_t>(accel)
  ) / MAX_ACCEL_VAL_HIGHRES * SCALE;
}

static float accelFromReg(int fd, int selectPin, uint8_t reg) {
  return accelToFloat(readReg16(fd, selectPin, reg));
}

Accel3 lis3dhSampleAccel(int fd, uint32_t selectPin) {
  return {
    accelFromReg(fd, selectPin, LIS3DH_OUT_X_L),
    accelFromReg(fd, selectPin, LIS3DH_OUT_Y_L),
    accelFromReg(fd, selectPin, LIS3DH_OUT_Z_L),
  };
}
