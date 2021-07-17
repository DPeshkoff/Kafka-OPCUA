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
from time import sleep

OPC_URL = 'opc.tcp://converter:4840'


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
            vars.append((node.get_browse_name(), node.get_value()))
        except BadNodeIdUnknown:
            break
        i += 1
    return vars


def main():
    client = get_opc_client()
    while True:
        vars = get_all_variables(client)
        for var in vars:
            print(*var, flush=True)
        sleep(10)


if __name__ == '__main__':
    sleep(20)
    main()
