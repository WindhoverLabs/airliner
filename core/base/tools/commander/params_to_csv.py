import argparse
import requests
import yaml


def read_yaml(yaml_file: str) -> dict:
    yaml_data = yaml.load(open(yaml_file, 'r'),
                          Loader=yaml.FullLoader)
    return yaml_data


def yamcs_rcv(yaml_dict):
    uri = yaml_dict['uri'] + yaml_dict['instance'] + ":" + yaml_dict['method']
    param_dict = {'parameters': yaml_dict['param_array'], 'start': yaml_dict['start'], 'stop': yaml_dict['stop'],
                  'delimiter': 'COMMA'}
    s = requests.session()

    with s.get(uri, params=param_dict, stream=True) as resp:
        with open(yaml_dict['output_csv_file'], 'wb') as csvfile:
            for line in resp.iter_lines():
                if line:
                    csvfile.write(line + b'\n')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--file", help="yaml configuration file",
                        type=str)

    args = parser.parse_args()
    print(args.file)

    yaml_dict = read_yaml(args.file)
    print(yaml_dict)

    yamcs_rcv(yaml_dict)


main()
