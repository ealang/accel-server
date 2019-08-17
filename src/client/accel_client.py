import logging
import grpc

import accel_pb2
import accel_pb2_grpc

def run():
    with grpc.insecure_channel('localhost:50051') as channel:
        stub = accel_pb2_grpc.AccelStub(channel)
        print('Server config: ' + str(
            stub.GetConfig(accel_pb2.GetConfigRequest()).config
        ))

        for msg in stub.Subscribe(accel_pb2.SubscribeRequest()):
            print(list(msg.samples))

if __name__ == '__main__':
    logging.basicConfig()
    run()
