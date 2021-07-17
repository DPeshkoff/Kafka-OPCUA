# Copyright 2021 DPeshkoff && let-robots-reign;
# Distributed under the GNU General Public License, Version 3.0. (See
# accompanying file LICENSE)
######################################################################
## TECHNICAL INFORMATION                                            ##
## Receiver                                                         ##
## Based on Python 3.9.1 64-bit                                     ##
######################################################################
# IMPORTS
from os import system
from sys import stdout
import requests
import subprocess
import logging
from time import sleep
from kafka import KafkaProducer
from json import dumps, loads
from datetime import date, datetime
from math import log, log10
######################################################################
# PRIVATE DATA

MAC_ADDRESS = "${mac}"
TOKEN = '${token}'

######################################################################
# GlOBAL DATA

RECEIVER_MODE = "TUNNEL"  # LOCAL or TUNNEL

######################################################################
# We search for dynamic IP of specific MAC address


def get_ip_by_mac(macaddr):
    try:
        # for now - uncomment if arp -a does not see NodeMCU
        # TODO - more accurate solution
        # for i in range (0, 255):
        #    system(f"ping 192.168.0.{i}")
        
        cmd = f'arp -a | findstr "{macaddr}" '

        returned_output = subprocess.check_output(
            (cmd), shell=True, stderr=subprocess.STDOUT)

        IP_ADDRESS = str(returned_output).split(' ', 1)[1].split(' ')[1]

        return IP_ADDRESS

    except Exception:
        print("[ERROR] IP not found, check WiFi connection of NodeMCU.")
        exit()


######################################################################
# http-get to nodemcu server


def request_update(ipaddr):
    try:
        return requests.get(
            f"http://{ipaddr}/update",
            headers={'Authorization': TOKEN},
        ).content.decode("utf-8")

    except (requests.ConnectionError):
        print("[ERROR] Connection failed.")
        return None

    except (requests.HTTPError):
        print("[ERROR] HTTP connection failed.")
        return None

    except (requests.Timeout):
        print("[ERROR] Connection timeout.")
        return None

######################################################################
# Convert raw voltages to SI format


def normalize(answer):

    # Convert raw temperature data to Celsius degrees

    normalized_temperature = 1 / \
        (log((answer["temperature"] * 5.0 / 1023.0) / 2.5) /
         4300.0 + 1.0 / 298.0) - 273.0

    # Convert raw brightness data to lux

    normalized_brightness = pow(10, (log10(
        20000 / ((answer["brightness"] * 10000) / (1024 - answer["brightness"]))) / 0.37))

    # Round and save
    answer["temperature"] = round(normalized_temperature, 2)
    answer["brightness"] = round(normalized_brightness, 2)

    return answer


######################################################################

def get_data(ip):
    answer = request_update(ip)

    if answer:
        logging.info(
            f'{datetime.now().time().replace(microsecond=0)} {answer}')

        answer = normalize(loads(answer))

        logging.info(
            f'{datetime.now().time().replace(microsecond=0)} Normalized data: {answer}')

        return answer
    
    return None

######################################################################


def main():
    dn = date.today()

    logging.basicConfig(filename=f"{dn}.log", level=logging.INFO)
    
    logging.getLogger().addHandler(logging.StreamHandler(stdout))

    logging.info(
        f'{datetime.now().time().replace(microsecond=0)} New session starting')

    if (RECEIVER_MODE == "LOCAL"):
        IP_ADDRESS = get_ip_by_mac(MAC_ADDRESS)

    elif (RECEIVER_MODE == "TUNNEL"):
        IP_ADDRESS = '23ad4b6b7721.ngrok.io'
    else:
        print("[ERROR] Please specify receiver mode")

    logging.info(
        f'{datetime.now().time().replace(microsecond=0)} Starting for IP: {IP_ADDRESS}')

    sleep(10)
    
    KAFKA_TOPIC_NAME = 'sensors'
    producer = KafkaProducer(
        bootstrap_servers=['kafka:9092'],
        value_serializer=lambda x: dumps(x).encode('utf-8')
    )

    while True:
        answer = get_data(IP_ADDRESS)
        if answer:
            producer.send(KAFKA_TOPIC_NAME, value=answer)
            logging.info(
                f'{datetime.now().time().replace(microsecond=0)} Sent message to Kafka')

        sleep(10)

######################################################################


if __name__ == '__main__':
    main()
