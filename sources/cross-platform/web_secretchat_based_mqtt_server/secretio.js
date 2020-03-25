function getrsakey(){
    var keypair = forge.rsa.generateKeyPair({ bits: 2048, e: 0x10001 }); /* 4096 */
    var publickeypem = forge.pki.publicKeyToPem(keypair.publicKey);
    var privatekeypem = forge.pki.privateKeyToPem(keypair.privateKey);
    var rkey = {
        publickey: forge.util.encode64(publickeypem),
        privatekey: forge.util.encode64(privatekeypem)
    };

    rkey.publickeymd = getmd5key(rkey.publickey);
    
    console.log("pubtest:" + (publickeypem === forge.util.decode64(rkey.publickey)));
    console.log("pubtest2:" + (privatekeypem === forge.util.decode64(rkey.privatekey)));
   return rkey;
}

function encrymsg(publickey, msg) {
    var bsmsg = encodeURIComponent(msg);
    console.log("msg en uri:" + bsmsg);
    var thpublickey = forge.pki.publicKeyFromPem(forge.util.decode64(publickey));
    var encrypted = forge.util.encode64(thpublickey.encrypt(bsmsg, "RSA-OAEP"));
    return encrypted;
}

function decrymsg(privatekey, msg) {
    var thprivatekey = forge.pki.privateKeyFromPem(forge.util.decode64(privatekey));

    var decrypted = thprivatekey.decrypt(forge.util.decode64(msg),"RSA-OAEP");
    console.log("rec msg en uri:" + decrypted);
    return decodeURIComponent(decrypted);
    //return decrypted;
}


function secretioonload() {
    console.log("load secret onload");
    document.getElementById("creatsseasymkey").addEventListener('click', creatensskey);
}

function creatensskey(evt) {
    window.curretrsakey = getrsakey();
    subscribe_sec_current_topic();
    create_current_public_key_accountinf();
}

function getmd5key(publickey) {
   var md = forge.md.md5.create();
   md.update(publickey);
   var md5key = md.digest();
   console.log(md5key.toHex());
   return md5key.toHex();
}