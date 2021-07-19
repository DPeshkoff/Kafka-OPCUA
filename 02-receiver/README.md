# Receiver Module
Receiver module consists of python script.

## Usage

Before you start, please check two global variables ```TOKEN``` (which is absolutely neccessary) and ```MAC_ADDRESS``` (optional) and correct them according to your project's settings.

* If you already have public IP of NodeMCU server, select ```"TUNNEL"``` mode to connect to it. 
* If you connect to the server, using local network, select ```"LOCAL"``` mode. If receiver returns error, uncomment lines ```38-39-40``` or ping every address in local pool. The problem should be solved.

Receiver then will connect to server every 10 seconds, normalize received data and publish it to Apache Kafka. 

Moreover, receiver logs important data to text file. Log is duplicated to stdout.

Docker container is available, as is docker-compose for further modules of the project.

## Used libraries

* Requests;
* Kafka-Python.

## Screenshots

Screenshot of receiver log:

![](screenshots/01-log.png =1190x1115)


## Project structure
### Folders

```bash
02-receiver
|--screenshots
|
|-receiver.py #main script
|
|-Dockerfile
|
|-README.md
```

### Code style

Used code style is [PEP8 style guide](https://pep8.org/).