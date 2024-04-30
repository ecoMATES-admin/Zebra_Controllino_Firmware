#ifndef ACTUATOR_H
#define ACTUATOR_H

enum class ActuatorType {
    PUMP,
    VALVE,
    HEATING_MAT
    // Add more types as needed
};

class Actuator {
protected:
    int actuatorID;
    ActuatorType actuatorType;

public:
    // Constructor
    Actuator(int id, ActuatorType type) : actuatorID(id), actuatorType(type) {}

    // Virtual destructor
    virtual ~Actuator() {}

    // Pure virtual functions
    virtual bool initialize() = 0;
    virtual bool isActive()= 0;
    virtual bool activate() = 0;
    virtual bool deactivate() = 0;

    // Additional actuator-specific methods can be added here
};


#endif
