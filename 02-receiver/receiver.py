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
from time import sleep
from kafka import KafkaProducer
from json import dumps
######################################################################

MAC_ADDRESS = "${mac-address}"

# We search for dynamic IP of specific MAC address


def get_ip_by_mac(macaddr):
    try:
        cmd = f'arp -a | findstr "{macaddr}" '

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
        return urllib.request.urlopen(f"http://{ipaddr}/update").read().decode("utf-8")

    except (urllib.error.URLError, http.client.HTTPException):
        print("[ERROR] Connection failed.")
        return None

######################################################################


def main():
    KAFKA_TOPIC_NAME = 'sensors'
    producer = KafkaProducer(
        bootstrap_servers=['kafka:9092'],
        value_serializer=lambda x: dumps(x).encode('utf-8')
    )

    answer = request_update(IP_ADDRESS)
    # TODO - normalize temperature, log to file
    if answer:
        producer.send(KAFKA_TOPIC_NAME, value=answer)
        print('Sent message to Kafka', flush=True)
    #print(answer, flush=True)

######################################################################


if __name__ == '__main__':
    #IP_ADDRESS = get_ip_by_mac(MAC_ADDRESS)
    IP_ADDRESS = 'ac89e0d5c309.ngrok.io'
    sleep(10)
    while True:
        main()
        sleep(10)
