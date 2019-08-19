cc_library(
  name = "lis3dh-driver",
  srcs = [
    "lis3dh-spi-dev.cc",
    "lis3dh-spi.cc",
    "lis3dh-registers.hh",
  ],
  deps = [
    "//src/lis3dh-driver-ifc:lis3dh-driver-ifc",
    "@com_github_google_glog//:glog",
  ],
  visibility = [
    "//src:__subpackages__",
  ]
)
