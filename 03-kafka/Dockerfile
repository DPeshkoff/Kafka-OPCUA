##############################
# Dockerfile to run broker   #
# Based on Ubuntu            #
##############################

FROM ubuntu:20.04

RUN apt update && \
    apt-get install curl -y && \
    mkdir ~/Downloads && \
    curl "https://downloads.apache.org/kafka/2.6.2/kafka_2.13-2.6.2.tgz" -o ~/Downloads/kafka.tgz && \
    mkdir ~/kafka && cd ~/kafka && \
    tar -xvzf ~/Downloads/kafka.tgz --strip 1

COPY ./kafka-config/server.properties ~/kafka/config

COPY ./kafka-config/zookeeper.service ~/etc/systemd/system/

COPY ./kafka-config/kafka.service ~/etc/systemd/system/

RUN mv ~/kafka /kafka && \
    sed -i s/localhost:2181/zookeeper:2181/g /kafka/config/server.properties && \
    sed -i s/broker.id=0/broker.id=0/g /kafka/config/server.properties

EXPOSE 9092
EXPOSE 2181

LABEL \
      name="Kafka Broker" \
      description="Launch Kafka Broker" \
      version="1.0" \
      maintainer="dm...kov@gmail.com"
