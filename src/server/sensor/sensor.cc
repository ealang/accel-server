#include <unistd.h>
#include <glog/logging.h>

#include "src/server/sensor/config.hh"
#include "src/server/sensor/sensor.hh"
#include "src/server/sensor/util.hh"

using namespace accel;
using namespace std;

uint32_t randomReading() {
  return rand() % MAX_ACCEL_VAL_HIGHRES;
}

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

Sensor::Sensor(
  uint32_t sensorId
): _publisher(make_shared<SensorPublisher>()),
   threadExit(false),
   sensorId(sensorId)
{
  pollThread = thread([this] {
    pollLoop();
  });
}

Sensor::~Sensor() {
  stop();
  pollThread.join();
}

void Sensor::pollLoop() {
  LOG(INFO) << "Starting sensor thread " << sensorId;
  uint32_t seqNum = 0;
  while (!threadExit.load()) {
    AccelSample sample;
    sample.set_sensor_id(sensorId);
    sample.set_sequence_num(seqNum++);
    sample.set_time_ms(time_ms());
    sample.add_data(randomReading());
    sample.add_data(randomReading());
    sample.add_data(randomReading());

    _publisher->publish(sample);
    usleep(
      static_cast<uint32_t>(1e6 / SAMPLE_RATE_HZ)
    );
  }
  LOG(INFO) << "Exiting sensor thread " << sensorId;
}

void Sensor::stop() {
  threadExit = true;
}

shared_ptr<Publisher<const AccelSample&>> Sensor::publisher() {
  return _publisher;
}
