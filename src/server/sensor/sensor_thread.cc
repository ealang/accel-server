#include <unistd.h>
#include <glog/logging.h>

#include "src/lis3dh-driver-ifc/lis3dh-spi.hh"
#include "src/server/sensor/sensor_thread.hh"
#include "src/server/sensor/util.hh"

using namespace accel;
using namespace std;

// Thread-safe publish subscribe logic for sensor thread
class SensorPublisher: public Publisher<const AccelSample&> {
  int nextId = 1000;
  unordered_map<int, function<void(const AccelSample&)>> subs;
  mutex lock;

  public:
    int subscribe(function<void(const AccelSample&)> callback) override {
      lock_guard<mutex> guard(lock);
      int id = nextId++;
      subs.emplace(id, callback);
      return id;
    }
    void unsubscribe(int subscriberId) override {
      lock_guard<mutex> guard(lock);
      subs.erase(subscriberId);
    }
    void publish(const AccelSample& sample) {
      lock_guard<mutex> guard(lock);
      for (auto& sub: subs) {
        sub.second(sample);
      }
    }
};

SensorThread::SensorThread(
  int sensorDevice,
  uint32_t sampleRateHz,
  std::unordered_map<uint32_t, uint32_t> sensorIdToSelectPin
): sensorDevice(sensorDevice),
   sampleRateHz(sampleRateHz),
   _publisher(make_shared<SensorPublisher>()),
   threadExit(false),
   sensorIdToSelectPin(sensorIdToSelectPin)
{
  pollThread = thread([this] {
    pollLoop();
  });
}

SensorThread::~SensorThread() {
  stop();
  pollThread.join();
}

void SensorThread::pollLoop() {
  LOG(INFO) << "Starting sensor thread";

  unordered_map<uint32_t, uint32_t> sensorSequenceNums;
  for (auto& sensor: sensorIdToSelectPin) {
    sensorSequenceNums[sensor.first] = 0;
  }

  while (!threadExit.load()) {
    for (auto& sensor: sensorIdToSelectPin) {
      uint32_t sensorId = sensor.first;
      uint32_t selectPin = sensor.second;
      uint32_t seqNum = sensorSequenceNums[sensorId]++;

      AccelSample result;
      result.set_sensor_id(sensorId);
      result.set_sequence_num(seqNum);

      auto sample = lis3dh_sample_accel(sensorDevice, selectPin);
      result.add_data(static_cast<float>(sample.x) / MAX_ACCEL_VAL_HIGHRES);
      result.add_data(static_cast<float>(sample.y) / MAX_ACCEL_VAL_HIGHRES);
      result.add_data(static_cast<float>(sample.z) / MAX_ACCEL_VAL_HIGHRES);

      _publisher->publish(result);
    }

    usleep(
      static_cast<uint32_t>(1e6 / sampleRateHz)
    );
  }
  LOG(INFO) << "Exiting sensor thread";
}

void SensorThread::stop() {
  threadExit = true;
}

shared_ptr<Publisher<const AccelSample&>> SensorThread::publisher() const {
  return _publisher;
}
