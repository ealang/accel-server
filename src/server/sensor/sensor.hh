#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include "src/protos/accel.pb.h"
#include "src/server/sensor/publisher.hh"

class SensorPublisher;

class Sensor {
  std::shared_ptr<SensorPublisher> _publisher;
  std::thread pollThread;
  std::atomic<bool> threadExit;

  void pollLoop();

public:
  const uint32_t sensorId;

  Sensor(uint32_t sensorId);
  virtual ~Sensor();

  void stop();

  std::shared_ptr<Publisher<const accel::AccelSample&>> publisher();
};
