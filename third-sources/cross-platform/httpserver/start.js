/**
 * @file 实现E515,E529等Websocket SVR，HTTP，Proxy代理
 *		 
 *	启动格式：node --inspect broker.js proto host port
 *
 *	文件名			:broker.js	
 *	编程者			:Awen
 *	日期			:2017.10.25	
 *	项目名			:车联网超级终端 Platform技术预研	
 *	操作系统		:Cross
 *
 *	编号----日期--------注释-------------------------------
 *	00		17.10.25	初期作成
 *	01		17.10.26	修改×××
 */
console.log('http server start..');
console.log('Usage:  argument:  -p <port> -r <root>');
var url = require("url"),
    fs = require("fs"),
    http = require("http"),
    path = require("path");

var debugbrk = require("./iovdebug.js").getDebug('broker');
var argv = require('optimist').argv;
debugbrk('%o', argv);
var root_dirname = argv.r || ".";
debugbrk('root:%s', root_dirname);
var port = argv.p || 80;
debugbrk('port:%o', port);
console.log('port: ' + port + ' , root: ' + root_dirname);

function handle_request(req, res) {
    debugbrk('url:%s', req.url);
    debugbrk('%s', url.parse(req.url).pathname);
    var pathname = root_dirname + url.parse(req.url).pathname;
    if (pathname.charAt(pathname.length - 1) === "/") {
        pathname += 'broker.htm';
    }
    var exists = fs.existsSync(pathname); //,function(exists){
    if (exists) {
        switch (path.extname(pathname)) {
            case ".html":
            case ".htm":
                res.writeHead(200, { "Content-Type": "text/html" });
                break;
            case ".js":
                res.writeHead(200, { "Content-Type": "text/javascript" });
                break;
            case ".css":
                res.writeHead(200, { "Content-Type": "text/css" });
                break;
            case ".gif":
                res.writeHead(200, { "Content-Type": "image/gif" });
                break;
            case ".jpg":
                res.writeHead(200, { "Content-Type": "image/jpeg" });
                break;
            case ".png":
                res.writeHead(200, { "Content-Type": "image/png" });
                break;
            default:
                res.writeHead(200, { "Content-Type": "application/octet-stream" });
        }
        fs.readFile(pathname, function(err, data) {
            res.end(data);
        });
    } else {
        res.writeHead(404, { "Content-Type": "text/html" });
        res.end("<h1>404 Not Found</h1>");
    };
}


var server = http.createServer(handle_request);
server.listen(port);