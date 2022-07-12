import argparse
import requests
import urllib.parse

# from yamcs.client import YamcsClient


def yamcs_rcv(instance, p1, start, stop):
    # client = YamcsClient('localhost:8090')
    uri = 'http://localhost:8090/api/archive/' + instance + ":exportParameterValues"
    param_dict = {'parameters': p1, 'start': start, 'stop': stop, 'delimiter': 'COMMA'}
    uri_str = uri + "?" + urllib.parse.urlencode(param_dict)
    print(uri_str)

    print(uri)
    s = requests.session()

    # with s.get(uri, params=param_dict, stream=True) as resp:
    with s.get(uri_str, params=None, stream=True) as resp:
        for line in resp.iter_lines():
            if line:
                print(line)

    # client = YamcsClient(uri)
    # mdb = client.get_mdb(instance='simulator')
    # # ...
    # archive = client.get_archive(instance='simulator')
    # # ...
    # processor = client.get_processor(instance='simulator', processor='realtime')
    # ...


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--instance", help="yamcs instance",
                        type=str)

    parser.add_argument("--p1", help="yamcs param path",
                        type=str)

    parser.add_argument("--start", help="yamcs start time UTC",
                        type=str)

    parser.add_argument("--stop", help="yamcs stop time UTC",
                        type=str)

    args = parser.parse_args()
    print(args.instance)
    print(args.p1)
    print(args.start)
    print(args.stop)

    # yamcs_rcv()
    yamcs_rcv(args.instance, args.p1, args.start, args.stop)


main()
