#pragma once

#include <grpcpp/grpcpp.h>
#include "src/protos/accel.grpc.pb.h"
#include "src/server/sensor/sensor_publisher_ref.hh"

class AccelServiceImpl final: public accel::Accel::Service {
  SensorPublisherRef sensorPublisher;
  const accel::SensorConfig config;
public:

  AccelServiceImpl(
    SensorPublisherRef sensorPublisher,
    accel::SensorConfig config
  );

  grpc::Status GetConfig(
      grpc::ServerContext* context,
      const accel::GetConfigRequest* request,
      accel::GetConfigReply* reply
  ) override;

  grpc::Status Subscribe(
      grpc::ServerContext* context,
      const accel::SubscribeRequest* request,
      grpc::ServerWriter<accel::SubscribeReply> *writer
  ) override;
};
