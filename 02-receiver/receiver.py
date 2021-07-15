# Copyright 2021 DPeshkoff && let-robots-reign;
# Distributed under the GNU General Public License, Version 3.0. (See
# accompanying file LICENSE)
######################################################################
## TECHNICAL INFORMATION                                            ##
## Receiver                                                         ##
## Based on Python 3.9.1 64-bit                                     ##
######################################################################
# IMPORTS
import urllib.request
import http
import subprocess
import time
######################################################################

MAC_ADDRESS = "${mac-address}"

# We search for dynamic IP of specific MAC address


def get_ip_by_mac(macaddr):
    try:
        cmd = 'arp -a | findstr "{}" '.format(macaddr)

        returned_output = subprocess.check_output(
            (cmd), shell=True, stderr=subprocess.STDOUT)

        IP_ADDRESS = str(returned_output).split(' ', 1)[1].split(' ')[1]

        print("Found IP address: ", IP_ADDRESS)

        return IP_ADDRESS

    except Exception:
        print("[ERROR] IP not found, check WiFi conenction of NodeMCU.")
        exit()


######################################################################

def request_update(ipaddr):
    try:
        return urllib.request.urlopen("http://" + ipaddr + "/update").read().decode("utf-8")

    except (urllib.error.URLError, http.client.HTTPException):
        print("[ERROR] Connection failed.")

######################################################################


def main():
    answer = request_update(IP_ADDRESS)
    #TODO - normalize temperature, log to file, post to Kafka
    print(answer)

######################################################################


if __name__ == '__main__':
    IP_ADDRESS = get_ip_by_mac(MAC_ADDRESS)
    while(True):
        main()
        time.sleep(10)
