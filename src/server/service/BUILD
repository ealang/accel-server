cc_library(
  name = "service",
  srcs = [
    "accel_service_impl.cc",
    "accel_service_impl.hh",
    "subscription_state.cc",
    "subscription_state.hh",
  ],
  deps = [
    "//src/protos:accel_grpc_lib",
    "//src/server/sensor:sensor",
    "@com_github_grpc_grpc//:grpc++",
  ],
  visibility = [
    "//src/server:__subpackages__",
  ]
)
