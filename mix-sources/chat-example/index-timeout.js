const express=require('express');
const path = require('path');
const app = require('express')();
const http = require('http');
const { Server } = require('socket.io');
const port = process.env.PORT || 30089;
const localnets = require("./localnets");
const server = http.createServer(app);
app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/public/index.html');
});

const io = new Server(server, {
    pingInterval: 25000,
    pingTimeout: 120000,
    connectTimeout: 60000,

    transports: ['websocket', 'polling'],

    allowUpgrades: true,

    cors: {
        origin: '*',
        methods: ['GET', 'POST']
    }
});

io.on('connection', (socket) => {
  socket.on('chat message', msg => {
    io.emit('chat message', msg);
  });
});

var localips = localnets.getlocalips();
if (localips != null) {
server.listen(port, () => {
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
