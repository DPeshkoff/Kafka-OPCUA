FROM python:3.9

COPY . .

RUN apt-get update && apt-get install -y python3-tk && pip install opcua
   
CMD python3 scada.py
