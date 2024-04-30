#ifndef SENSORS_H
#define SENSORS_H

#include <Indio.h>

enum class SensorStatus {
    OK,
    ERROR,
    UNINITIALIZED
};

enum class SensorType {
    INDUCTIVE_PROXIMITY,
    ULTRASONIC,
    FLOAT_SWITCH
};

class Sensor {
protected:
    const int sensorID;
    SensorType sensorType;
    SensorStatus status;

public:
    Sensor(int id, SensorType type) : sensorID(id), sensorType(type), status(SensorStatus::UNINITIALIZED) {}

    virtual ~Sensor() {}

    virtual bool initialize() = 0;
    virtual double readData() = 0;
    virtual bool reset() = 0;

    SensorStatus getStatus() const {
        return status;
    }

    int getID() const {
      return sensorID;
    }
    
    // New Method: Self-Test
    virtual bool performSelfTest() = 0;

    // New Method: Update Status
    virtual void updateStatus(SensorStatus newStatus) {
        status = newStatus;
    }
};


#endif
