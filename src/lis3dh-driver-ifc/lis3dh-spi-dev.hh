#pragma once

#include <string>
#include <cstdint>

/**
 * One time setup for driver.
 */
void initializeDriver();

/**
 * One time initialization per select pin.
 */
void initializeChipSelectPin(uint32_t selectPin);

/**
 * Open and configure the given SPI device using settings for the lis3dh.
 */
int openSpiDeviceForLis3dh(std::string deviceName, uint32_t maxSpeedHz);

/**
 * Close the SPI device.
 */
void closeSpiDevice(int fd);
