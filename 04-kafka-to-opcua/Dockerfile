##############################
# Dockerfile to run converter#
# Based on python 3.9        #
##############################

FROM python:3.9

COPY . .

RUN apt-get update && pip install kafka-python && pip install opcua
   
CMD python3 convert.py

LABEL \
      name="Kafka-to-OPCUA Converter" \
      description="Launch Kafka-to-OPCUA Converter" \
      version="1.0" \
      maintainer="zo...i.1@yandex.ru"
