var mqttsrvip = "171.221.141.157";
var mqttsrvport = 1883;
var mqttsrvpath = "/";
var mqttsrvsec = false;
var client;
var last_subscribe = null;
var sspretitle = "82109a0b-e411-4e2a-94a3-64a88fc9752f-ss-";
var ss_current_topic = "";
var rsakeypm = "i3rqu-e5d291db-b61b-44de-9ad9-RSA256-key";
var rsakeyval = "";
var rsa = forge.pki.rsa;
var pki = forge.pki;
var keypempair = {};
var mqtt_account_randrom;

var connect_srv_status = false;

function peer_change_notf(evt) {
    
    var nSel = document.getElementById("selectthispeerns_id");
        var index = nSel.selectedIndex; // 选中索引
        var text = nSel.options[index].text; 
        console.log(text);
}

function peer_addto_select_items(peer) {
    select = document.getElementById('selectthispeerns_id');

    //for (var i = min; i<=max; i++){
        var opt = document.createElement('option');
        opt.value = peer.mdtit;//i;
        opt.innerHTML = peer.aliasn;//i;
        select.appendChild(opt);
    //}
}



function addPeerPublicKey() {
    var bspeer = document.getElementById('peer-ss-key-public_id').value;
    if(bspeer){

        console.log(forge.util.decode64(bspeer));
    }
   
    //document.getElementById('peer-ss-key-public_id').textContent = "";
}

function subscribe_sec_current_topic() {
    if(connect_srv_status) {
        if(window.curretrsakey && window.curretrsakey.publickey) subscribeMqttTopic (sspretitle + getmd5key(window.curretrsakey.publickey));
    }
}

function create_current_public_key_accountinf(){
    if(window.curretrsakey && window.curretrsakey.publickey) {
        var accountss = {publickey: window.curretrsakey.publickey};
        accountss.aliasn = mqtt_account_randrom;
        if(!isNaN(Number(mqtt_account_randrom))){
            var result = prompt("指定别名特征字符", "usr");
            if(result){
               accountss.aliasn = result + mqtt_account_randrom;
            }
        }
 
        window.curretrsakey.aliasn = accountss.aliasn;

        var pubaccinf = forge.util.encode64(JSON.stringify(accountss));
         document.getElementById('current-ss-key-public_id').textContent = pubaccinf;
         document.getElementById("improt-peer-key-public_btid").disabled = false; 
         fsavepublicinfo(pubaccinf, window.curretrsakey.aliasn);
    }
}



// Called when the connection is made
function onConnect() {
    console.log("Connected!");
    connect_srv_status = true;
    var mqttPInfo = document.getElementById("mqtt-client-in");
    mqttPInfo.style.visibility = "visible";
    subscribeMqttTopic(rsakeypm);
    subscribeMqttTopic(mqtt_account_randrom);
    if(window.curretrsakey) subscribeMqttTopic (sspretitle + getmd5key(window.curretrsakey.publickey));
    if (last_subscribe) subscribeMqttTopic(last_subscribe);
}

function subscribeMqttTopic(thisTopic) {
    var subscribeOptions = {
        qos: 0
    };
    client.subscribe(thisTopic, subscribeOptions);
    console.log("subscribe:" + thisTopic);
}

function getcryptrandom(len) {
    var array = new Uint8Array(len);
    console.log(array.toString());
    window.crypto.getRandomValues(array);
    console.log(array.toString());
    return array.join('x');
}

function get_thepeer_info() {
     var pinf = {};
     var nSel = document.getElementById("selectthispeerns_id");
     var index = nSel.selectedIndex; 
     var text = nSel.options[index].text; 
     window.peerinfos.forEach(function (value) {
        if(text && value.aliasn == text) {
            pinf = value;
        }
      });
    return pinf;
}

function find_rcvpeer_info(keymd){
    var pinf = {};
    if(keymd && keymd.len == 32) {
        window.peerinfos.forEach(function (value) {
            if(value.mdtit === keymd) {
                pinf = value;
            }
          });
    }
    return pinf;
}


function sendmqttmsga() {
    var sendmsg = document.getElementById('mqttmessage_id').value;
    //var sendtopic = document.getElementById('mqtttitle_id').value;

    var sendtopic = "";
    var peerinf = get_thepeer_info();
    if(peerinf) {
        sendtopic = sspretitle + peerinf.mdtit;
    }
    if (sendmsg && sendtopic) {
        //console.log("pub:" + keypempair.publicKey);
        //var publicKey = pki.publicKeyFromPem(keypempair.publicKey);
        //var encrypted = publicKey.encrypt(sendmsg);
        // console.log("enc:" + forge.util.encode64(encrypted));
        /*        var privateKey = pki.decryptRsaPrivateKey(keypempair.privateKey, 'password');
                var decrypted = privateKey.decrypt(encrypted);
                console.log("dec:" + decrypted);*/
        var randstr = getcryptrandom(8);
        var msgnstr = sendmsg; // + '\ue101' + randstr;
        /*if(window.curretrsakey) {
            console.log("secre: is::" + encrymsg(window.curretrsakey.publickey, msgnstr));
            getmd5key(window.curretrsakey.publickey);
        }*/
        if(peerinf) {
            msgnstr = encrymsg(peerinf.publickey, msgnstr);
            console.log("send secinf:" + msgnstr);
            msgnstr = window.curretrsakey.publickeymd + msgnstr;
        }
        var message = new Paho.MQTT.Message(msgnstr);
        message.destinationName = sendtopic;
        message.qos = 0;
        client.send(message);
        console.log("send msg:" + sendmsg);
        console.log("send msg ss len:" + msgnstr.length);
        console.log("send msg ss:" + msgnstr);
        console.log("send topic:" + sendtopic);
        document.getElementById('mqttmessage_id').value = '';
    }

}

function subscribemqtttopica() {
    var topic = document.getElementById('mqttsubscribetitle_id').value;
    if (topic) {
        subscribeMqttTopic(topic);
        last_subscribe = topic;
        document.getElementById('mqttsubscribetitle_id').value = "";
    }
}

function usrioonload() {
    document.getElementById("selectthispeerns_id").addEventListener('change', peer_change_notf, false);
}


window.onload = function() {
    secretioonload();
    fileioonload();
    usrioonload();
    window.peerinfos = [];
    var sip = document.getElementById('mqttconfigserver_id').textContent;
    console.log("tt128:" + sip);
    if (sip.length) {
        if (sip.match(":")) {
            var spind = sip.indexOf(":");
            if (sip.slice(0, spind) == "wss") {
                mqttsrvsec = true;
            }
            sip = sip.slice(spind + 1);
            spind = sip.indexOf(":");
            mqttsrvip = sip.slice(0, spind);
            var sleftr = sip.slice(spind + 1);
            var spind = sleftr.indexOf("/");
            mqttsrvport = Number(sleftr.slice(0, spind));
            mqttsrvpath = sleftr.slice(spind + 1);
            //mqttsrvport = Number(sip.slice(spind + 1));
            console.log("typarse mqttserver con:" + mqttsrvip + "::" + mqttsrvport + "::" + mqttsrvpath);
        } else {
            mqttsrvip = sip;
        }

        //mqttsrvport = Number(9007)
    }
    document.getElementById('client-account-login-id').textContent = Math.floor(Math.random() * 100000);

    var mqttclient_login_account = document.getElementById('client-account-login-id').textContent;

    //console.log("mqtt acount:" + mqttclient_login_account);
    //console.log("mqtt acountn leng:" + mqttclient_login_account.length);
    var ss = mqttclient_login_account.replace(/ /g, '');
    ss = ss.replace(/\r\n/g, '');
    ss = ss.replace(/\r/g, '');
    ss = ss.replace(/\n/g, '');
    console.log("mqtt acount:" + ss + " len:" + ss.length);
    mqtt_account_randrom = ss;
    try {
        rsakeyval = window.localStorage.getItem(rsakeypm + ss);
    } catch (e) {

    }
    if (rsakeyval) {
        console.log("ras key found!");
        keypempair = JSON.parse(rsakeyval);
    } else {
        //console.log("rsa key create!");
        // var keypair = rsa.generateKeyPair({ bits: 2048, e: 0x10001 });

        //keypempair.privateKey = pki.encryptRsaPrivateKey(keypair.privateKey, 'password');
        //keypempair.publicKey = pki.publicKeyToPem(keypair.publicKey);
        
        /*try {
            window.localStorage.setItem(rsakeypm + ss, JSON.stringify(keypempair));
        } catch (e) {

        }*/
    }



    client = new Paho.MQTT.Client(mqttsrvip, mqttsrvport, mqttsrvpath, ss);

    // set callback handlers
    client.onConnectionLost = function(responseObject) {
        connect_srv_status = false;
        console.log("Connection Lost: " + responseObject.errorMessage);
        /*client.connect({
            cleanSession: true,
            onSuccess: onConnect
        });*/
        function MQTTreconnect() {
            console.log("reconnect mqtt server");
            client.connect({
                cleanSession: true,
                useSSL: mqttsrvsec,
                onSuccess: onConnect,
                onFailure: function(message) {
                    setTimeout(MQTTreconnect, 10000);
                }
            });
        };

        setTimeout(MQTTreconnect, 10000);
    }

    client.onMessageArrived = function(message) {
        console.log("Message Arrived topic: " + message.destinationName);
        //console.dir(message);
        console.log("Message Arrived: " + message.payloadString);
        console.log("Message Arrived len: " + message.payloadString.length);
        var sim = "";
        if(message.destinationName.indexOf(sspretitle) >= 0 && message.payloadString){
            /* this is secret msg */
            var chatmsg = message.payloadString;
            var rcvpper = find_rcvpeer_info(chatmsg.substr(0, 32));
            if(rcvpper) {
                console.log("Message Arrived sss: " + chatmsg.slice(32));
                sim = decrymsg(window.curretrsakey.privatekey, chatmsg.slice(32));
                
            }
        }

        var receivmsg = message.payloadString;
        if(sim) receivmsg = sim;
        var findt = receivmsg.search('\ue101');
        if (findt >= 0) { receivmsg = receivmsg.substr(0, findt); }


        document.getElementById('mqttmessage_receive_id').textContent = receivmsg;
        document.getElementById('mqtttopic_receive_id').textContent = message.destinationName;

    }

    client.connect({
        cleanSession: true,
        useSSL: mqttsrvsec,
        onSuccess: onConnect
    });
};