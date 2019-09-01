import json
import argparse
import os
import sys
from functools import partial
from google.protobuf.json_format import MessageToDict

from .accel_client import AccelClient


msg_to_dict = partial(
    MessageToDict,
    preserving_proto_field_name=True,
    including_default_value_fields=True
)


def main_show_config(client):
    print(json.dumps(msg_to_dict(client.config())))


def main_stream(args, client):
    history = []
    try:
        for batch in client.subscribe(args.batch_size):
            for sample in batch:
                json_sample = msg_to_dict(sample)
                print(json.dumps(json_sample))
                if args.dump is not None:
                    history.append(json_sample)
    except KeyboardInterrupt:
        if args.dump is not None:
            with open(args.dump, 'w') as fp:
                json.dump(
                    {
                        'config': msg_to_dict(client.config()),
                        'samples': history
                    },
                    fp,
                    indent=2
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
        '--show-config',
        action='store_true',
        help='Dump server config and exit'
    )
    parser.add_argument(
        '--dump',
        type=str,
        help='Dump samples to given path'
    )
    parser.add_argument(
        '--batch-size',
        default=1,
        type=int,
        help='Num samples required per sensor before data is transmitted'
    )

    args = parser.parse_args()
    if args.dump is not None and os.path.exists(args.dump):
        print(f'{args.dump} already exists')
        sys.exit(1)

    client = AccelClient(
        addr=f'{args.host}:{args.port}'
    )

    if args.show_config:
        main_show_config(client)
    else:
        main_stream(args, client)


if __name__ == '__main__':
    main()
