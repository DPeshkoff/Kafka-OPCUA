# Copyright 2021 DPeshkoff && let-robots-reign;
# Distributed under the GNU General Public License, Version 3.0. (See
# accompanying file LICENSE)
######################################################################
## TECHNICAL INFORMATION                                            ##
## Convert                                                          ##
## Based on Python 3.9.1 64-bit                                     ##
######################################################################
# IMPORTS
from kafka import KafkaConsumer
from opcua import Server
from opcua.ua import VariantType
from json import loads
from time import sleep
######################################################################

TOPIC_NAME = 'sensors'
KAFKA_HOST = 'kafka'
KAFKA_PORT = 9092
SLEEP_TIMEOUT = 10

OPC_URL = 'opc.tcp://0.0.0.0:4840'

TYPE_TO_OPC_TYPE = {
    type(int): VariantType.Int64,
    type(float): VariantType.Double,
    type(bool()): VariantType.Boolean,
    type(str()): VariantType.String,
    type(None): VariantType.Null
}


def get_kafka_consumer():
    return KafkaConsumer(
        TOPIC_NAME,
        bootstrap_servers=[f'{KAFKA_HOST}:{KAFKA_PORT}'],
        auto_offset_reset='earliest',
        enable_auto_commit=False,
        group_id='sensors-group',
        value_deserializer=lambda x: loads(x.decode('utf-8'))
    )

######################################################################


def get_opc_server():
    server = Server()
    server.set_endpoint(OPC_URL)

    objects = server.get_objects_node()
    namespace = server.register_namespace('Arduino\'s measurements')

    return server, objects, namespace

######################################################################


def convert_message(msg):
    # проходимся по полям сообщения
    # если этого поля нет среди opc переменных, добавляем
    # присваиваем значение opc переменной
    pass


######################################################################

def main():
    consumer = get_kafka_consumer()
    if not consumer:
        print('[ERROR] Couldn\'t connect to Kafka consumer')
        return

    opc_server, objects, namespace = get_opc_server()
    if not opc_server:
        print('[ERROR] Couldn\'t connect to OPC server')
        return

    opc_server.start()

    for event in consumer:
        message = event.value
        print(f'Got message from Kafka: {message}', flush=True)
        convert_message(message)
        consumer.commit()
        sleep(SLEEP_TIMEOUT)

######################################################################


if __name__ == '__main__':
    sleep(10)
    main()
        