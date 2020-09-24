Object.defineProperty(exports, '__esModule', { value: true });

//const reactNativeStorage = require('react-native-storage');
//import reactNativeStorage from '@react-native-community/async-storage';
//const reactNativeStorage = require('react-native-storage');
//const reactNativeStorage = require('@react-native-community/async-storage');
import Storage from 'react-native-storage';
require('./mqttws31');

/**
 * see https://github.com/sunnylqm/react-native-storage for more details
 * @param options {object}
 */

function init(options) {
  //localStorage = new reactNativeStorage.default(options);
  localStorage = new Storage(options);
}

exports.default = init;
