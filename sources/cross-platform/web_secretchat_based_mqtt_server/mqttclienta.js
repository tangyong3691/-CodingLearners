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

function subscribe_sec_current_topic() {
    if(connect_srv_status) {
        if(window.curretrsakey) subscribeMqttTopic (sspretitle + getmd5key(window.curretrsakey.publickey));
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


function sendmqttmsga() {
    var sendmsg = document.getElementById('mqttmessage_id').value;
    var sendtopic = document.getElementById('mqtttitle_id').value;
    if (sendmsg && sendtopic) {
        //console.log("pub:" + keypempair.publicKey);
        //var publicKey = pki.publicKeyFromPem(keypempair.publicKey);
        //var encrypted = publicKey.encrypt(sendmsg);
        // console.log("enc:" + forge.util.encode64(encrypted));
        /*        var privateKey = pki.decryptRsaPrivateKey(keypempair.privateKey, 'password');
                var decrypted = privateKey.decrypt(encrypted);
                console.log("dec:" + decrypted);*/
        var randstr = getcryptrandom(8);
        var msgnstr = sendmsg + '\ue101' + randstr;
        if(window.curretrsakey) {
            console.log("secre: is::" + encrymsg(window.curretrsakey.publickey, msgnstr));
            getmd5key(window.curretrsakey.publickey);
        }
        var message = new Paho.MQTT.Message(msgnstr);
        message.destinationName = sendtopic;
        message.qos = 0;
        client.send(message);
        console.log("send msg:" + sendmsg);
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


window.onload = function() {
    secretioonload();
    fileioonload();
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
        if(message.destinationName.indexOf(sspretitle) >= 0){
            /* this is secret msg */
            var 
        }

        var receivmsg = message.payloadString;
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