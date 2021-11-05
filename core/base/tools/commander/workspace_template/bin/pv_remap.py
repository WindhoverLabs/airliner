"""
Usage:
python3 pv_remap.pv --csv_document /home/lgomez/Downloads/VIPER-to-Airliner_PV_Mapping_Sheet1.csv --old_col Airliner --new_col Viper --display_root_dir /home/lgomez/airliner/core/base/tools/commander/workspace_template/Displays
"""

import argparse
import csv
import os


def parse_cli() -> argparse.Namespace:
    """
    Parses cli arguments.
    :return: The namespace that has all of the arguments that have been parsed.
    """
    parser = argparse.ArgumentParser(description='Takes in path to sqlite database.')

    parser.add_argument('--csv_document', type=str, required=True,
                        help='The csv file to scan.')
    parser.add_argument('--old_col', type=str, required=False,
                        help='The old column that has old pvs which will be replaced.')
    parser.add_argument('--new_col', type=str, required=False,
                        help='The new column.')

    parser.add_argument('--display_root_dir', type=str, required=False,
                        help='The input directory to scan(recursively) for displays.')

    return parser.parse_args()


def replace_pv(old: str, new: str, display: str):
    return display.replace(old, new)


def remap_display(csv_document, old_col, new_col, in_display, out_display):
    rows = []
    old_display = ''
    with open(csv_document, 'r+') as csvfile:
        reader = csv.reader(csvfile, delimiter=",", dialect='excel')
        rows = list(reader)
    header = rows[0]
    old_index = 0
    new_index = 1
    if header[1] == old_col:
        old_index = 1
        new_index = 0
    with open(in_display, 'r') as old_display_file:
        old_display = old_display_file.read()
    with open(out_display, 'w+') as new_display_file:
        new_display = str(old_display)
        for row in rows[1:]:
            if row[old_index] != '':
                new_display = replace_pv(row[old_index], row[new_index], new_display)

        new_display_file.write(new_display)


def main():
    args = parse_cli()
    opi_files = []

    for (parent, subdirs, files) in os.walk(args.display_root_dir):
        for name in files + subdirs:
            filename, file_extension = os.path.splitext(os.path.join(parent, name))
            if file_extension == '.opi':
                opi_files.append(os.path.join(parent, name))

    for file in opi_files:
        remap_display(args.csv_document, args.old_col, args.new_col, file, file)


if __name__ == '__main__':
    main()
