cc_library(
  name = "sensor",
  srcs = [
    "publisher.hh",
    "sensor_thread.cc",
    "util.cc",
    "util.hh",
  ],
  hdrs = [
    "sensor_thread.hh",
    "sensor_publisher_ref.hh",
  ],
  deps = [
    "//src/protos:accel_grpc_lib",
    "//src/lis3dh-driver-ifc:lis3dh-driver-ifc",
    "@com_github_google_glog//:glog",
  ],
  visibility = [
    "//src/server:__subpackages__",
  ]
)
