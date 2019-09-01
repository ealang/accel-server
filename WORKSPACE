load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")

# GRPC dependencies
http_archive(
  name = "com_github_grpc_grpc",
  urls = [
    "https://github.com/grpc/grpc/archive/0beb353f0a21024659bedfb3e816a4f99eff41e8.tar.gz",
  ],
  strip_prefix = "grpc-0beb353f0a21024659bedfb3e816a4f99eff41e8",
  sha256 = "c25540cd581ada8d8934c7ce74d7ec0be8508a8af95c129e1b69f3fb7c208bdc",
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

# glog dependencies
http_archive(
  name = "com_github_google_glog",
  urls = [
    "https://github.com/google/glog/archive/v0.4.0.tar.gz",
  ],
  strip_prefix = "glog-0.4.0",
  sha256 = "f28359aeba12f30d73d9e4711ef356dc842886968112162bc73002645139c39c",
)

# json parsing library
http_file(
  name = "com_github_nlohmann_json_single_header",
  downloaded_file_path="json.hpp",
  urls = [
    "https://github.com/nlohmann/json/releases/download/v3.7.0/json.hpp",
  ],
  sha256 = "a503214947952b69f0062f572cb74c17582a495767446347ce2e452963fc2ca4",
)
