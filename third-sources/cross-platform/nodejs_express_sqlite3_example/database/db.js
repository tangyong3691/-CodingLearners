/*var mongoose = require("mongoose"); 
mongoose.connect('mongodb://localhost:27017/testchihuo');
var Schema = mongoose.Schema; 
var userScheMa = new Schema({
    name: String,
    password: String
}); 
exports.user = mongoose.model('testusers', userScheMa); */

const sqlite3 = require('sqlite3').verbose();
exports.user = function() {
    return new sqlite3.Database('./db/chinook.db', sqlite3.OPEN_READWRITE | sqlite3.OPEN_CREATE, (err) => {
        if (err) {
            console.error(err.message);
        }
        console.log('Connected to the chinook database.');
    });
}