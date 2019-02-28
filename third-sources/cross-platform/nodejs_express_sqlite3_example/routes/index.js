var express = require('express');
var router = express.Router();
//var mongoose = require('mongoose');
var user = require('../database/db').user;
var fs = require('fs');
var path = require('path');

var sqlitelogindb = user();
global.lclfileconfig = {};
var iauthdepot = global.invitationauthdepot = [];

function randInvite6Create() {
    var text = "";
    var possible = "ADEGHNQRTYadehnqty3457";
    var ss = require('crypto').randomBytes(4).readUInt32BE(0);
    var range = possible.length;
    var ind;
    for (var i = 0; i < 6; i++) {
        ind = ss % range;

        text += possible.charAt(ind);
        possible = possible.slice(0, ind) + possible.slice(ind + 1);
        ss = Math.floor(ss / range);
        range -= 1;
    }
    return text;
}

/* name must be valid mqtt topic  */
function isValidUsrName(name) {
    //var invalidch = [0x0, 0xa, 0xd, 0x20, 0x23, 0x2b]; // #+ blank
    var length = name.length;
    // ()\_%;,'""
    var invalstr = "\u000a\u000d\u0020\u0023\u002b\u005c\u005f\u0025\u003b\u0027\u0022\u0028\u0029\u002c\u0000"; //invalidch.toString();
    //console.log("invalid str len:" + invalstr.length);
    if (length == 0 || length == null || length > 64) return false;
    for (var i = 0; i < length; i++) {
        var ucode = name.charCodeAt(i);
        if ((invalstr.indexOf(name.charAt(i)) != -1) || (ucode >= 0 && ucode <= 0x1f) || (ucode >= 0x7f && ucode <= 0x9f)) return false;
    }
    return true;
};

function isValidPasswd(passwd) {
    var length = passwd.length;
    if (length == 0 || length == null || length > 64) return false;
    return true;
};



(function() {
    console.log("local config file:" + path.join(__dirname, '../config.json'));
    if (fs.existsSync(path.join(__dirname, '../config.json'))) {
        global.lclfileconfig = JSON.parse(fs.readFileSync(path.join(__dirname, '../config.json')).toString());
        console.log("fileconfig:" + JSON.stringify(global.lclfileconfig));
    };
})();



/* GET home page. */
router.get('/', function(req, res) {
    res.render('index', { title: 'index' });
});

router.get('/opret', function(req, res) {
    var renderArg = { title: decodeURIComponent(req.query.resultn), result: req.query.ret };

    if (req.query.hasOwnProperty('faildescribe')) {
        renderArg.result = decodeURIComponent(req.query.faildescribe);
    } else if (req.query.ret == 'success') {
        renderArg.result = '成功';
    } else if (req.query.ret == 'failed') {
        renderArg.result = '失败';
    }
    res.render('result', renderArg);
});


/*login*/
router.get('/login', function(req, res) {
    res.render('login', { title: 'login' });
});

router.get('/test', function(req, res) {
    res.render('test');
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
    if (!isValidUsrName(req.body.name)) {
        res.redirect('/opret?resultn=' + encodeURIComponent('创建帐号') + '&ret=failed' + '&faildescribe=' + encodeURIComponent('失败：此帐号名称不支持，请更改'));
        return;
    };

    if (!isValidPasswd(req.body.password)) {
        res.redirect('/opret?resultn=' + encodeURIComponent('创建帐号') + '&ret=failed' + '&faildescribe=' + encodeURIComponent('失败：此密码不支持，请更改'));
        return;
    };

    var query_doc = { name: req.body.name, password: req.body.password, invitate: req.body.invitatecode };
    sqlitelogindb.serialize(function() {
        console.log("ttyy: " + JSON.stringify(query_doc));
        sqlitelogindb.run("CREATE TABLE IF NOT EXISTS resttestab1  (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, password TEXT)",
            function() {
                var isFirstAccount = false;
                var checkfuser = function() {
                    return new Promise((resolve, reject) => {
                        //console.log("aaa checkfuser");
                        sqlitelogindb.all("SELECT * FROM resttestab1", function(err, rows) {
                            //console.log("tsiieo :" + rows.length);
                            isFirstAccount = rows.length ? false : true;
                            resolve();
                        });
                    });
                };

                var checkvalidinvite = function() {
                    return new Promise((resolve, reject) => {
                        //console.log("aaa checkvalidinvite");
                        var idn = -1;
                        if (isFirstAccount) resolve();
                        else {
                            idn = iauthdepot.indexOf(query_doc.invitate);
                            if (idn !== -1) {
                                iauthdepot.splice(idn, 1);
                                resolve();
                            } else {
                                res.redirect('/opret?resultn=' + encodeURIComponent('创建帐号') + '&ret=failed' + '&faildescribe=' + encodeURIComponent('失败: 邀请码错误'));
                                reject();
                            }
                        }
                    });
                };
                var checkaccount = function() {
                    return new Promise((resolve, reject) => {
                        //console.log("aaa checkaccount");
                        sqlitelogindb.all("SELECT * FROM resttestab1 WHERE name = '" + query_doc.name + "'", function(err, rows) {
                            if (rows.length == 0) {
                                var stmt = sqlitelogindb.prepare("INSERT INTO resttestab1(name,password) VALUES (?,?)");
                                stmt.run(query_doc.name, query_doc.password);
                                stmt.finalize();
                                res.redirect('/opret?resultn=' + encodeURIComponent('创建帐号') + '&ret=success');
                            } else {
                                res.redirect('/opret?resultn=' + encodeURIComponent('创建帐号') + '&ret=failed' + '&faildescribe=' + encodeURIComponent('失败：此帐号已存在，请填写新的帐号'));
                            }
                        });
                    });
                };

                checkfuser()
                    .then(checkvalidinvite)
                    .then(checkaccount)
                    .catch(function(reason) {
                        //console.log("idio catchit");
                    });
                //Promise.all([checkfuser, checkvalidinvite, checkaccount]).then();
                //console.log("tdii30s3,hee");
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
        console.log("ttyy account: " + query_doc.name + "len:" + query_doc.name.length);
        sqlitelogindb.run("CREATE TABLE IF NOT EXISTS resttestab1  (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, password TEXT)",
            function() {
                sqlitelogindb.all("SELECT * FROM resttestab1 WHERE name = '" + query_doc.name + "' AND password = '" + query_doc.password + "'", function(err, rows) {
                    if (err) console.log("e:" + err);
                    let contador = 0;
                    var mqttServerIp = "115.153.77.48";
                    if (global.lclfileconfig.hasOwnProperty("mqttServerIp")) mqttServerIp = global.lclfileconfig.mqttServerIp;
                    if (global.lclfileconfig.hasOwnProperty("mqttServerPort")) mqttServerIp += ":" + global.lclfileconfig.mqttServerPort;
                    if (rows.length) {
                        var invite1 = randInvite6Create();
                        var invite2 = randInvite6Create();
                        if (iauthdepot.length > 10) {
                            iauthdepot.shift();
                            iauthdepot.shift();
                        }
                        iauthdepot.push(invite1, invite2);


                        res.render('ucenter', { title: query_doc.name, mqttsrvi: mqttServerIp, invite1: invite1, invite2: invite2 });
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