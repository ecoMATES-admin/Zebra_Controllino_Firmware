#ifndef SENSORS_H
#define SENSORS_H

enum class SensorStatus {
    OK,
    ERROR,
    UNINITIALIZED
};

enum class SensorType {
    INDUCTIVE_PROXIMITY,
    ULTRASONIC,
    FLOAT_SWITCH,
    HALL_SENSOR
};

class Sensor {
protected:
    const int _sensorId;
    SensorType _sensorType;
    SensorStatus _status;

public:
    Sensor(int id, SensorType type) : _sensorId(id), _sensorType(type), _status(SensorStatus::UNINITIALIZED) {}

    virtual ~Sensor() {}

    virtual bool initialize() = 0;
    virtual double readValue() = 0;
    virtual bool reset() = 0;

    SensorStatus get_status() const {
        return _status;
    }

    int getId() const {
      return _sensorId;
    }
    
    // New Method: Self-Test
    virtual bool performSelfTest() = 0;

    // New Method: Update _status
    virtual void update_status(SensorStatus new_status) {
        _status = new_status;
    }
};


#endif
