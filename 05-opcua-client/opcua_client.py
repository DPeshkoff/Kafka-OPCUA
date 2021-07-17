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
from time import sleep

OPC_URL = 'opc.tcp://converter:4840'


def get_opc_client():
    client = Client(OPC_URL)
    client.connect()
    return client


def main():
    client = get_opc_client()
    # node = client.get_node("ns=1;i=1") 
    # print(node)
    # while True:
    #     print(node.get_description(), node.get_browse_name(), node.get_value(), flush=True)
    #     sleep(1)


if __name__ == '__main__':
    sleep(12)
    main()
