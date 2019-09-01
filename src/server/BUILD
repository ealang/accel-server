cc_library(
  name = "nlohmann_json",
  hdrs = ["@com_github_nlohmann_json_single_header//file"],
  visibility = ["//visibility:public"],
  include_prefix = "nlohmann",
  strip_include_prefix = "/external/com_github_nlohmann_json_single_header/file",
)

cc_binary(
    name = "fakeserver",
    srcs = glob(["*.cc", "*.hh"]),
    deps = [
        "//src/server/service:service",
        "//src/lis3dh-fakedriver:lis3dh-fakedriver",
        "@com_github_google_glog//:glog",
        ":nlohmann_json",
    ],
)

cc_binary(
    name = "server",
    srcs = [
      "main.cc",
    ],
    deps = [
        "//src/server/service:service",
        "//src/lis3dh-driver:lis3dh-driver",
        "@com_github_google_glog//:glog",
    ],
    linkopts = [
      "-lwiringPi"
    ],
)
