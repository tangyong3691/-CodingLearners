const express=require('express');
const path = require('path');
const app = require('express')();
const http = require('http').Server(app);
const io = require('socket.io')(http);
const port = process.env.PORT || 3009;
const localnets = require("./localnets");

app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/public/index.html');
});

io.on('connection', (socket) => {
  socket.on('chat message', msg => {
    io.emit('chat message', msg);
  });
});

var localips = localnets.getlocalips();
if (localips != null) {
http.listen(port, () => {
  console.log('Socket.IO chat server running');
  //console.log(`Socket.IO chat server running at http://localhost:${port}/`);
  for (const name of Object.keys(localips))  {
   console.log( name + ": " + "http://" + localips[name] + ":" + port);
  }
});
}
else {
	console.log('error: no net connect!');
}
