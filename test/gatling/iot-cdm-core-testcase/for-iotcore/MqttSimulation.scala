package com.github.mnogu.gatling.mqtt.test

import com.github.mnogu.gatling.mqtt.Predef._
import io.gatling.core.Predef._
import org.fusesource.mqtt.client.QoS

import scala.concurrent.duration._

class MqttSimulation extends Simulation {
  val hpath = sys.env("GATLING_HOME")
  val  feeder = csv(hpath + "/user-files/corep.csv").circular
  val mqttConf = mqtt.host("tcp://192.168.1.231:21883").userName("${devtoken}")
  val connect = exec(mqtt("connect")
    .connect())

  val publish = repeat(600) {
    exec(mqtt("publish")
      .publish("v1/devices/me/telemetry", "{\"xtest1\":\"1\",\"xtest2\":\"2\",\"xtest3\":\"3\",\"xtest4\":\"4\",\"xtest5\":\"5\",\"xtest6\":\"6\",\"xtest7\":\"7\",\"xtest8\":\"8\",\"xtest9\":\"9\",\"xtest10\":\"10\"}", QoS.AT_LEAST_ONCE, retain = false))
      .pause(1000 milliseconds)
  }

  val disconnect = exec(mqtt("disconnect")
    .disconnect())

  val scn = scenario("MQTT Test")
    .feed(feeder)
    .exec(connect, publish, disconnect)
  

  setUp(
    scn
      .inject(rampUsers(200) during (2 seconds))
  ).protocols(mqttConf)
}
