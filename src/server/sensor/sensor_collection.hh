#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "src/server/sensor/sensor.hh"

struct SensorCollectionSubscription {
  std::vector<int> subIds;
};

class SensorCollection {
  std::vector<std::unique_ptr<Sensor>> sensors;
  std::vector<uint32_t> _sensorIds;
public:

  const std::vector<uint32_t>& sensorIds();
  void addSensor(std::unique_ptr<Sensor> sensor);
  accel::SensorConfig sensorConfig();

  SensorCollectionSubscription subscribeAll(
    std::function<void(const accel::AccelSample&)> callback
  );
  void unsubscribeAll(SensorCollectionSubscription subscription);
};
