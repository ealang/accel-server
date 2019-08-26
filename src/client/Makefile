.PHONY: all
all: sdist

.PHONY: protos
protos:
	python -m grpc_tools.protoc -I../protos --python_out=. --grpc_python_out=. ../protos/accel.proto

.PHONY: sdist
sdist: protos
	python setup.py sdist

.PHONY: clean
clean:
	rm -f accel_pb2.py accel_pb2_grpc.py MANIFEST
	rm -rf dist __pycache__
