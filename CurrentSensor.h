#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H

#include "Sensor.h" // Ensure this path is correct
#include "ACS712.h"


class CurrentSensor: public Sensor {
  private:
    enum class State {
      IDLE,
      SAMPLE
    };

    State _currentState;

    unsigned long _timer = 0;
    unsigned long _sampleInterval = 2; //in sec
    unsigned long _systemPeriod;
    unsigned long _previousTime;

    int _pin;
    uint32_t _sensorValue;
    ACS712 & _acs;

  public:
    CurrentSensor(int id, int sensor_pin, ACS712 & acs, unsigned long sysPeriod)
      : Sensor(id, SensorType::HALL_SENSOR), _pin(sensor_pin), _acs(acs), _systemPeriod(sysPeriod) {}

    bool initialize() override {
      //_pinMode(_pin,INPUT)
      _acs.autoMidPoint();
      if (true) {
        Serial.print("MidPoint: ");
        Serial.print(_acs.getMidPoint());
        Serial.print(". Noise mV: ");
        Serial.println(_acs.getNoisemV());
      }
      _status = SensorStatus::OK;
      return true;
    }

    void update() {
      unsigned long currentTime = millis();
      if ( currentTime - _previousTime >= _systemPeriod ) {
        _previousTime = currentTime;
        _timer++;
        //SerialUSB.println(static_cast<int>(currentState));
        switch (_currentState) {
          case State::IDLE:
            if (_timer >= _sampleInterval * 1000 / _systemPeriod) {
              _currentState = State::SAMPLE;
              _timer = 0;
            }
            break;
          case State::SAMPLE:
            _sensorValue = readValue();
            _currentState = State::IDLE;
            _timer = 0;
            break;
          default:
            Serial.print("Error: Loadcell - Sensor ID ");
            Serial.println(getId());
        }
      }
    }

    uint32_t getSensorValue() {
      return _sensorValue;
    }

    double readValue() override {
      return _acs.mA_AC();
    }

    bool reset() override {
      return initialize();
    }

    bool performSelfTest() override {
      // Implement self-test logic here.
      // For example, check if the sensor responds to a known target within a specific range.
      // Return true if test is successful, false otherwise.
    }

    // Additional method to detect electrical noise or interference
    bool isNoisy() {
      // Implement logic to detect noise.
      // For example, check if the readings are fluctuating more than a threshold within a short time.
      // Return true if noisy, false otherwise.
    }
};



#endif
