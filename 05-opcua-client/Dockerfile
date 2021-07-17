FROM python:3.9

COPY . .

RUN apt-get update && pip install requests && pip install opcua
   
CMD python3 opcua_client.py
