1.安装社区版（odoo_14.0.20201231.tar.gz）；
//https://www.odoo.com/documentation/14.0/administration/install/install.html#setup-install-source-linux
2.配置社区版Add-on路径，指向企业版源码目录（enterprise-14.0.zip）；
3.(bdh版本，不执行此步骤)安装应用web_enterprise和odoo_matrix，按顺序执行；
4.(bdh版本，不执行此步骤)使用注册码注册：“odoo”；




-----
odoo 14 社区版安装 + 企业版源码addon (debian)
1. python3.6.8以上
2. sudo apt install postgresql postgresql-client
   sudo -u postgres createuser -s $USER
   createdb $USER
   sudo -u postgres psql
   ALTER ROLE <user> WITH PASSWORD 'odoo123';
   alter user <user> CREATEDB;
   exit
3. sudo apt install python3-dev libxml2-dev libxslt1-dev libldap2-dev libsasl2-dev
   sudo apt install libtiff5-dev libopenjp2-7-dev zlib1g-dev libfreetype6-dev
   sudo apt install liblcms2-dev libwebp-dev libharfbuzz-dev libfribidi-dev libxcb1-dev libpq-dev
4. pip3 install setuptools wheel
   修改 requirements.txt (feedparser==5.2.1改为 feedparser==6.0.10, centos7.9)
5. pip3 install -r requirements.txt -i https://mirrors.aliyun.com/pypi/simple
6. cp setup/odoo odoo-bin
7. export PYTHONPATH=.
8. (原命令)python odoo-bin -i base -r $USER -w odoo123 --addons-path=/opt/odoo14/enterprise-14.0 -d $USER
  (bdh ver，采用配置,首测使用install.conf)  python odoo-bin -c ../odoo-install.conf --load-language=zh_CN
    install.conf配置：
	  admin_passwd = Hw!202210252353   （设置创建数据库的admin密码）
	  db_name = False
	  proxy_mode = True  （nginx代理访问）
	  without_demo = True
	  #workers = 64   （不能配置线程）
   首测运行后，配置界面中创建数据库（需要postgresql给帐号创建库的权限）
	  
	（bdh version，后续运行）python odoo-bin -c ../odoo-start.conf --load-language=zh_CN
	  odoo-start.conf配置:
	    db_name = BDH_EE  (设置已创建的数据库名称)
		workers = 64  (设置线程)
注：生产环境需用ningx代理（解决某些pdf文件无法在线浏览的问题)

访问：http://ip:8069

---------------------
采用源码编译python3.6.8的工具链 使用make altinstall安装后，需更新pip
python3.6 -m pip install --upgrade pip -i https://mirrors.aliyun.com/pypi/simple

python3.6 -m pip install -r requirements.txt -i https://mirrors.aliyun.com/pypi/simple
