from kafka import KafkaConsumer
from json import loads
from time import sleep

TOPIC_NAME = 'sensors'
KAFKA_HOST = 'kafka'
KAFKA_PORT = 9092
SLEEP_TIMEOUT = 10


def main():
    consumer = KafkaConsumer(
            TOPIC_NAME,
            bootstrap_servers=[f'{KAFKA_HOST}:{KAFKA_PORT}'],
            auto_offset_reset='earliest',
            enable_auto_commit=False,
            group_id='detector-group',
            value_deserializer=lambda x: loads(x.decode('utf-8'))
        )

    for event in consumer:
        message = event.value
        print(f'Got message from Kafka: {message}', flush=True)
        consumer.commit()
        sleep(SLEEP_TIMEOUT)


if __name__ == '__main__':
    sleep(10)
    main()
        