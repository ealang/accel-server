#pragma once

#include <cstdint>

struct Accel3 {
  int16_t x, y, z;
};

struct Lis3dhStatus {
  // data is not being read fast enough
  bool overrun;
  // data is available to bool
  bool data_available;
};

constexpr uint32_t LIS3DH_SAMPLE_RATE_1HZ = 0b1;
constexpr uint32_t LIS3DH_SAMPLE_RATE_10HZ = 0b10;
constexpr uint32_t LIS3DH_SAMPLE_RATE_25HZ = 0b11;
constexpr uint32_t LIS3DH_SAMPLE_RATE_50HZ = 0b100;
constexpr uint32_t LIS3DH_SAMPLE_RATE_100HZ = 0b101;
constexpr uint32_t LIS3DH_SAMPLE_RATE_200HZ = 0b110;
constexpr uint32_t LIS3DH_SAMPLE_RATE_400HZ = 0b111;

/**
 * Write values to the lis3dh registers to initialize it.
 */
void lis3dh_initialize(int fd, int selectPin, uint8_t sample_rate_flags);

/**
 * Test if we can communicate with the lis3dh.
 */
void lis3dh_self_check(int fd, int selectPin);

/**
 * Check data status of the device.
 */
Lis3dhStatus lis3dh_status(int fd, int selectPin);

/**
 * Take an acceleration sample.
 */
Accel3 lis3dh_sample_accel(int fd, int selectPin);
