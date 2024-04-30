#ifndef MQTTCLIENT_CONTROLLER_H
#define MQTTCLIENT_CONTROLLER_H

#include "FloatSwitch.h"
#include "Pump.h"
#include "Valve.h"

//#define ARDUINOJSON_ENABLE_PROGMEM 0
#include <ArduinoJson.h>

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

class MqttClientController {
  private:
    enum class State {
      HW_START,
      CONNECT,
      WAIT_CONNECT,
      IDLE,
      PUBLISH,
    };

    State _currentState;

    unsigned long _previousTime;
    unsigned long _systemPeriod;

    long _timer = 0;
    unsigned long _publishInterval = 10; //in sec

    Pump & _pump;
    Valve & _valveReservoir;
    Valve & _valveHyg;
    FloatSwitch & _floatSwitch;
    PubSubClient & _client;
    JsonDocument _doc;

    char _output[100];
    //size_t _outputCapacity;

  public:
    // Constructor
    MqttClientController(PubSubClient &client, JsonDocument doc, Pump &p, Valve &vR, Valve &vH, FloatSwitch &floatS, unsigned long cInterval)
      : _client(client), _doc(doc), _pump(p), _valveReservoir(vR), _valveHyg(vH), _floatSwitch(floatS), _systemPeriod(cInterval), _previousTime(0), _currentState(State::CONNECT) {
    }

    // Method to update the control state
    void run() {
      unsigned long currentTime = millis();
      if ( currentTime - _previousTime >= _systemPeriod ) {
        _previousTime = currentTime;
        _timer++;
        //Serial.println(static_cast<int>(_currentState));
        //Serial.println(static_cast<int>(_timer));
        switch (_currentState) {
          case State::HW_START: //skipped for now, since there is a 2sec delay at the end of setup()
            if (_timer >= 20) {
              _currentState = State::CONNECT;
              _timer = 0;
            }
            break;
          case State::CONNECT:
            if (_client.connect("c1", "conClient", "1234")) {
              Serial.println("connected");
              // Once connected, publish an announcement...
              //_client.publish("cont/online", "Hi bitches, I am back");
              // ... and resubscribe
              _client.subscribe("ind/SensorData");

              _currentState = State::IDLE;
              _timer = 0;
            } else {
              Serial.print("failed, rc=");
              Serial.print(_client.state());
              Serial.println(" try again in 5 seconds");

              _currentState = State::WAIT_CONNECT;
              _timer = 0;
            }
            break;
          case State::WAIT_CONNECT:
            if (_timer >= 50) {
              _currentState = State::CONNECT;
              _timer = 0;
            }
            break;
          case State::IDLE:
            if (!_client.connected()) {
              _currentState = State::CONNECT;
              _timer = 0;
              break;
            } else {
              _client.loop();
            }
            if (_timer >= _publishInterval * 1000 / _systemPeriod) {
              _currentState = State::PUBLISH;
              _timer = 0;
            }
            break;
          case State::PUBLISH:
            _client.publish("cont/sensorData", "{\"Sensor1\": 40.55, \"TSensor2\": 20.33 , \"Sensor3\": 65.23, \"Sensor4\": 25.45}");
            _client.publish("cont/actuatorData", "{\"Actuator1\": 1, \"Actuator2\": 45 , \"Actuator3\": 1, \"Actuator4\": 0}");
            _client.publish("cont/watchdogData", "{\"WD1\": 0, \"WD2\": 10 , \"WD3\": 0, \"WD4\": 120, \"WD5\": 0, \"WD6\": 10 , \"WD7\": 0, \"WD8\": 120}");/*
            _doc["tempFermenter"] = _tempFermenter.getSensorValue();
            _doc["tempReservoir"] = _tempReservoir.getSensorValue();
            _doc["hydPressure"] = _pressure.getSensorValue();
            _doc["weight"] = _weight.getSensorValue();

            serializeJson(_doc, _output);
            _client.publish("ind/SensorData", _output);
            Serial.println(_output);*/

            _currentState = State::IDLE;
            _timer = 0;
            break;
          default:
            Serial.println("Error: mqttClientController");
        }
      }
    }

  private:

    // Additional control methods and logic can be added here
};


#endif
