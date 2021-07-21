# Kafka to OPC UA Module
Kafka to OPC UA module consists of Dockefile and two scripts. It collects data from Kafka and runs as a OPC UA server for modules 05 and 06.

## Used libraries

* OPCUA;
* Kafka-Python.

## Project structure
### Folders

```bash
04-kafka-to-opcua
|-convert.py
|
|-faker.py #fakes measurements
|
|-Dockerfile
|
|-README.md
```