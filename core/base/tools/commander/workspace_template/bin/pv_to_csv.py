"""
Gather all PV names from YAMCS server and dump them on a CSV file.
"""
import csv
import requests
from yamcs.client import YamcsClient

INSTANCE = "viper"
SERVER = "http://192.168.2.96:8090"

all_pvalues = f"{SERVER}/api/mdb/{INSTANCE}/parameters"

# Could be useful, though beware that this returns not just PValue names but also the actual value as well.
pvalues_to_csv = f"{SERVER}/api/archive/{INSTANCE}:exportParameterValues"

output_file = "PValues.csv"

MEMBER_SEPARATOR = "."


def get_aggregate_qualified_names(parameter, param_qualified_name, out_names: [str]):
    if 'member' in parameter['type']:
        for member in parameter['type']['member']:
            if member['type']['engType'] == 'aggregate':
                get_aggregate_qualified_names(member, param_qualified_name + MEMBER_SEPARATOR + member['name'],
                                              out_names)
            else:
                out_names.append(param_qualified_name + MEMBER_SEPARATOR + member['name'])


def get_params_from_aggregates():
    response = requests.get(all_pvalues)
    parameters = response.json()['parameters']
    parameter_count = 0

    for p in parameters:
        # Avoid params that are not part of XTCE such YAMCS internal command counters
        if 'type' in p:
            if p['type']['engType'] == 'aggregate' and 'member' in p['type']:
                out_list = []
                get_aggregate_qualified_names(p, p['qualifiedName'], out_list)
                with open(output_file, 'a+') as csvfile:
                    parameter_count = len(out_list) + parameter_count
                    writer = csv.writer(csvfile, delimiter=",", dialect='excel')
                    writer.writerows((pv,) for pv in out_list)
            else:
                out_list = [p['qualifiedName']]
                with open(output_file, 'a+') as csvfile:
                    writer = csv.writer(csvfile, delimiter=",", dialect='excel')
                    parameter_count = len(out_list) + parameter_count
                    writer.writerows((pv,) for pv in out_list)

    print(f'Loaded {parameter_count} parameters from YAMCS')


def get_params_from_api():
    response = requests.get(pvalues_to_csv)
    with open(output_file, 'wb') as csvfile:
        csvfile.write(response.content)


def get_params_from_python_api():
    client = YamcsClient(SERVER[7:])
    mdb = client.get_mdb(instance=INSTANCE)
    parameter_count = 0
    for parameter in mdb.list_parameters():
        out_list = [parameter]
        with open(output_file, 'a+') as csvfile:
            writer = csv.writer(csvfile, delimiter=",", dialect='excel')
            parameter_count = len(out_list) + parameter_count
            writer.writerows((pv,) for pv in out_list)
    print(f'Loaded {parameter_count} parameters from YAMCS')

def main():
    get_params_from_python_api()


if __name__ == '__main__':
    main()
