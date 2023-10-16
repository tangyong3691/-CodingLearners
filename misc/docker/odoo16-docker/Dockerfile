FROM ubuntu:20.04
RUN mkdir /usr/src/app
WORKDIR /usr/src/app
COPY ./debian-depend .
COPY ./odoo-init.conf .
COPY ./odoo-start.conf .
COPY ./odoo-16.0.post20221115/requirements.txt .
COPY ./wkhtmltox_0.12.6-1.focal_amd64.deb .
COPY odoo-16.0.post20221115 .
SHELL ["/bin/bash", "-xo", "pipefail", "-c"]
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get upgrade 
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata
RUN apt-get install -y python3-pip
RUN apt-get install -y python-dev python3-dev libxml2-dev libxslt1-dev zlib1g-dev \
  libsasl2-dev libldap2-dev build-essential libssl-dev libffi-dev libmysqlclient-dev \
  libjpeg-dev libpq-dev libjpeg8-dev liblcms2-dev libblas-dev libatlas-base-dev
#RUN apt -y install git wget nodejs npm python3 build-essential libzip-dev python3-dev libxslt1-dev python3-pip \
#                libldap2-dev python3-wheel libsasl2-dev python3-venv python3-setuptools node-less libjpeg-dev \
#                 xfonts-75dpi xfonts-base libpq-dev libffi-dev fontconfig
RUN apt-get install -y npm
#RUN ln -s /usr/bin/nodejs /usr/bin/node
RUN npm config set registry https://registry.npmmirror.com
RUN npm install -g less less-plugin-clean-css
RUN apt-get install -y node-less
#RUN npm config set registry https://mirrors.huaweicloud.com/repository/npm/ && npm install -g rtlcss
RUN pip3 install  -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple/ 
RUN apt-get install -y xfonts-base xfonts-75dpi
RUN apt-get install -y fontconfig
RUN dpkg -i wkhtmltox_0.12.6-1.focal_amd64.deb
RUN apt install -f
#RUN sed -n -e '/^Depends:/,/^Pre/ s/ python3-\(.*\),/python3-\1/p' debian-depend | xargs apt-get install -y
#RUN pip3 install wheel
#RUN pip3 install  -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple/ 
WORKDIR /usr/src/app/odoo-16.0.post20221115
#RUN pip3 install wheel
#RUN pip install  -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple/

CMD python3 odoo-bin -c ../odoo-start.conf --load-language=zh_CN
#CMD ["bash"]
