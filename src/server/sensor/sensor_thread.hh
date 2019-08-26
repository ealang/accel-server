#pragma once

#include <atomic>
#include <thread>
#include <unordered_map>

#include "src/server/sensor/sensor_publisher_ref.hh"

class SensorPublisher;

// Thread that can poll and publish samples from multiple accelerometers
class SensorThread {
  const int sensorDevice;
  const uint32_t sampleRateHz;
  std::shared_ptr<SensorPublisher> _publisher;
  std::thread pollThread;
  std::atomic<bool> threadExit;
  const std::unordered_map<uint32_t, uint32_t> sensorIdToSelectPin;

  void pollLoop();

public:
  SensorThread(
    int sensorDevice,
    uint32_t sampleRateHz,
    std::unordered_map<uint32_t, uint32_t> sensorIdToSelectPin
  );
  virtual ~SensorThread();

  void stop();

  SensorPublisherRef publisher() const;
};
