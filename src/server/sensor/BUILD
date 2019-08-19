cc_library(
  name = "sensor",
  srcs = [
    "config.hh",
    "publisher.hh",
    "sensor.cc",
    "sensor_collection.cc",
    "util.cc",
    "util.hh",
  ],
  hdrs = [
    "sensor_collection.hh",
    "sensor.hh",
  ],
  deps = [
    "//src/protos:accel_grpc_lib",
    "@com_github_google_glog//:glog",
  ],
  visibility = [
    "//src/server:__subpackages__",
  ]
)
