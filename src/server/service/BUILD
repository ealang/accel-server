cc_library(
  name = "service",
  srcs = [
    "accel_service_impl.cc",
    "subscription_state.cc",
    "subscription_state.hh",
  ],
  hdrs = [
    "accel_service_impl.hh",
  ],
  deps = [
    "//src/protos:accel_grpc_lib",
    "//src/server/sensor:sensor",
    "@com_github_grpc_grpc//:grpc++",
    "@com_github_google_glog//:glog",
  ],
  visibility = [
    "//src/server:__subpackages__",
  ]
)
