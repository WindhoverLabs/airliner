import argparse
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
        delta_time_ms,
        param_dict,
        param_headers):
    csv_header = ['Time', 'delta_time_ms']
    for param_header in param_headers:
        csv_header.append(param_header)
        param_cnt_header = param_header + 'cnt'
        csv_header.append(param_cnt_header)

    csv_data = []
    for i in range(len(time_col)):
        csv_row = [time_col[i], delta_time_ms[i]]
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
    new_time_col = []
    delta_time_ms = []
    time_cell_count = 0
    write_time_cell = ''
    duplicate_time_cell_indexes = []
    time_duplicates = []
    dt_zero = 0
    dt_delta_ms = 0
    for time_cell in time_col:
        dtime = dt.datetime.strptime(time_cell, "%Y-%m-%dT%H:%M:%S.%fZ")
        if time_cell_count == 0:
            # this is the first time_col entry
            write_time_cell = time_cell
            dt_zero = dtime
        else:
            if time_cell != write_time_cell:
                # This is a new time entry
                new_time_col.append(write_time_cell)
                delta_time_ms.append(dt_delta_ms)
                time_duplicates.append(duplicate_time_cell_indexes)
                duplicate_time_cell_indexes = []

                # Write the new time entry
                write_time_cell = time_cell
                dt_delta_ms = (dtime - dt_zero).total_seconds() * 1000

        duplicate_time_cell_indexes.append(time_cell_count)
        time_cell_count += 1

    print('Done with Time')

    param_headers = []
    param_dict = {}
    k = 0
    for param_path in yaml_dict['param_array']:
        param_basename = os.path.basename(param_path)
        print(param_basename)
        param_headers.append(param_basename)
        param_dict[param_basename] = {}
        prev_cnt = 0
        prev_val = 0
        param_dict[param_basename]['cnt'] = []
        param_dict[param_basename]['col'] = []

        for i in range(len(new_time_col)):
            for j in time_duplicates[i]:
                if (csv_data[param_basename][j] != '') \
                        and not (pd.isna(csv_data[param_basename][j])) \
                        and csv_data[param_basename][j] != prev_val:
                    prev_cnt += 1
                    prev_val = csv_data[param_basename][j]
            param_dict[param_basename]['cnt'].append(prev_cnt)
            param_dict[param_basename]['col'].append(prev_val)

        print('params done with ', k, ' of ', len(yaml_dict['param_array']))
        k += 1

    write_csv(yaml_dict,
              new_time_col,
              delta_time_ms,
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
