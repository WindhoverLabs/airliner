"""
Usage:
For obc-like builds:
python3 yaml_path_merger.py --yaml_output final_yaml_ppd.yaml --yaml_input merged_yaml.yaml --yaml_path /modules/cpd
python3 yaml_path_merger.py --yaml_output final_yaml_ppd.yaml --yaml_input merged_yaml.yaml --yaml_path /modules/ppd

For bebop-like builds:
Notice how the path is omitted. These configurations tend to be a lot simpler.

python3 yaml_path_merger.py --yaml_output final_yaml_ppd.yaml --yaml_input merged_yaml.yaml
"""
import argparse
import yaml
import yaml_merger
import os

ROOT_PATH = "/"


def parse_cli() -> argparse.Namespace:
    """
    Parses cli argyments.
    :return: The namespace that has all of the arguments that have been parsed.
    """
    parser = argparse.ArgumentParser(description='Takes in paths to yaml file and sqlite database.')
    parser.add_argument('--yaml_output', type=str,
                        help='The output yaml file that has everything merged in.',
                        required=True)
    parser.add_argument('--yaml_input', type=str,
                        help='The input file that will be merged into the output file at yaml_path.', required=True)

    parser.add_argument('--yaml_path', type=str,
                        help='The path to the node in the output file which to merge the contents into',
                        default=ROOT_PATH)

    return parser.parse_args()


def add_value_to_dict(yaml_dict: dict, value: object):
    for key in yaml_dict:
        if type(yaml_dict[key]) == dict and len(yaml_dict[key]) > 0:
            add_value_to_dict(yaml_dict[key], value)
        else:
            yaml_dict[key] = value


def add_dict(path_node: str, root_dict: dict):
    root_dict[path_node] = dict()


def make_dict_from_path(paths):
    """
    Adds an empty nested dictionary from the path.
    """
    root_dict = dict()
    dict_partition = root_dict
    for path in paths:
        add_dict(path, dict_partition)
        dict_partition = dict_partition[path]

    return root_dict


# NOTE: Could actually be useful.
def get_dict_at_path(paths, root_dict):
    dict_partition = root_dict
    for path in paths:
        dict_partition = dict_partition[path]

    return dict_partition


def add_new_module(yaml_path, yaml_input):
    new_node = make_dict_from_path(yaml_path.split(ROOT_PATH)[1:])
    with open(yaml_input, 'r+') as outFile:
        yaml_dict = yaml.load(outFile, Loader=yaml.FullLoader)
        add_value_to_dict(new_node, yaml_dict)

    return new_node


def main():
    # FIXME: Could definitely use some cleanup
    args = parse_cli()
    # Create the file in case it does not exist
    if os.path.exists(args.yaml_output) is False:
        f = open(args.yaml_output, '+w')
        f.close()

    if args.yaml_path != ROOT_PATH:
        new_module_dict = add_new_module(args.yaml_path, args.yaml_input)

        with open(args.yaml_output, 'r+') as output_file:
            yaml_dict = yaml.load(output_file, Loader=yaml.FullLoader)

            # If our output YAML has content, merge th new one onto it
            if yaml_dict is not None:
                yaml_merger.merge(new_module_dict, yaml_dict)

                # I don't like doing this, but yaml.dump does not overwrite the contents of the file.
                output_file.seek(0)
                output_file.truncate()

                yaml_merger.merge(new_module_dict, yaml_dict)
                yaml.dump(yaml_dict, output_file)

            else:
                with open(args.yaml_output, 'r+') as outFile:
                    yaml.dump(new_module_dict, outFile)

    # If all we want is to add it to the root, then it is really simple.
    else:
        with open(args.yaml_input, 'r+') as input_file:
            yaml_dict_input = yaml.load(input_file, Loader=yaml.FullLoader)

        with open(args.yaml_output, 'r+') as output_file:
            yaml.dump(yaml_dict_input, output_file)


if __name__ == '__main__':
    main()
