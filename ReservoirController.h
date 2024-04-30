#ifndef RESERVOIR_CONTROLLER_H
#define RESERVOIR_CONTROLLER_H

#include "FloatSwitch.h"
#include "Pump.h"
#include "Valve.h"

class ReservoirController {
  private:
    enum class State {
      IDLE,
      KREISLAUF,
      PUMPEN,
      HYGIENISIEREN,
      ABLASSEN
    };

    unsigned long previousTime;
    unsigned long systemPeriod;

    long timer = 0;

    Pump &pump;
    Valve &valveReservoir;
    Valve &valveHyg;
    FloatSwitch &floatSwitch;

    State currentState;

uint32_t timerStartidle = 200; //entspricht 10sec
uint32_t timerMixing = 300;
uint32_t timerSterilization = 100;
uint32_t timerDrain = 250; 


  public:
    // Constructor
    ReservoirController(Pump &p, Valve &vR, Valve &vH, FloatSwitch &floatS, unsigned long sysPeriod)
      : pump(p), valveReservoir(vR), valveHyg(vH), floatSwitch(floatS), systemPeriod(sysPeriod), previousTime(0), currentState(State::IDLE) {}

    // Method to update the control state
    void firstTest() {
      unsigned long currentTime = millis();
      if ( currentTime - previousTime >= systemPeriod ) {
        previousTime = currentTime;
        timer++;

        uint32_t potiResult = analogRead(CONTROLLINO_AI0);
        uint32_t pwmValue = potiResult / 4;

        switch (currentState) {
          case State::IDLE:
            pump.deactivate();
            valveReservoir.deactivate();
            valveHyg.deactivate();
            if (timer >= timerStartidle) {
              currentState = State::KREISLAUF;
              timer = 0;
            }
            break;
          case State::KREISLAUF:
            pump.pumpControl(true, pwmValue);
            if (timer >= timerMixing) {
              currentState = State::PUMPEN;
              timer = 0;
            }
            break;
          case State::PUMPEN:
            pump.pumpControl(true, pwmValue);
            valveReservoir.activate();
            if (floatSwitch.readData() == HIGH) {
              currentState = State::HYGIENISIEREN;
              timer = 0;
            }
            break;
          case State::HYGIENISIEREN:
            valveReservoir.deactivate();
            pump.deactivate();
            if (timer >= timerSterilization) {
              currentState = State::ABLASSEN;
              timer = 0;
            }
            break;
          case State::ABLASSEN:
            valveHyg.activate();
            if (timer >= timerDrain) {
              currentState = State::IDLE;
              timer = 0;
            }
            break;
        }

        Serial.print(static_cast<int>(currentState));
        Serial.print("\t");
        if (pump.isActive()) {
          Serial.print("ON");
        } else {
          Serial.print("OFF");
        }
        Serial.print("\t");
        Serial.print(pwmValue * 100 / 256);
        Serial.println("%");

      }
    }

  private:

    // Additional control methods and logic can be added here
};


#endif
