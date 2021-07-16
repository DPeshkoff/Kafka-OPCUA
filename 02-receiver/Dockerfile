##############################
# Dockerfile to run receiver #
# Based on Python 3.9        #
##############################

FROM python:3.9

COPY . .

RUN apt-get update && pip install kafka-python
   
CMD python3 receiver.py