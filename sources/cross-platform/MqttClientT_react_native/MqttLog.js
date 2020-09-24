import React, { Component } from 'react';
import init from 'react_mqtt_t';
import { StyleSheet, Text, View, SafeAreaView } from 'react-native';
import AsyncStorage from '@react-native-community/async-storage';
init({
  size: 10000,
  storageBackend: AsyncStorage,
  defaultExpires: 1000 * 3600 * 24,
  enableCache: true,
  sync: {},
});

const styles = StyleSheet.create({
  container: {
    width: '100%',
    height: '100%',
  },
});

export default class MqttLog extends Component {
  constructor(props) {
    super(props);

    //const client = new Paho.MQTT.Client('iot.eclipse.org', 443, 'uname');
    const client = new Paho.MQTT.Client('192.168.1.69', 1884,'/', 'uname');  //websocket listen port of mqtt
    client.onConnectionLost = this.onConnectionLost;
    client.onMessageArrived = this.onMessageArrived;
    client.connect({ onSuccess: this.onConnect, useSSL: false , cleanSession: true});  //true

    this.state = {
      text: ['...'],
      client,
    };
  }

  pushText = entry => {
    const { text } = this.state;
    this.setState({ text: [...text, entry] });
  };

  onConnect = () => {
    const { client } = this.state;
    client.subscribe('WORLD', {qos: 0}); //
    this.pushText('connected');
    var message = new Paho.MQTT.Message("hello,i");
    message.destinationName = "HELLOIT";
    message.qos = 0;
    client.send(message);
  };

  onConnectionLost = responseObject => {
    if (responseObject.errorCode !== 0) {
      this.pushText(`connection lost: ${responseObject.errorMessage}`);
    }
  };

  onMessageArrived = message => {
    this.pushText(`new message: ${message.payloadString}`);
  };

  render() {
    const { text } = this.state;

    return (
      <View style={styles.container}>
        <Text key="ssii">mqtt</Text>
        {text.map(entry => <Text key={entry}>{entry}</Text>)}
      </View>
    );
  }
}
