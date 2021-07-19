##############################
# Dockerfile to run receiver #
# Based on python 3.9        #
##############################

FROM python:3.9

COPY . .

RUN apt-get update && pip install kafka-python && pip install requests

CMD python3 receiver.py

LABEL \
      name="Kafka-OPCUA Receiver" \
      description="Launch Kafka-OPCUA Receiver" \
      version="1.0" \
      maintainer="zo...i.1@yandex.ru"
