import grpc
from typing import Iterable, List

import accel_pb2
import accel_pb2_grpc


class AccelClient:
    def __init__(self, addr: str):
        channel = grpc.insecure_channel(addr)
        self._stub = accel_pb2_grpc.AccelStub(channel)

    def config(self) -> accel_pb2.SensorConfig:
        return self._stub.GetConfig(accel_pb2.GetConfigRequest()).config

    def subscribe(self, batch_size: int = 1) -> Iterable[List[accel_pb2.AccelSample]]:
        """
        :param batch_size: Num samples required per sensor before data is transmitted
        """
        parameters = accel_pb2.SubscriptionParameters(
            per_sensor_batch_size=batch_size,
        )
        for msg in self._stub.Subscribe(accel_pb2.SubscribeRequest(parameters=parameters)):
            yield list(msg.samples)
