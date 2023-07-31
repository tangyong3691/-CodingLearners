FROM python:3.7-bullseye
RUN sed -i "s/deb\.debian\.org/mirrors.aliyun.com/g" /etc/apt/sources.list 
ARG DEBIAN_FRONTEND=noninteractive
RUN apt update && apt -o Dpkg::Options::="--force-confold" upgrade -q -y --force-yes && apt -o Dpkg::Options::="--force-confold" dist-upgrade -q -y --force-yes
RUN apt -y install gcc
WORKDIR /app
COPY ./opc-ua-sensor-simulator /app
##ADD ./opc-ua-sensor-simulator /

RUN pip install  -r requirements.txt -i https://mirrors.aliyun.com/pypi/simple 

CMD ["python", "opc-ua-server.py"]
