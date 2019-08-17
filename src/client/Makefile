.PHONY: protos
protos:
	 python -m grpc_tools.protoc -I../protos --python_out=. --grpc_python_out=. ../protos/accel.proto

.PHONY: clean
clean:
	rm -f accel_pb2.py
	rm -f accel_pb2_grpc.py
