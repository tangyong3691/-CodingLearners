var express = require('express');
var router = express.Router();
var mongoose = require('mongoose');
var user = require('../database/db').user;

var sqlitelogindb = user();
/* GET home page. */
router.get('/', function(req, res) {
    res.render('index', { title: 'index' });
});

router.get('/opret', function(req, res) {
    res.render('result', { title: decodeURIComponent(req.query.resultn), result: req.query.ret });
});


/*login*/
router.get('/login', function(req, res) {
    res.render('login', { title: 'login' });
});


router.get('/accountnew', function(req, res) {
    res.render('accountnew', { title: '创建帐号' });
});

router.get('/timedatas', function(req, res) {
    //console.log("ttyyreqh: " + JSON.stringify(req.headers));
    console.log("ttyyreqb: " + JSON.stringify(req.body));
    res.setHeader('Content-Type', 'application/json');

    var ttrestjson1 = {};
    ttrestjson1.testcarMPH = Math.floor(Math.random() * 18000) / 100;
    console.log("rest api ret: " + JSON.stringify(ttrestjson1));

    res.send(JSON.stringify(ttrestjson1));

});

router.post('/accountnewa', function(req, res) {
    //console.log("ttyyreqh: " + JSON.stringify(req.headers));
    /*console.log("ttyyreqb: " + JSON.stringify(req.body));
    res.setHeader('Content-Type', 'application/json');

    var ttrestjson1 = {};
    ttrestjson1.testcarMPH = Math.floor(Math.random() * 18000) / 100;
    console.log("rest api ret: " + JSON.stringify(ttrestjson1));

    res.send(JSON.stringify(ttrestjson1));*/
    var query_doc = { name: req.body.name, password: req.body.password };
    sqlitelogindb.serialize(function() {
        console.log("ttyy: " + JSON.stringify(query_doc));
        sqlitelogindb.run("CREATE TABLE IF NOT EXISTS resttestab1  (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, password TEXT)",
            function() {
                sqlitelogindb.all("SELECT * FROM resttestab1 WHERE name = '" + query_doc.name + "'", function(err, rows) {
                    if (rows.length == 0) {
                        var stmt = sqlitelogindb.prepare("INSERT INTO resttestab1(name,password) VALUES (?,?)");
                        stmt.run(query_doc.name, query_doc.password);
                        stmt.finalize();
                        res.redirect('/opret?resultn=' + encodeURIComponent('创建帐号') + '&ret=sucess');
                    } else {
                        res.redirect('/opret?resultn=' + encodeURIComponent('创建帐号') + '&ret=failed');
                    }
                });

            });



    });
});


/*ucenter*/
router.post('/ucenter', function(req, res) {
    //console.log("ttyyreqh: " + JSON.stringify(req.headers));
    /*console.log("ttyyreqb: " + JSON.stringify(req.body));
    res.setHeader('Content-Type', 'application/json');

    var ttrestjson1 = {};
    ttrestjson1.testcarMPH = Math.floor(Math.random() * 18000) / 100;
    console.log("rest api ret: " + JSON.stringify(ttrestjson1));

    res.send(JSON.stringify(ttrestjson1));*/
    var query_doc = { name: req.body.name, password: req.body.password };
    sqlitelogindb.serialize(function() {
        console.log("ttyy: " + JSON.stringify(query_doc));
        sqlitelogindb.run("CREATE TABLE IF NOT EXISTS resttestab1  (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, password TEXT)",
            function() {
                sqlitelogindb.all("SELECT * FROM resttestab1 WHERE name = '" + query_doc.name + "' AND password = '" + query_doc.password + "'", function(err, rows) {
                    if (err) console.log("e:" + err);
                    let contador = 0;
                    if (rows.length) {
                        res.render('ucenter', { title: query_doc.name });
                    } else {
                        res.redirect('/opret?resultn=' + encodeURIComponent('登录状态:') + '&ret=failed');
                    }
                    rows.forEach(function(row) {
                        console.log("get resttestab1:" + row.name + " " + row.password);
                        //lista[contador] = row.nombre + ";" + row.cedula + ";" + row.edad + ";" + row.pais;
                    });
                });
            });


        /*user.count(query_doc, function(err, doc) {
            if (doc == 1) {
                console.log(query_doc.name + ": login success in " + new Date());
                console.log("ttyy5:" + JSON.stringify(doc));
                res.render('ucenter', { title: query_doc.name });
            } else {
                console.log(query_doc.name + ": login failed in " + new Date());
                res.redirect('/');
            }
        });*/
    });
});

router.post('/student', function(req, res) {
    //console.log("ttyyreqh: " + JSON.stringify(req.headers));
    //console.log("ttyyreqb: " + JSON.stringify(req.body));
    res.setHeader('Content-Type', 'application/json');

    var ttrestjson1 = {};
    ttrestjson1.testmidscores = Math.floor(Math.random() * 10000) / 100;
    console.log("rest api(student) ret: " + JSON.stringify(ttrestjson1));

    res.send(JSON.stringify(ttrestjson1));

});

router.post('/gapwidth', function(req, res) {
    console.log("gapwidth ttyyreqh: " + JSON.stringify(req.headers));
    //console.log("ttyyreqb: " + JSON.stringify(req.body));
    res.setHeader('Content-Type', 'application/json');

    var ttrestjson1 = {};
    ttrestjson1.gapwidth = Math.floor(Math.random() * 10000) / 100;
    console.log("rest api(gapwidth) ret: " + JSON.stringify(ttrestjson1));

    res.send(JSON.stringify(ttrestjson1));

});


module.exports = router;