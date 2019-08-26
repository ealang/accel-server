# Accel Server

## Overview

Project to stream LIS3DH accelerometer data from a raspi.

## Components

* Embedded GRPC server
* Python GRPC client
* User space LIS3DH driver

## Building

The build system for this project is Bazel. It has been tested with Bazel 0.28.1.

To compile on the raspi, wiringpi is required:

```
sudo apt-get install wiringpi
```

Build the server:

```shell
bazel build //src/server:server
```

Build a server with a fake driver (handy for off-target testing):

```shell
bazel build //src/server:fakeserver
```

## TODO

- Max buffer size for subscription
- Subscription framing options
- Stop polling if there are no clients
- Runtime configurable sensor settings
