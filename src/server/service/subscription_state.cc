#include <glog/logging.h>
#include "src/server/service/subscription_state.hh"

using namespace accel;
using namespace std;

constexpr uint32_t maxPerSensorSampleCount = 10 * 1024;

void SubscriptionState::clearCounts() {
  perSensorSampleCount.clear();
}

void SubscriptionState::pushSample(const AccelSample& sample) {
  lock_guard<mutex> guard(lock);

  uint32_t sensorId = sample.sensor_id();
  if (perSensorSampleCount[sensorId] < maxPerSensorSampleCount) {
    perSensorSampleCount[sensorId] += 1;
    samples.emplace_back(sample);
  }

  bufferIsComplete = bufferIsComplete || computeBufferIsComplete();
  if (bufferIsComplete) {
    cv.notify_one();
  }
}

vector<AccelSample> SubscriptionState::resetBuffer() {
  clearCounts();
  vector<AccelSample> samplesTmp = move(samples);
  samples = vector<AccelSample>();
  bufferIsComplete = false;
  return samplesTmp;
}

bool SubscriptionState::computeBufferIsComplete() const {
  const uint32_t perSensorBatchSize = min(
    clientParams.per_sensor_batch_size(),
    maxPerSensorSampleCount
  );

  return perSensorSampleCount.size() == numSensors && all_of(
    perSensorSampleCount.begin(),
    perSensorSampleCount.end(),
    [this, &perSensorBatchSize](auto& pair) {
      return pair.second >= perSensorBatchSize;
    }
  );
}

SubscriptionState::SubscriptionState(
  const uint32_t numSensors,
  const SubscriptionParameters& clientParams,
  SensorPublisherRef sensorPublisher
): numSensors(numSensors),
   clientParams(clientParams),
   sensorPublisher(sensorPublisher) {

  sensorSubscriptionId = sensorPublisher->subscribe([this](const AccelSample& sample) {
    pushSample(sample);
  });
}

SubscriptionState::~SubscriptionState() {
  sensorPublisher->unsubscribe(sensorSubscriptionId);
}

vector<AccelSample> SubscriptionState::wait() {
  unique_lock<mutex> guard(lock);
  cv.wait(guard, [this] {
    return bufferIsComplete;
  });
  return resetBuffer();
}
