
--------
使用:
1. （可选，特性：支持mqtt主题通信)添加config.json文件，参考config.json.example
2. 运行:
   node app.js
3. 访问:
   http://127.0.0.1:9035
--------
(version 0.0.2)基于原有的chihuo_0.01修改，数据库采用sqlite3，支持特性:
1. 基于sqlite3数据库的帐号创建、登录
2. 基于文件config.json的配置支持
3. 实现连接mqtt服务器，发送主题消息(config.json.example)，订阅、接收主题消息

-----------------------------------
下面是项目原作者留下的信息：
-----------------------------------------------
chihuo_0.01
===========

初识nodejs，学习总结。
版本v0.0.1

我在[汇智网](http://www.hubwiz.com)上面 还写有一个[《node.js实战》](http://www.hubwiz.com/course/549a704f88dba0136c371703/)的一个小项目，课程是做一个小的电商网站

感兴趣的可以去看一下。
