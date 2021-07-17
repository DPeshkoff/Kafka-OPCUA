FROM python:3.9

COPY . .

RUN apt-get update && pip install kafka-python && pip install requests

CMD python3 receiver.py
