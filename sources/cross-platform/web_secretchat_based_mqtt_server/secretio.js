
const RSABITS= 2048;
var rsacrylen = RSABITS/8;
//what-is-the-maximum-size-of-the-plaintext-message-for-rsa-oaep
//RSA 2048, SHA-1, mlen = 214; RSA 2048, SHA-256, mlen = 190
//RSA 4096, SHA-1, mlen = 470; RSA 4096, SHA-512, mlen = 382
var rsa2klen = 214; 
var rsa4klen = 382; 

function getrsakey(){
    var keypair = forge.rsa.generateKeyPair({ bits: RSABITS, e: 0x10001 }); /* 4096 */
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
    var rsamlen = 126;
    if(RSABITS == 2048) rsamlen = rsa2klen;
    if(RSABITS == 4096) rsamlen = rsa4klen;
    var encrypted = "";
    if(bsmsg.length > rsamlen) {
        var leng = 0;
           do {
               leng = bsmsg.length;
               encrypted += thpublickey.encrypt(bsmsg.slice(0, rsamlen), "RSA-OAEP");
               //console.log(forge.util.encode64(thpublickey.encrypt(bsmsg.slice(0, rsa2klen), "RSA-OAEP")));
               bsmsg = bsmsg.slice(rsamlen);
           } while ( leng > rsamlen);

    } else {
        encrypted = thpublickey.encrypt(bsmsg, "RSA-OAEP");
    }
    
    return forge.util.encode64(encrypted);
}

function decrymsg(privatekey, msg) {
    var thprivatekey = forge.pki.privateKeyFromPem(forge.util.decode64(privatekey));

    var crmsg = forge.util.decode64(msg);
    var decrypted = "";
    if(crmsg.length > rsacrylen) {
        var leng = 0;
        do {
            leng = crmsg.length;
            decrypted += thprivatekey.decrypt(crmsg.slice(0, rsacrylen),"RSA-OAEP");
            crmsg = crmsg.slice(rsacrylen);
        } while (leng > rsacrylen);
    }else {
        decrypted = thprivatekey.decrypt(crmsg,"RSA-OAEP");
    }
    
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