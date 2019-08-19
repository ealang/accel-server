load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "protos",
    srcs = ["accel.proto"],
)

cc_proto_library(
    name = "accel_proto_lib",
    deps = ["//src/protos:protos"],
)

cc_grpc_library(
    name = "accel_grpc_lib",
    srcs = ["//src/protos:protos"],
    grpc_only = True,
    deps = [":accel_proto_lib"],
  visibility = ["//src:__subpackages__"]
)
