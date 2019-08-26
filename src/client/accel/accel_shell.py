import argparse
from .accel_client import AccelClient


def main():
    parser = argparse.ArgumentParser(description='Stream accel data')
    parser.add_argument(
        '--host',
        default='127.0.0.1',
        help='Server hostname'
    )
    parser.add_argument(
        '--port',
        default=50051,
        type=int,
        help='Server port'
    )

    args = parser.parse_args()

    client = AccelClient(
        addr=f'{args.host}:{args.port}'
    )
    print(client.config())

    for sample in client.subscribe():
        print(sample)


if __name__ == '__main__':
    main()
