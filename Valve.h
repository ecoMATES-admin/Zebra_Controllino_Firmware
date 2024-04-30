#ifndef VALVE_H
#define VALVE_H

#include "Actuator.h" // Include the base Actuator class

class Valve : public Actuator {
private:
    int openPin;
    int closePin;
    bool state;

public:
    // Constructor
    Valve(int id, int openP, int closeP) : Actuator(id, ActuatorType::VALVE), openPin(openP), closePin(closeP) {}

    // Implement initialize method
    bool initialize() override {
        pinMode(openPin,OUTPUT);
        pinMode(closePin,OUTPUT); // Set the control pins as outputs
        deactivate(); // Ensure the valve is closed initially
        return true;
    }

    // Implement activate method
    bool activate() override {
        digitalWrite(openPin, HIGH); 
        digitalWrite(closePin, LOW); // Open the valve
        state = true;
        return true; // In a real scenario, add checks to confirm activation
    }

    // Implement deactivate method
    bool deactivate() override {
        digitalWrite(openPin, LOW); 
        digitalWrite(closePin, HIGH); // Close the valve
        state = false;
        return true; // In a real scenario, add checks to confirm deactivation
    }

     bool isActive(){
      return state;
    }

    // Additional methods specific to the Valve can be added here
    // For example, methods for partial opening, emergency shut-off, etc.
};


#endif
