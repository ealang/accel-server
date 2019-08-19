cc_library(
  name = "lis3dh-fakedriver",
  srcs = [
    "lis3dh-spi-dev.cc",
    "lis3dh-spi.cc",
  ],
  deps = [
    "//src/lis3dh-driver-ifc:lis3dh-driver-ifc",
    "@com_github_google_glog//:glog",
  ],
  visibility = [
    "//src:__subpackages__",
  ]
)
