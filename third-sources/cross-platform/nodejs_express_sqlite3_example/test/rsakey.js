var keypair = require('keypair');
var pair = keypair({ bits: 2048 });
console.log(pair);
//var ursa = require('ursa');
var forge = require('node-forge');
var publicKey = forge.pki.publicKeyFromPem(pair.public);
var privateKey = forge.pki.privateKeyFromPem(pair.private);
//var privkeyBob = ursa.createPrivateKey(pair);
//var pubkeyAlice = ursa.createPublicKey(pair);
//console.log("ttpriv: " + publicKey);
var msg = 'heioo2';
console.log("orgin msg:" + msg);
var enc = publicKey.encrypt(msg, 'RSA-OAEP');
//console.log("enc:" + JSON.stringify(enc));
var base64enc = process.browser ? btoa(enc) : Buffer(enc).toString('base64');
console.log("base64enc:" + base64enc);
//console.log("cahn:" + JSON.stringify(Buffer.from(base64enc, 'base64').toString('utf8')));
var enc2 = process.browser ? atob(base64enc) : Buffer.from(base64enc, 'base64').toString('utf8');
var decmsg = privateKey.decrypt(enc2, 'RSA-OAEP');
console.log("dec:" + decmsg);
//https://github.com/broofa/node-uuid
//https://segmentfault.com/a/1190000015469294

//var uuid = guid();
//console.log("uuid:" + uuid);

// 6个字符，每个字符32个变化
//Math.floor(Math.random()*1073741824)

//<script src="/js/rsakeybundle.js" type="text/javascript"></script>