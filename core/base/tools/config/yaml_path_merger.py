import argparse


def parse_cli() -> argparse.Namespace:
    """
    Parses cli argyments.
    :return: The namespace that has all of the arguments that have been parsed.
    """
    parser = argparse.ArgumentParser(description='Takes in paths to yaml file and sqlite database.')
    parser.add_argument('--yaml_path', type=str,
                        help='The file path to the YAML file which contains telemetry and command metadata.',
                        required=True)
    parser.add_argument('--sqlite_path', type=str,
                        help='The file path to the sqlite database', required=True)

    return parser.parse_args()

def main():
    pass

if __name__ == '__main__':
    main()