#include <glog/logging.h>
#include "src/server/service/subscription_state.hh"

using namespace accel;
using namespace std;

void SubscriptionState::clearCounts() {
  perSensorSampleCount.clear();
}

void SubscriptionState::pushSample(const AccelSample& sample) {
  lock_guard<mutex> guard(lock);

  uint32_t sensorId = sample.sensor_id();
  perSensorSampleCount[sensorId] += 1;
  samples.emplace_back(sample);

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

bool SubscriptionState::computeBufferIsComplete() {
  return perSensorSampleCount.size() == numSensors && all_of(
    perSensorSampleCount.begin(),
    perSensorSampleCount.end(),
    [this](auto& pair) {
      return pair.second > 0;
    }
  );
}

SubscriptionState::SubscriptionState(
  uint32_t numSensors,
  SubscriptionParameters params,
  SensorPublisherRef sensorPublisher
): numSensors(numSensors), sensorPublisher(sensorPublisher) {

  subscription = sensorPublisher->subscribe([this](const AccelSample& sample) {
    pushSample(sample);
  });
}

SubscriptionState::~SubscriptionState() {
  sensorPublisher->unsubscribe(subscription);
}

vector<AccelSample> SubscriptionState::wait() {
  unique_lock<mutex> guard(lock);
  cv.wait(guard, [this] {
    return bufferIsComplete;
  });
  return resetBuffer();
}
