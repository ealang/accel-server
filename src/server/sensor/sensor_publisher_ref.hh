#pragma once

#include <memory>
#include "src/server/sensor/publisher.hh"
#include "src/protos/accel.pb.h"

using SensorPublisherRef = std::shared_ptr<Publisher<const accel::AccelSample&>>;
