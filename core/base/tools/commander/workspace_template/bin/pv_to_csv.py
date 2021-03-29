
"""
Gather all PV names from YAMCS server and dump them on a CSV file.
"""
import csv
import requests

INSTANCE = "yamcs-cfs"

all_pvalues = f"http://localhost:8090/api/mdb/{INSTANCE}/parameters"

# Could be useful, though beware that this returns not just PValue names but also the actual value as well.
pvalues_to_csv = f"http://localhost:8090/api/archive/{INSTANCE}:exportParameterValues"

output_file = "PValues.csv"

MEMBER_SEPARATOR = "."


def get_aggregate_qualified_names(parameter, param_qualified_name, out_names: [str]):
    for member in parameter['type']['member']:
        if member['type']['engType'] == 'aggregate':
            get_aggregate_qualified_names(member, param_qualified_name + MEMBER_SEPARATOR + member['name'], out_names)
        else:
            out_names.append(param_qualified_name + MEMBER_SEPARATOR + member['name'])


def main():
    response = requests.get(all_pvalues)
    parameters = response.json()['parameters']

    for p in parameters:
        # Avoid params that are not part of XTCE such YAMCS internal command counters
        if 'type' in p:
            if p['type']['engType'] == 'aggregate' and 'member' in p['type']:
                out_list = []
                get_aggregate_qualified_names(p, p['qualifiedName'], out_list)
                with open(output_file, 'a+') as csvfile:
                    writer = csv.writer(csvfile, delimiter=",", dialect='excel')
                    writer.writerows((pv,) for pv in out_list)


if __name__ == '__main__':
    main()
