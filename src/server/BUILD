cc_binary(
    name = "fakeserver",
    srcs = [
      "main.cc",
    ],
    deps = [
        "//src/server/service:service",
        "//src/lis3dh-fakedriver:lis3dh-fakedriver",
        "@com_github_google_glog//:glog",
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
