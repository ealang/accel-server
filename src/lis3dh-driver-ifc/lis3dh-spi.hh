#pragma once

#include <cstdint>

struct Accel3 {
  int16_t x, y, z;
};

struct Lis3dhStatus {
  // data is not being read fast enough
  bool overrun;
  // data is available to bool
  bool dataAvailable;
};

constexpr uint32_t MAX_ACCEL_VAL_HIGHRES = ((1 << 15) - 1) & ~0xF;

constexpr uint32_t LIS3DH_SAMPLE_RATE_1HZ = 0b1;
constexpr uint32_t LIS3DH_SAMPLE_RATE_10HZ = 0b10;
constexpr uint32_t LIS3DH_SAMPLE_RATE_25HZ = 0b11;
constexpr uint32_t LIS3DH_SAMPLE_RATE_50HZ = 0b100;
constexpr uint32_t LIS3DH_SAMPLE_RATE_100HZ = 0b101;
constexpr uint32_t LIS3DH_SAMPLE_RATE_200HZ = 0b110;
constexpr uint32_t LIS3DH_SAMPLE_RATE_400HZ = 0b111;

/**
 * Convert from Hz to sample rate flag.
 */
uint32_t sampleRateFlag(uint32_t sampleRateHz);

/**
 * Write values to the lis3dh registers to initialize it.
 */
void lis3dhInitialize(int fd, uint32_t selectPin, uint8_t sampleRateFlags);

/**
 * Test if we can communicate with the lis3dh.
 */
void lis3dhSelfCheck(int fd, uint32_t selectPin);

/**
 * Check data status of the device.
 */
Lis3dhStatus lis3dhStatus(int fd, uint32_t selectPin);

/**
 * Take an acceleration sample.
 */
Accel3 lis3dhSampleAccel(int fd, uint32_t selectPin);
