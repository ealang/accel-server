#include <glog/logging.h>
#include "src/server/sensor/sensor_collection.hh"
#include "src/server/service/accel_service_impl.hh"
#include "src/server/service/subscription_state.hh"

using namespace accel;
using namespace grpc;
using namespace std;

AccelServiceImpl::AccelServiceImpl(
  SensorCollection& sensors
): sensors(sensors) {}

Status AccelServiceImpl::GetConfig(
    ServerContext* context,
    const GetConfigRequest* request,
    GetConfigReply* reply
) {
  reply->mutable_config()->CopyFrom(
    sensors.sensorConfig()
  );
  return Status::OK;
}

Status AccelServiceImpl::Subscribe(
    ServerContext* context,
    const SubscribeRequest* request,
    ServerWriter<SubscribeReply> *writer
) {

  SubscriptionState sub(request->parameters(), sensors);
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
