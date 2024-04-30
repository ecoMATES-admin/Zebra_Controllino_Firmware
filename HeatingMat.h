#ifndef HEATING_MAT_H
#define HEATING_MAT_H

#include "Actuator.h" // Include the base Actuator class

class HeatingMat : public Actuator {
private:
    int _pin;
    bool _state;

public:
    // Constructor
    HeatingMat(int id, int pin) : Actuator(id, ActuatorType::HEATING_MAT), _pin(pin){}

    // Implement initialize method
    bool initialize() override {
        pinMode(_pin,OUTPUT);
        deactivate(); // Ensure the valve is closed initially
        return true;
    }

    // Implement activate method
    bool activate() override {
        digitalWrite(_pin, HIGH); 
        _state = true;
        return true; // In a real scenario, add checks to confirm activation
    }

    // Implement deactivate method
    bool deactivate() override {
        digitalWrite(_pin, LOW); 
        _state = false;
        return true; // In a real scenario, add checks to confirm deactivation
    }

     bool isActive(){
      return _state;
    }

    // Additional methods specific to the Valve can be added here
    // For example, methods for partial opening, emergency shut-off, etc.
};


#endif
