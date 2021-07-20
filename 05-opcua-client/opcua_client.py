# Copyright 2021 DPeshkoff && let-robots-reign;
# Distributed under the GNU General Public License, Version 3.0. (See
# accompanying file LICENSE)
######################################################################
## TECHNICAL INFORMATION                                            ##
## OPC UA Client                                                    ##
## Based on Python 3.9.1 64-bit                                     ##
######################################################################
# IMPORTS
from opcua import Client
from opcua.ua.uaerrors._auto import BadNodeIdUnknown
from influxdb import InfluxDBClient
from time import sleep

OPC_URL = 'opc.tcp://converter:4840'

INFLUX_HOST = 'influxdb'
INFLUX_PORT = 8086
INFLUX_DB_NAME = 'sensors'


def get_influx_client():
    try:
        client = InfluxDBClient(host=INFLUX_HOST, port=INFLUX_PORT, username='grafana', password='password')
        client.create_database(INFLUX_DB_NAME)
        client.switch_database(INFLUX_DB_NAME)
        return client
    except Exception as e:
        print(f'Couldn\'t connect to InfluxDB: {e}', flush=True)
        return None


def get_influx_message(vars):
    return [
        {
            'measurement': 'sensors',
            'fields': {
                var[0]: var[1]
            }
        }
        for var in vars
    ]


def get_opc_client():
    client = Client(OPC_URL)
    client.connect()
    return client


def get_all_variables(client):
    vars = []
    i = 2
    while True:
        try:
            node = client.get_node(f'ns=2;i={i}')
            vars.append((node.get_browse_name().Name, node.get_value()))
        except BadNodeIdUnknown:
            break
        i += 1
    return vars


def main():
    opc_client = get_opc_client()
    influx_client = get_influx_client()

    if opc_client is None or influx_client is None:
        print('Something wrong with OPC UA client or Influx client, exiting...', flush=True)
        return

    while True:
        vars = get_all_variables(opc_client)
        influx_client.write_points(get_influx_message(vars))
        for var in vars:
            print(*var, flush=True)

        sleep(10)


if __name__ == '__main__':
    sleep(20)
    main()
