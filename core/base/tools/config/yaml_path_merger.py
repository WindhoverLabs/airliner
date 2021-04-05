"""

"""
import argparse
import subprocess
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

    parser.add_argument('--append', type=bool,
                        help='Append to the yaml_ouput file. Very useful when invoking obc-like builds that'
                             ' should be merged into a single file by invoking this tool multiple times',
                        default=False)

    return parser.parse_args()


def add_empty_dict(yaml_dict: dict):
    for key in yaml_dict:
        if type(yaml_dict[key]) == dict:
            add_empty_dict(yaml_dict[key])
        else:
            if yaml_dict[key] == 'none':
                yaml_dict[key] = {}


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


def main():
    # FIXME: Could definitely use some cleanup
    args = parse_cli()

    # Create the file in case it does not exist
    if os.path.exists(args.yaml_output) is False:
        f = open(args.yaml_output, '+w')
        f.close()

    if args.append is True:
        new_node = make_dict_from_path(args.yaml_path.split(ROOT_PATH)[1:])
        with open(args.yaml_output, 'r+') as outFile:
            yaml_dict = yaml.load(outFile, Loader=yaml.FullLoader)
        yaml_merger.merge(new_node, yaml_dict)

        subprocess.run(["yaml-merge", "-m", args.yaml_path, '-w', args.yaml_output, args.yaml_output,
                        args.yaml_input])

    else:
        if args.yaml_path != ROOT_PATH:
            subprocess.run(["yaml-set", f"--change={args.yaml_path}", "--value=none", args.yaml_output])

    with open(args.yaml_output, 'r+') as outFile:
        yaml_dict = yaml.load(outFile, Loader=yaml.FullLoader)

        if args.append is False:
            # I have to do this because yaml-paths tools do not write an empty dictionary to the file. It stringifies it,
            # so then when merging it won't actually merge it at the right path.
            outFile.seek(0)
            outFile.truncate()

            # If we only have root, we don't have to do anything here.
            if args.yaml_path != ROOT_PATH:
                add_empty_dict(yaml_dict)

            yaml.dump(yaml_dict, outFile)

    subprocess.run(["yaml-merge", "-m", args.yaml_path, '-w', args.yaml_output, args.yaml_output,
                    args.yaml_input])


if __name__ == '__main__':
    main()
