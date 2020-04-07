/* from https://nodejs.org/zh-cn/docs/guides/anatomy-of-an-http-transaction/
 */
const http = require('http');
const server = http.createServer((req, res) => {
    console.log("url: " + req.url);
    console.log("method: " + req.method);
    console.log("Content-Type: " + req.headers["content-type"]);
    console.log("Accept: " + req.headers["accept"]);
    //res.writeHead(200,{"Content-Type":"text\plain"});
    res.setHeader('Content-Type', 'text\plain');
    //res.setHeader('Content-Type', 'application/json');
    //res.statusCode = 404;
    let body = [];
    if (req.method === 'POST' || req.method === 'PUT' || req.method === 'GET' || req.method === 'DELETE') {
           
    req.on('data', chunk => {
        //body += chunk.toString(); // convert Buffer to string
        body.push(chunk);
    });
  
    req.on('end', () => {
        console.log(Buffer.concat(body).toString());
        if(req.method === 'POST' || req.method === 'PUT')
           res.end(Buffer.concat(body).toString());
        else if(req.method === 'GET'){
             var reshtml = req.headers["accept"];
             reshtml = reshtml && reshtml.indexOf('text/html') > -1;
             if(reshtml) {
               res.writeHead(200,{"Content-Type":"text\html"});
               res.end('<html><body><h1>ok!</h1></body></html>');
             } else {
               res.statusCode = 404;
               res.end('');
             }
         }
        else
           res.end('');
    });
    
    req.on('error', (err) => {
       console.error(err.stack);
    });
    }
      
});

console.log("http post listen:22002");

server.listen(22002);
