var mqttsrvip = "171.221.141.157";
var client = new Paho.MQTT.Client(mqttsrvip, Number(9007), "/", "clientId");

// set callback handlers
client.onConnectionLost = function(responseObject) {
    console.log("Connection Lost: " + responseObject.errorMessage);
}

client.onMessageArrived = function(message) {
    console.log("Message Arrived: " + message.payloadString);
}

// Called when the connection is made
function onConnect() {
    console.log("Connected!");
    var mqttPInfo = document.getElementById("mqtt-client-in");
    mqttPInfo.style.visibility = "visible";

}

function sendmqttmsga() {
    var message = new Paho.MQTT.Message(document.getElementById('mqttmessage_id').value);
    message.destinationName = document.getElementById('mqtttitle_id').value;
    client.send(message);
}


window.onload = function() {
    client.connect({
        onSuccess: onConnect
    });
};