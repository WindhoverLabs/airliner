import argparse
import math
import os.path

import requests
import yaml
import csv

import pandas as pd
import datetime as dt


def read_yaml(yaml_file: str) -> dict:
    yaml_data = yaml.load(open(yaml_file, 'r'),
                          Loader=yaml.FullLoader)
    return yaml_data


def rcv_csv_from_yamcs(yaml_dict):
    uri = yaml_dict['uri'] + yaml_dict['instance'] + ":" + yaml_dict['method']
    param_dict = {'parameters': yaml_dict['param_array'],
                  'start': yaml_dict['start'], 'stop': yaml_dict['stop'],
                  'delimiter': 'COMMA'}
    s = requests.session()

    with s.get(uri, params=param_dict, stream=True) as resp:
        with open(yaml_dict['yamcs_csv_file'], 'wb') as csvfile:
            for line in resp.iter_lines():
                if line:
                    csvfile.write(line + b'\n')


def write_csv(
        yaml_dict,
        time_col,
        epoch_ms_delta_col,
        param_dict,
        param_headers):
    csv_header = ['Time', 'delta_time_ms']
    for param_header in param_headers:
        csv_header.append(param_header)
        param_cnt_header = param_header + 'cnt'
        csv_header.append(param_cnt_header)

    csv_data = []
    for i in range(len(time_col)):
        csv_row = []
        csv_row.append(time_col[i])
        csv_row.append(epoch_ms_delta_col[i])
        for param_header in param_headers:
            csv_row.append(param_dict[param_header]['col'][i])
            csv_row.append(param_dict[param_header]['cnt'][i])
        csv_data.append(csv_row)
        print('done with ', i, ' of ', len(time_col))

    with open(yaml_dict['output_csv_file'],
              'w', encoding='UTF8', newline='') as f:
        writer = csv.writer(f)

        # write the header
        writer.writerow(csv_header)

        # write multiple rows
        writer.writerows(csv_data)


def parse_csv(yaml_dict):
    csv_data = pd.read_csv(yaml_dict['yamcs_csv_file'])
    time_col = csv_data['Time']
    epoch_ms_delta_col = []
    param_headers = []
    time_cell_count = 0
    dt_zero = 0
    param_dict = {}
    for param_path in yaml_dict['param_array']:
        param_basename = os.path.basename(param_path)
        param_headers.append(param_basename)
        param_dict[param_basename] = {}
        prev_cnt = 0
        prev_val = 0
        param_dict[param_basename]['cnt'] = []
        param_dict[param_basename]['col'] = []
        for csv_cell in csv_data[param_basename]:
            if (csv_cell != '') and (csv_cell != prev_val) and not(pd.isna(csv_cell)):
                prev_cnt += 1
                param_dict[param_basename]['cnt'].append(prev_cnt)
                param_dict[param_basename]['col'].append(csv_cell)
                prev_val = csv_cell
            else:
                param_dict[param_basename]['col'].append(prev_val)
                param_dict[param_basename]['cnt'].append(prev_cnt)

    for time_cell in time_col:
        dtime = dt.datetime.strptime(time_cell, "%Y-%m-%dT%H:%M:%S.%fZ")
        if time_cell_count == 0:
            dt_zero = dtime
        dt_delta_ms = (dtime - dt_zero).total_seconds()*1000
        epoch_ms_delta_col.append(dt_delta_ms)

        time_cell_count += 1

    write_csv(yaml_dict,
              time_col,
              epoch_ms_delta_col,
              param_dict,
              param_headers)


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

    print('Yamcs Params to CSV Successful.')


main()
