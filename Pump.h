#ifndef PUMP_H
#define PUMP_H

#include "Actuator.h" // Include the base Actuator class

class Pump : public Actuator {
  private:
    int controlPin; // Pin used to control the pump
    int forwardPin;
    int backwardPin;
    uint8_t _speed;
    bool state;



  public:
    // Constructor
    Pump(int id, int pinControl, int pinForward, int pinBackward = 99) : Actuator(id, ActuatorType::PUMP), controlPin(pinControl), forwardPin(pinForward), backwardPin(pinBackward) {}

    // Implement initialize method
    bool initialize() override {
      deactivate(); // Ensure the pump is off initially
      return true;
    }

    // Implement activate method
    bool activate() override {
      digitalWrite(controlPin, HIGH); // Turn on the pump
      state = true;
      return true; // In a real scenario, add checks to confirm activation
    }

    // Implement deactivate method
    bool deactivate() override {
      digitalWrite(controlPin, LOW); // Turn off the pump
      digitalWrite(forwardPin, LOW);
      if(backwardPin != 99){
        digitalWrite(backwardPin, LOW);
      }
      state = false;
      return true; // In a real scenario, add checks to confirm deactivation
    }

    bool pumpControl(bool forward, uint8_t speeed) {
      _speed = speeed;
      if (!forward && backwardPin != 99) {
        digitalWrite(forwardPin, LOW);
        analogWrite(backwardPin, speeed);
        activate();
      } else {
        if (backwardPin != 99){
          digitalWrite(backwardPin, LOW);
        }
        analogWrite(forwardPin, speeed);
        activate();
      }
      return forward;
    }

    bool isActive() {
      return state;
    }

    // Additional methods specific to the Pump can be added here
};

#endif
