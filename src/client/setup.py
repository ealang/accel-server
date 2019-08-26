from distutils.core import setup

setup(
    name='accel',
    version='1.0',
    packages=['accel'],
    py_modules=['accel_pb2', 'accel_pb2_grpc'],
    install_requires=[
        'grpcio',
        'grpcio-tools',
    ],
    entry_points={
        'console_scripts': [
            'accel = accel.accel_shell:main',
        ]
    },
)
