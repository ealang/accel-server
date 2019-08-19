# Accel Server

## Overview

Project to stream LIS3DH accelerometer data from a raspi.

## Components

* Embedded GRPC server
* Python GRPC client
* User space LIS3DH driver

## Building

The build system for this project is Bazel. It has been tested with Bazel 0.28.

Build the server:

```shell
bazel build //src/server:server
```

Build a server with a fake driver:

```shell
bazel build //src/server:fakeserver
```

## TODO

- Implement real driver
- Max buffer size for subscription
- Stop polling if there are no clients
