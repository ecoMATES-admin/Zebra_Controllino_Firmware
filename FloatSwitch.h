#ifndef FLOAT_SWITCH_H
#define FLOAT_SWITCH_H

#include "Sensor.h" // Ensure this path is correct


class FloatSwitch: public Sensor {
private:
    int pin;
    bool lastReading;

public:
    FloatSwitch(int id, int sensorPin)
        : Sensor(id, SensorType::FLOAT_SWITCH), pin(sensorPin), lastReading(false) {}

    bool initialize() override {
        pinMode(pin,INPUT);
        _status = SensorStatus::OK;
        return true;
    }

    double readValue() override {
        bool lastReading = digitalRead(pin);;
  
        //software debounce required?
        
        return static_cast<double>(lastReading);
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
