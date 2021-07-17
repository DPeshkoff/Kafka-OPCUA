FROM python:3.9

COPY . .

RUN apt-get update && pip install kafka-python && pip install opcua
   
CMD python3 convert.py
