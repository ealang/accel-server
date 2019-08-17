#include <memory>
#include "src/server/sensor/sensor_collection.hh"
#include "src/server/sensor/sensor.hh"
#include "src/server/service/accel_service_impl.hh"

using namespace grpc;
using namespace std;

int main(int argc, char** argv) {
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

  return 0;
}
