#include <glog/logging.h>
#include <memory>
#include "src/server/sensor/sensor_collection.hh"
#include "src/server/sensor/sensor.hh"
#include "src/server/service/accel_service_impl.hh"
#include "src/lis3dh-driver-ifc/lis3dh-spi-dev.hh"

using namespace grpc;
using namespace std;

int main(int argc, char** argv) {
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  uint32_t maxSpeedHz = 10'1000'1000;
  int spiDevice = openSpiDeviceForLis3dh("/dev/spidev0.0", maxSpeedHz);

  string server_address("0.0.0.0:50051");

  unique_ptr<Sensor> sensor1 = make_unique<Sensor>(100);
  unique_ptr<Sensor> sensor2 = make_unique<Sensor>(200);
  SensorCollection sensors;
  sensors.addSensor(move(sensor1));
  sensors.addSensor(move(sensor2));

  AccelServiceImpl service(sensors);

  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  unique_ptr<Server> server(builder.BuildAndStart());
  cout << "Server listening on " << server_address << endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();

  closeSpiDevice(spiDevice);

  return 0;
}
