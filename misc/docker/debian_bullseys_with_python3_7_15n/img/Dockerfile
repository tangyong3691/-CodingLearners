# This is a simple Dockerfile to use while developing
# It's not suitable for production
#
# It allows you to run both flask and celery if you enabled it
# for flask: docker run --env-file=.flaskenv image flask run
# for celery: docker run --env-file=.flaskenv image celery worker -A myapi.celery_app:app
#
# note that celery will require a running broker and result backend
FROM debian:bullseye
WORKDIR /
COPY sources.list /etc/apt/
RUN apt clean 
RUN apt update 
RUN DEBIAN_FRONTEND=noninteractive apt upgrade -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold"
RUN apt install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libsqlite3-dev libreadline-dev libffi-dev curl libbz2-dev -y
RUN apt install g++ -y
RUN apt install unixodbc-dev  -y
#RUN apt install python3 -y
#RUN apt-cache policy python3
RUN mkdir /code
WORKDIR /code


COPY Python-3.7.15.tgz .
RUN tar zxvf Python-3.7.15.tgz
WORKDIR /code/Python-3.7.15
RUN ./configure --enable-optimizations --enable-shared
RUN make -j 20
RUN make altinstall
RUN ldconfig /code/Python-3.7.15

WORKDIR /code
#COPY get-pip.py ./
COPY pip-23.0.1.tar.gz ./
RUN tar zxvf pip-23.0.1.tar.gz
WORKDIR /code/pip-23.0.1
RUN python3.7 setup.py install
#RUN python3.7 -m pip install --upgrade pip
WORKDIR /code
#RUN pip3.7 install Werkzeug
COPY tcp_server_examp.py ./

#COPY requirements.txt setup.py tox.ini ./
#RUN pip install -U pip -i https://mirrors.aliyun.com/pypi/simple
#RUN pip install -r requirements.txt -i https://mirrors.aliyun.com/pypi/simple
#RUN pip install -e .

#EXPOSE 5999
