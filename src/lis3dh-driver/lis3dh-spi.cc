#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include <glog/logging.h>
#include <wiringPi.h>

#include "src/lis3dh-driver-ifc/lis3dh-spi.hh"
#include "src/lis3dh-driver/lis3dh-registers.hh"

constexpr uint32_t IDENTITY = 0x33;
constexpr uint32_t READ_OP = 0x80;
constexpr uint32_t READ_AUTO_INC_OP = READ_OP | 0x40;
constexpr uint32_t MAX_ACCEL_VAL_HIGHRES = ((1 << 15) - 1) & ~0xF;

// hard-coded scale setting
constexpr uint32_t SCALE = 2;
constexpr uint32_t CTRL_REG1_SCALE_FLAG = 0;  // ±2g

static void transaction(int fd, int selectPin, uint8_t *tx, uint32_t size, uint8_t *rx) {
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

static void write_reg(int fd, int selectPin, uint8_t reg, uint8_t value) {
  uint8_t rx[2];
  uint8_t tx[2] = {reg, value};
  transaction(fd, selectPin, tx, 2, rx);
}

static uint8_t read_reg_8(int fd, int selectPin, uint8_t reg) {
  uint8_t rx[2];
  uint8_t tx[2] = {
    static_cast<uint8_t>(reg | READ_OP), 0
  };
  transaction(fd, selectPin, tx, 2, rx);
  return rx[1];
}

static uint16_t read_reg_16(int fd, int selectPin, uint8_t reg) {
  uint8_t rx[3] = {0, 0, 0};
  uint8_t tx[3] = {
    static_cast<uint8_t>(reg | READ_AUTO_INC_OP), 0, 0
  };
  transaction(fd, selectPin, tx, 3, rx);
  return (uint16_t)rx[1] | (uint16_t)rx[2] << 8;
}

void lis3dh_initialize(int fd, int selectPin, uint8_t sample_rate_flags) {
  uint8_t axes_en_flags = 0x7; // enable all axes
  uint8_t reg_1_val = sample_rate_flags << 4 | axes_en_flags;
  write_reg(fd, selectPin, LIS3DH_CTRL_REG1, reg_1_val);

  uint8_t scale_flag = CTRL_REG1_SCALE_FLAG;
  uint8_t high_res_flag = 1; // enable high res
  uint8_t reg_4_val = scale_flag << 4 | high_res_flag << 3;
  write_reg(fd, selectPin, LIS3DH_CTRL_REG4, reg_4_val);
}

void lis3dh_self_check(int fd, int selectPin) {
  uint8_t readIdentity = read_reg_8(fd, selectPin, LIS3DH_WHO_AM_I);
  LOG_IF(
    ERROR,
    readIdentity != IDENTITY
  ) << "failed to communicate with lis3dh, unexpected who_ami_i: "
    << readIdentity;
}

Lis3dhStatus lis3dh_status(int fd, int selectPin) {
  uint8_t reg = read_reg_8(fd, selectPin, LIS3DH_STATUS_REG2);
  return {
    static_cast<bool>(reg & 0x80),
    static_cast<bool>(reg & 0x8),
  };
}

Accel3 lis3dh_sample_accel(int fd, int selectPin) {
  return {
    static_cast<int16_t>(read_reg_16(fd, selectPin, LIS3DH_OUT_X_L)),
    static_cast<int16_t>(read_reg_16(fd, selectPin, LIS3DH_OUT_Y_L)),
    static_cast<int16_t>(read_reg_16(fd, selectPin, LIS3DH_OUT_Z_L))
  };
}
