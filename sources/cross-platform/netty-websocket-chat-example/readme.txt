Chinese description:
这是按照使用ChatGPT(gpt-3.5-turbo)和Claude-instant学习开发的结果，可作为初学者学习java开发的例子，同时也是一个可作为个人使用的开箱即用的消息对话软件。
开发过程中参考了chat-example(node.js socket.io server)运行时的网络交互流程（wireshark抓包分析)。
申明： 不对使用软件造成的问题承担责任，使用者需自行承担所有责任

--------
this is a java demo compatible with the socket.io,also It can be used immediately without any experience or skills required.
1. websocket server , support socket.io v4.;
2. include http server, so you can use it with browser(http://ip:port);
3. include "chat message" broadcast to all client (socket.io).