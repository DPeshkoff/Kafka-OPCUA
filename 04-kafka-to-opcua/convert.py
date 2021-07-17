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
from json import loads, dumps
from time import sleep
######################################################################

TOPIC_NAME = 'sensors'
KAFKA_HOST = 'kafka'
KAFKA_PORT = 9092
SLEEP_TIMEOUT = 10

OPC_URL = 'opc.tcp://0.0.0.0:4840'

TYPE_TO_OPC_TYPE = {
    type(int()): VariantType.Int64,
    type(float()): VariantType.Double,
    type(bool()): VariantType.Boolean,
    type(str()): VariantType.String,
    type(None): VariantType.Null
}

OPC_VARIABLES = {}


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
    arduino_sensors = objects.add_object(namespace, 'arduino_sensors')

    return server, arduino_sensors, namespace

######################################################################


def convert_message(msg, object, namespace):
    # проходимся по полям сообщения
    # если этого поля нет среди opc переменных, добавляем
    # присваиваем значение opc переменной
    for field, value in msg.items():
        try:
            opc_value_type = TYPE_TO_OPC_TYPE[type(value)]
        except KeyError:
            print(f'Invalid type: {type(value)}, couldn\'t convert to OPC variable', flush=True)
            continue

        default_value = type(value)()
        if field not in OPC_VARIABLES:
            var = object.add_variable(namespace, field, default_value, varianttype=opc_value_type)
            OPC_VARIABLES[field] = var
        
        OPC_VARIABLES[field].set_value(value, varianttype=opc_value_type)

######################################################################


def main():
    consumer = get_kafka_consumer()
    if not consumer:
        print('[ERROR] Couldn\'t connect to Kafka consumer')
        return

    opc_server, object, namespace = get_opc_server()
    if not opc_server:
        print('[ERROR] Couldn\'t connect to OPC server')
        return

    opc_server.start()

    for event in consumer:
        message = event.value
        print(f'Got message from Kafka:\n{dumps(message, indent=4)}', flush=True)
        convert_message(message, object, namespace)
        consumer.commit()
        sleep(SLEEP_TIMEOUT)

######################################################################


if __name__ == '__main__':
    sleep(10)
    main()
