#include <glog/logging.h>
#include "src/server/service/accel_service_impl.hh"
#include "src/server/service/subscription_state.hh"

using namespace accel;
using namespace grpc;

AccelServiceImpl::AccelServiceImpl(
  SensorPublisherRef sensorPublisher,
  SensorConfig config
): sensorPublisher(sensorPublisher), config(config) {}

Status AccelServiceImpl::GetConfig(
    ServerContext* context,
    const GetConfigRequest* request,
    GetConfigReply* reply
) {
  reply->mutable_config()->CopyFrom(config);
  return Status::OK;
}

Status AccelServiceImpl::Subscribe(
    ServerContext* context,
    const SubscribeRequest* request,
    ServerWriter<SubscribeReply> *writer
) {

  SubscriptionState sub(
    config.sensor_id_to_name_size(),
    request->parameters(),
    sensorPublisher
  );
  DLOG(INFO) << "New subscriber client";

  while (!context->IsCancelled()) {
    SubscribeReply reply;
    for (const auto& srcSample: sub.wait()) {
      AccelSample* dstSample = reply.add_samples();
      dstSample->CopyFrom(srcSample);
    }

    writer->Write(reply);
  }

  DLOG(INFO) << "Client unsubscribed";
  return Status::OK;
}
