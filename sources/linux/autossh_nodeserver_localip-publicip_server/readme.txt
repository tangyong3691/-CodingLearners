this project aim:
when You in local network , you can manual get my public.
but when one server(no one stand by it, or can't direct use it) in loal network someware, you need remote login it with public server bridge like frp tools. but public server limit ip for safety. so you need remote get this server's public ip (this ip could change) .
-----------------------
方案是通过在公网服务器建立一个node.js rest server, 内网的电脑或服务器配置为自动向公网服务器汇报它的公网ip(运营商可能会改变公网的ip), 这样远程的人可以根据此信息，来修改远程login的服务器（比如frp工具搭建)的限制ip方案，保证已知的ip可访问，既达到限制黑客侵入，又能安全的远程使用内网电脑和服务器。
此方案能可靠运行，但仍然不是整体的解决方案，需要自行搭建环境。