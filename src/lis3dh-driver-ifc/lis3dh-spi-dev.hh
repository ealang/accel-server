#pragma once

#include <string>
#include <cstdint>

/**
 * Open and configure the given SPI device using settings for the lis3dh.
 */
int openSpiDeviceForLis3dh(std::string deviceName, uint32_t maxSpeedHz);

/**
 * Close the SPI device.
 */
void closeSpiDevice(int fd);
