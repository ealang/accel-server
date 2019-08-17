#pragma once

#include <grpcpp/grpcpp.h>
#include "src/protos/accel.grpc.pb.h"

class SensorCollection;

class AccelServiceImpl final: public accel::Accel::Service {
  SensorCollection& sensors;
public:

  AccelServiceImpl(
    SensorCollection& sensors
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
