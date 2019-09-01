import json
import argparse
from functools import partial
from google.protobuf.json_format import MessageToDict

from .accel_client import AccelClient


msg_to_dict = partial(
    MessageToDict,
    preserving_proto_field_name=True,
    including_default_value_fields=True
)


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
    parser.add_argument(
        '--dump-config',
        action='store_true',
        help='Dump server config and exit'
    )
    parser.add_argument(
        '--batch-size',
        default=1,
        type=int,
        help='Num samples required per sensor before data is transmitted'
    )

    args = parser.parse_args()
    client = AccelClient(
        addr=f'{args.host}:{args.port}'
    )

    if args.dump_config:
        print(json.dumps(msg_to_dict(client.config())))
    else:
        for batch in client.subscribe(args.batch_size):
            print(json.dumps([
                msg_to_dict(sample) for sample in batch
            ]))


if __name__ == '__main__':
    main()
