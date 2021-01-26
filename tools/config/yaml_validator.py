import cerberus
import yaml
import argparse


def parse_cli() -> argparse.Namespace:
    """
    Parses cli arguments.
    :return: The namespace that has all of the arguments that have been parsed.
    """
    parser = argparse.ArgumentParser(description='Takes in path to sqlite database.')

    parser.add_argument('--yaml_document', type=str, required=True,
                        help='The document which to validate.')
    parser.add_argument('--yaml_schema', type=str, required=True,
                        help='The yaml schema which will be used to validate the document.')

    return parser.parse_args()


def read_yaml(yaml_file: str) -> dict:
    yaml_data = yaml.load(open(yaml_file, 'r'),
                          Loader=yaml.FullLoader)
    return yaml_data


def val(yaml_schema: str, yaml_document: str):
    schema = read_yaml(yaml_schema)
    document = read_yaml(yaml_document)

    V = cerberus.Validator(schema)
    is_valid = V.validate(document)

    if is_valid:
        print('The document %s has passed validation with no errors.',yaml_document)
    else:
        print('The document %s has failed validation because of the following errors: \n%s',
              yaml_document, V.errors)
    return is_valid


def main():
    args = parse_cli()
    val(args.yaml_schema, args.yaml_document)


if __name__ == '__main__':
    main()