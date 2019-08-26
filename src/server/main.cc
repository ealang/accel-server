#include <memory>
#include <string>
#include <glog/logging.h>
#include "src/lis3dh-driver-ifc/lis3dh-spi.hh"
#include "src/lis3dh-driver-ifc/lis3dh-spi-dev.hh"
#include "src/server/sensor/sensor_thread.hh"
#include "src/server/service/accel_service_impl.hh"

using namespace grpc;
using namespace std;
using namespace std;

struct HardwareConfig {
  string sensorName;
  uint32_t selectPin;
  uint32_t sensorId;
};

// TODO: make this runtime configurable
HardwareConfig sensorDefinitions[2] = {
  { "sensor1", 8, 100 },
  { "sensor2", 9, 101 }
};

// Initialize sensors and SPI device
int hardwareInit(uint32_t sampleRateHz) {
  initializeDriver();

  for (const auto& sensor: sensorDefinitions) {
    initializeChipSelectPin(sensor.selectPin);
  }

  uint32_t maxSpeedHz = 20'000'000;
  int spiDevice = openSpiDeviceForLis3dh("/dev/spidev0.0", maxSpeedHz);

  for (const auto& sensor: sensorDefinitions) {
    lis3dhSelfCheck(spiDevice, sensor.selectPin);
    lis3dhInitialize(
      spiDevice,
      sensor.selectPin,
      sampleRateFlag(sampleRateHz)
    );
  }
  return spiDevice;
}

// Create user facing config object
accel::SensorConfig makeSensorConfig(uint32_t sampleRateHz) {
  accel::SensorConfig cfg;
  cfg.set_sample_rate_hz(sampleRateHz);

  for (const auto& sensor: sensorDefinitions) {
    (*cfg.mutable_sensor_id_to_name())[sensor.sensorId] = sensor.sensorName;
  }
  return cfg;
}

// Initialize sensor poll thread
unique_ptr<SensorThread> makeSensorPollThread(int spiDevice, uint32_t sampleRateHz) {
  unordered_map<uint32_t, uint32_t> sensorIdToSelectPin;
  for (const auto& sensor: sensorDefinitions) {
    sensorIdToSelectPin[sensor.sensorId] = sensor.selectPin;
  };

  return make_unique<SensorThread>(
    spiDevice,
    sampleRateHz,
    sensorIdToSelectPin
  );
}

int main(int argc, char** argv) {
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  uint32_t sampleRateHz = 25;
  int spiDevice = hardwareInit(sampleRateHz);
  accel::SensorConfig cfg = makeSensorConfig(sampleRateHz);
  unique_ptr<SensorThread> sensorThread = makeSensorPollThread(spiDevice, sampleRateHz);

  AccelServiceImpl service(sensorThread->publisher(), cfg);
  string server_address("0.0.0.0:50051");

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  unique_ptr<Server> server(builder.BuildAndStart());
  LOG(INFO) << "Server listening on " << server_address;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();

  closeSpiDevice(spiDevice);

  return 0;
}
