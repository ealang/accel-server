#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include <glog/logging.h>
#include <nlohmann/json.hpp>

#include "src/server/arg_parse.hh"
#include "src/lis3dh-driver-ifc/lis3dh-spi.hh"
#include "src/lis3dh-driver-ifc/lis3dh-spi-dev.hh"
#include "src/server/sensor/sensor_thread.hh"
#include "src/server/service/accel_service_impl.hh"

using namespace grpc;
using namespace std;
using namespace std;

// Per-sensor config
struct SensorConfig {
  string name;
  uint32_t selectPin;
  uint32_t sensorId;
};

// Global app config
struct AppConfig {
  vector<SensorConfig> sensors;
  // sample rate for all sensors
  uint32_t sampleRateHz;
  uint32_t spiBusRateMhz;
  string spiDevice;
};

// Initialize sensors and SPI device
int hardwareInit(const AppConfig& config) {
  initializeDriver();

  for (const SensorConfig& sensor: config.sensors) {
    initializeChipSelectPin(sensor.selectPin);
  }

  int spiDevice = openSpiDeviceForLis3dh(
    config.spiDevice,
    config.spiBusRateMhz * 1'000'000
  );

  for (const SensorConfig& sensor: config.sensors) {
    lis3dhSelfCheck(spiDevice, sensor.selectPin);
    lis3dhInitialize(
      spiDevice,
      sensor.selectPin,
      sampleRateFlag(config.sampleRateHz)
    );
  }
  return spiDevice;
}

// Create user facing config object
accel::SensorConfig makeSensorConfig(const AppConfig& config) {
  accel::SensorConfig cfg;
  cfg.set_sample_rate_hz(config.sampleRateHz);

  for (const SensorConfig& sensor: config.sensors) {
    (*cfg.mutable_sensor_id_to_name())[sensor.sensorId] = sensor.name;
  }
  return cfg;
}

// Initialize sensor poll thread
unique_ptr<SensorThread> makeSensorPollThread(int spiDevice, const AppConfig& config) {
  unordered_map<uint32_t, uint32_t> sensorIdToSelectPin;
  for (const auto& sensor: config.sensors) {
    sensorIdToSelectPin[sensor.sensorId] = sensor.selectPin;
  };

  return make_unique<SensorThread>(
    spiDevice,
    config.sampleRateHz,
    sensorIdToSelectPin
  );
}

void showHelp() {
  CommandLineInput defaultOpts;
  cout
    << "Accel Server options:" << endl
    << " --help            Display this message" << endl
    << " --config <path>   Path to config file (default " << defaultOpts.configPath << ")" << endl;
}

// Given path to json return AppConfig
AppConfig parseConfig(string jsonPath) {
  ifstream fp(jsonPath);
  stringstream buffer;
  buffer << fp.rdbuf();

  const auto json = nlohmann::json::parse(buffer);

  AppConfig appConfig;
  appConfig.sampleRateHz = json["sensor-sample-rate-hz"].get<uint32_t>();
  appConfig.spiBusRateMhz = json["spi-bus-rate-mhz"].get<uint32_t>();
  appConfig.spiDevice = json["spi-device"].get<string>();
  for (const auto &sensorJson: json["sensors"]) {
    SensorConfig sensorConfig;
    appConfig.sensors.emplace_back(SensorConfig {
      .name = sensorJson["name"].get<string>(),
      .selectPin = sensorJson["select-pin"].get<uint32_t>(),
      .sensorId = sensorJson["sensor-id"].get<uint32_t>(),
    });
  }

  return appConfig;
}

void runServer(const AppConfig& config) {
  int spiDevice = hardwareInit(config);
  accel::SensorConfig cfg = makeSensorConfig(config);
  unique_ptr<SensorThread> sensorThread = makeSensorPollThread(spiDevice, config);

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
}

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();

  CommandLineInput input = parseArgs(argc, argv);
  if (input.showHelp) {
    showHelp();
    return 0;
  }

  AppConfig config = parseConfig(input.configPath);
  runServer(config);

  return 0;
}
