#pragma once

#include <condition_variable>
#include "src/server/sensor/sensor_publisher_ref.hh"

// Manage the state associated with a GRPC client subscription to
// accelerometer data. Receives and buffers data from sensor
// subscription.
class SubscriptionState {
  const uint32_t numSensors;
  uint32_t sensorSubscriptionId;
  const accel::SubscriptionParameters clientParams;
  SensorPublisherRef sensorPublisher;

  std::mutex lock;
  std::condition_variable cv;
  std::unordered_map<uint32_t, uint32_t> perSensorSampleCount;
  std::vector<accel::AccelSample> samples;
  bool bufferIsComplete = false;

  void clearCounts();

  void pushSample(const accel::AccelSample& sample);
  std::vector<accel::AccelSample> resetBuffer();
  bool computeBufferIsComplete() const;

public:
  SubscriptionState (
    const uint32_t numSensors,
    const accel::SubscriptionParameters& clientParams,
    SensorPublisherRef sensorPublisher
  );
  virtual ~SubscriptionState ();

  // Blocking wait for the next batch of data to be ready
  std::vector<accel::AccelSample> wait();
};
