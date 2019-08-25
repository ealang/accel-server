#include <stdexcept>
#include "src/lis3dh-driver-ifc/lis3dh-spi.hh"

uint32_t sampleRateFlag(uint32_t sampleRateHz) {
  switch (sampleRateHz) {
    case 1:
      return LIS3DH_SAMPLE_RATE_1HZ;
    case 10:
      return LIS3DH_SAMPLE_RATE_10HZ;
    case 25:
      return LIS3DH_SAMPLE_RATE_25HZ;
    case 50:
      return LIS3DH_SAMPLE_RATE_50HZ;
    case 100:
      return LIS3DH_SAMPLE_RATE_100HZ;
    case 200:
      return LIS3DH_SAMPLE_RATE_200HZ;
    case 400:
      return LIS3DH_SAMPLE_RATE_400HZ;
    default:
      throw std::runtime_error("Unsupported sample rate");
  }
}
