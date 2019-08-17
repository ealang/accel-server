load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "com_github_grpc_grpc",
  urls = [
    "https://github.com/grpc/grpc/archive/0beb353f0a21024659bedfb3e816a4f99eff41e8.tar.gz",
  ],
  strip_prefix = "grpc-0beb353f0a21024659bedfb3e816a4f99eff41e8",
  sha256 = "c25540cd581ada8d8934c7ce74d7ec0be8508a8af95c129e1b69f3fb7c208bdc"
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()
