#pragma once

#include <condition_variable>
#include <vector>

#include "src/server/sensor/sensor_collection.hh"

class SensorCollection;

/* Manage the state associated with a GRPC client subscription to
 * accelerometer data. Receives and buffers data from sensor
 * subscriptions.
 */
class SubscriptionState {
  SensorCollectionSubscription subscription;
  SensorCollection& sensors;

  std::mutex lock;
  std::condition_variable cv;
  std::unordered_map<uint32_t, uint32_t> perSensorSampleCount;
  std::vector<accel::AccelSample> samples;
  bool bufferIsComplete = false;

  void clearCounts();

  void pushSample(const accel::AccelSample& sample);
  std::vector<accel::AccelSample> resetBuffer();
  bool computeBufferIsComplete();

public:
  SubscriptionState (
    accel::SubscriptionParameters params,
    SensorCollection& sensors
  );
  virtual ~SubscriptionState ();

  // Blocking wait for the next batch of data to be ready
  std::vector<accel::AccelSample> wait();
};
