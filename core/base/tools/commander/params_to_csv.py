import argparse
import csv

import requests
import yaml

import pandas as pd
import datetime as dt


def read_yaml(yaml_file: str) -> dict:
    yaml_data = yaml.load(open(yaml_file, 'r'),
                          Loader=yaml.FullLoader)
    return yaml_data


def rcv_csv_from_yamcs(yaml_dict):
    uri = yaml_dict['uri'] + yaml_dict['instance'] + ":" + yaml_dict['method']
    param_dict = {'parameters': yaml_dict['param_array'], 'start': yaml_dict['start'], 'stop': yaml_dict['stop'],
                  'delimiter': 'COMMA'}
    s = requests.session()

    with s.get(uri, params=param_dict, stream=True) as resp:
        with open(yaml_dict['yamcs_csv_file'], 'wb') as csvfile:
            for line in resp.iter_lines():
                if line:
                    csvfile.write(line + b'\n')


def parse_csv(yaml_dict):
    csv_data = pd.read_csv(yaml_dict['yamcs_csv_file'])
    time_col = csv_data['Time']
    epoch_ms_delta_col = []
    time_cell_count = 0
    for time_cell in time_col:
        dtime = dt.datetime.strptime(time_cell, "%Y-%m-%dT%H:%M:%S.%fZ")
        if time_cell_count == 0:
            dt_zero = dtime
        dt_delta_ms = (dtime - dt_zero).total_seconds()*1000
        epoch_ms_delta_col.insert(time_cell_count, dt_delta_ms)

        time_cell_count += 1

    print(epoch_ms_delta_col)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", help="yaml configuration file",
                        type=str)

    args = parser.parse_args()
    print(args.file)

    yaml_dict = read_yaml(args.file)
    print(yaml_dict)

    rcv_csv_from_yamcs(yaml_dict)
    parse_csv(yaml_dict)


main()
