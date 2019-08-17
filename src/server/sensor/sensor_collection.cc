#include <boost/range/combine.hpp>

#include "src/server/sensor/config.hh"
#include "src/server/sensor/sensor_collection.hh"

using namespace accel;
using namespace std;

const vector<uint32_t>& SensorCollection::sensorIds() {
  return _sensorIds;
}

void SensorCollection::addSensor(unique_ptr<Sensor> sensor) {
  _sensorIds.emplace_back(sensor->sensorId);
  sensors.emplace_back(move(sensor));
}

SensorConfig SensorCollection::sensorConfig() {
  SensorConfig cfg;
  cfg.set_sample_rate_hz(SAMPLE_RATE_HZ);
  cfg.set_max_sample_value(MAX_ACCEL_VAL_HIGHRES);
  return cfg;
}

SensorCollectionSubscription SensorCollection::subscribeAll(
  function<void(const AccelSample&)> callback
) {
  vector<int> subIds;
  for (auto& sensor: sensors) {
    subIds.emplace_back(sensor->publisher()->subscribe(callback));
  }
  return SensorCollectionSubscription {
    subIds
  };
}

void SensorCollection::unsubscribeAll(SensorCollectionSubscription subscription) {
  for (auto pair: boost::combine(sensors, subscription.subIds)) {
    unique_ptr<Sensor>& sensor = get<0>(pair);
    int subId = get<1>(pair);

    sensor->publisher()->unsubscribe(subId);
  }
}
