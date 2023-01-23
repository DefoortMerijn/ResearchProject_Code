import http from 'http';
import fs from 'fs';
import { SerialPort } from 'serialport';
import { Server } from 'socket.io';
import { ReadlineParser } from 'serialport';
import express from 'express';

var port = new SerialPort({
  path: 'COM3',
  baudRate: 9600,
});
const parser = new ReadlineParser();
port.pipe(parser);

var app = express();
const server = app.listen(3000, function () {
  console.log('listening on port 3000');
});
const io = new Server(server, {
  allowEIO3: true,
  cors: { credentials: true, origin: 'http://127.0.0.1:3000' },
});

io.on('connection', function (socket) {
  console.log('connected!' + socket.id);
  io.emit('connected', 'you are connected');
});

parser.on('data', function (data) {
  // console.log(data);
  io.emit('data', data);
});


// Open errors will be emitted as an error event
port.on('error', function (err) {
  console.log('Error: ', err.message);
});
