#include "src/server/service/subscription_state.hh"

using namespace accel;
using namespace std;

void SubscriptionState::clearCounts() {
  for (auto id: sensors.sensorIds()) {
    perSensorSampleCount[id] = 0;
  }
}

void SubscriptionState::pushSample(const AccelSample& sample) {
  lock_guard<mutex> guard(lock);

  uint32_t sensorId = sample.sensor_id();
  perSensorSampleCount[sensorId] += 1;
  samples.emplace_back(sample);

  bufferIsComplete = bufferIsComplete || computeBufferIsComplete();
  if (bufferIsComplete) {
    printf("driver: buffer is now complete\n");
    cv.notify_one();
 }
}

vector<AccelSample> SubscriptionState::resetBuffer() {
  printf("client: got data, resetting buffer\n");
  clearCounts();
  vector<AccelSample> samplesTmp = move(samples);
  samples = vector<AccelSample>();
  bufferIsComplete = false;
  return samplesTmp;
}

bool SubscriptionState::computeBufferIsComplete() {
  auto& sensorIds = sensors.sensorIds();
  return all_of(
    sensorIds.begin(),
    sensorIds.end(),
    [this](uint32_t sensorId) {
      return perSensorSampleCount[sensorId] > 0;
    }
  );
}

SubscriptionState::SubscriptionState(
  SubscriptionParameters params,
  SensorCollection& sensors
): sensors(sensors) {
  clearCounts();
  subscription = sensors.subscribeAll([this](const AccelSample& sample) {
    pushSample(sample);
  });
}

SubscriptionState::~SubscriptionState() {
  sensors.unsubscribeAll(subscription);
}

vector<AccelSample> SubscriptionState::wait() {
  printf("client: waiting for data\n");
  unique_lock<mutex> guard(lock);
  cv.wait(guard, [this] {
    return bufferIsComplete;
  });
  return resetBuffer();
}
