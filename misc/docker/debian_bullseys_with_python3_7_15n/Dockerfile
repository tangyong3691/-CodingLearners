# This is a simple Dockerfile to use while developing
# It's not suitable for production
#
# It allows you to run both flask and celery if you enabled it
# for flask: docker run --env-file=.flaskenv image flask run
# for celery: docker run --env-file=.flaskenv image celery worker -A myapi.celery_app:app
#
# note that celery will require a running broker and result backend
FROM debian_withpython3_7_15test:latest

WORKDIR /code
#RUN pip3 install Werkzeug -i https://mirrors.aliyun.com/pypi/simple
COPY tcp_server_examp.py ./
COPY requirements.txt setup.py tox.ini ./
RUN pip3 install -r requirements.txt -i https://mirrors.aliyun.com/pypi/simple
RUN pip3 install -e .
COPY ytestapp ytest/
COPY ttapp ttapp/
COPY migrations migrations/
#COPY requirements.txt setup.py tox.ini ./
#RUN pip install -U pip -i https://mirrors.aliyun.com/pypi/simple
#RUN pip install -r requirements.txt -i https://mirrors.aliyun.com/pypi/simple
#RUN pip install -e .

EXPOSE 5000
