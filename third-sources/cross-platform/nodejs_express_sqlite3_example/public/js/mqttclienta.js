var mqttsrvip = "171.221.141.157";
var mqttsrvport = 1883;
var mqttsrvpath = "/";
var mqttsrvsec = false;
var client;
var last_subscribe = null;
var rsakeypm = "i3rqu-e5d291db-b61b-44de-9ad9-RSA256-key";
var rsakeyval = "";
var rsa = forge.pki.rsa;
var pki = forge.pki;
var keypempair = {};
// Called when the connection is made
function onConnect() {
    console.log("Connected!");
    var mqttPInfo = document.getElementById("mqtt-client-in");
    mqttPInfo.style.visibility = "visible";
    subscribeMqttTopic(rsakeypm);
    if (last_subscribe) subscribeMqttTopic(last_subscribe);
}

function subscribeMqttTopic(thisTopic) {
    var subscribeOptions = {
        qos: 0
    };
    client.subscribe(thisTopic, subscribeOptions);
    console.log("subscribe:" + thisTopic);
}

function sendmqttmsga() {
    var sendmsg = document.getElementById('mqttmessage_id').value;
    var sendtopic = document.getElementById('mqtttitle_id').value;
    if (sendmsg && sendtopic) {
        console.log("pub:" + keypempair.publicKey);
        var publicKey = pki.publicKeyFromPem(keypempair.publicKey);
        var encrypted = publicKey.encrypt(sendmsg);
        console.log("enc:" + forge.util.encode64(encrypted));
        var privateKey = pki.decryptRsaPrivateKey(keypempair.privateKey, 'password');
        var decrypted = privateKey.decrypt(encrypted);
        console.log("dec:" + decrypted);
        var message = new Paho.MQTT.Message(sendmsg);
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
    var mqttclient_login_account = document.getElementById('client-account-login-id').textContent;
    //console.log("mqtt acount:" + mqttclient_login_account);
    //console.log("mqtt acountn leng:" + mqttclient_login_account.length);
    var ss = mqttclient_login_account.replace(/ /g, '');
    ss = ss.replace(/\r\n/g, '');
    ss = ss.replace(/\r/g, '');
    ss = ss.replace(/\n/g, '');
    console.log("mqtt acount:" + ss + " len:" + ss.length);

    try {
        rsakeyval = window.localStorage.getItem(rsakeypm + ss);
    } catch (e) {

    }
    if (rsakeyval) {
        console.log("ras key found!");
        keypempair = JSON.parse(rsakeyval);
    } else {
        console.log("rsa key create!");
        var keypair = rsa.generateKeyPair({ bits: 2048, e: 0x10001 });

        keypempair.privateKey = pki.encryptRsaPrivateKey(keypair.privateKey, 'password');
        keypempair.publicKey = pki.publicKeyToPem(keypair.publicKey);
        try {
            window.localStorage.setItem(rsakeypm + ss, JSON.stringify(keypempair));
        } catch (e) {

        }
    }



    client = new Paho.MQTT.Client(mqttsrvip, mqttsrvport, mqttsrvpath, ss);

    // set callback handlers
    client.onConnectionLost = function(responseObject) {
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


        document.getElementById('mqttmessage_receive_id').textContent = message.payloadString;
        document.getElementById('mqtttopic_receive_id').textContent = message.destinationName;

    }

    client.connect({
        cleanSession: true,
        useSSL: mqttsrvsec,
        onSuccess: onConnect
    });
};