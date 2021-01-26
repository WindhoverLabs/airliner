"""
A tool for merging yaml files. The source file always overwrites the destination file. The user may pass in a schema
to validate both files against.
"""

import argparse
import yaml
import yaml_validator
import logging


def merge(source: dict, destination: dict):
    """
    run me with nosetests --with-doctest file.py    
    >>> a = { 'first' : { 'all_rows' : { 'pass' : 'dog', 'number' : '1' } } }
    >>> b = { 'first' : { 'all_rows' : { 'fail' : 'cat', 'number' : '5' } } }
    >>> merge(b, a) == { 'first' : { 'all_rows' : { 'pass' : 'dog', 'fail' : 'cat', 'number' : '5' } } }
    True
    """
    for key, value in source.items():
        if isinstance(value, dict):
            # get node or create one
            node = destination.setdefault(key, dict())
            merge(value, node)
        else:
            destination[key] = value
    return destination


def read_yaml(yaml_file: str) -> dict:
    yaml_data = yaml.load(open(yaml_file, 'r'),
                          Loader=yaml.FullLoader)
    return yaml_data


def yaml_merge(source_file: str, destination_file: str) -> dict:
    """
    Merges two yaml files. The contents of destination_file are overwritten with the contents of destination_file.
    :param source_file: The path to the destination yaml file.
    :param destination_file: The path to the destination yaml file.
    :return: A dict which is the result of merging the two yaml files.
    """

    source_yaml = read_yaml(source_file)
    destination_yaml = read_yaml(destination_file)

    merge(source_yaml, destination_yaml)

    return destination_yaml


def write_yaml(yaml_dict: dict, out_file: str):
    """
    Writes the yaml_dict to a file with the name of out_file.
    :param yaml_dict: The dictionary data to be written to out_file.
    :param out_file:The file path to write yaml_dict to.
    :return:
    """
    f = open(out_file, 'w')

    yaml.dump(yaml_dict, f, sort_keys=False)


def parse_cli() -> argparse.Namespace:
    """
    Parses cli arguments.
    :return: The namespace that has all of the arguments that have been parsed.
    """
    parser = argparse.ArgumentParser(description='Takes two yaml files and a schema.')

    parser.add_argument('--source', type=str, required=True,
                        help=' The source yaml file. This always overwrites the destination.')
    parser.add_argument('--destination', type=str, required=True,
                        help=' The destination yaml file. This always overwrites the destination.')

    parser.add_argument('--output_file', type=str, default='merged_output.yaml',
                        help='The output file that will have the result of the source and destination yaml files '
                             'merged together.')

    parser.add_argument('--schema', type=str, default=None,
                        help='The schema the destination and source files adhere to. When this option is used,'
                             'the yaml files are validated against the specified schema.')

    return parser.parse_args()


def main():
    args = parse_cli()

    if args.schema:
        is_source_valid = yaml_validator.val(args.schema, args.source)

        if is_source_valid is True:
            is_destination_valid = yaml_validator.val(args.schema, args.destinaiton)

            if is_destination_valid is False:
                logging.info('There was a problem validating %s', args.destination)

        else:
            logging.info('There was a problem validating %s', args.source)

    final_yaml = yaml_merge(args.source, args.destination)

    write_yaml(final_yaml, args.output_file)


if __name__ == '__main__':
    main()

