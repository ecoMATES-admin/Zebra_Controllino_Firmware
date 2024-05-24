#ifndef RESERVOIR_CONTROLLER_H
#define RESERVOIR_CONTROLLER_H

#include "FloatSwitch.h"
#include "Pump.h"
#include "Valve.h"
#include "PumpScheduler.h"

class ReservoirController {
  private:
    enum class State {
      SETUP,
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
    PumpScheduler &pumpScheduler;

    State currentState;

    uint32_t timerStartidle = 200; //entspricht 10sec
    uint32_t timerMixing = 300;
    uint32_t timerSterilization = 100;
    uint32_t timerDrain = 250;

    uint8_t _pumpInterval = 2; // values between [2-24]
    uint8_t _currentHour;
    uint8_t _pumpHour;


  public:
    // Constructor
    ReservoirController(PumpScheduler &pS, Pump &p, Valve &vR, Valve &vH, FloatSwitch &floatS, unsigned long sysPeriod)
      : pumpScheduler(pS), pump(p), valveReservoir(vR), valveHyg(vH), floatSwitch(floatS), systemPeriod(sysPeriod), previousTime(0), currentState(State::SETUP) {}

    // Method to update the control state
    void firstTest() {
      unsigned long currentTime = millis();
      if ( currentTime - previousTime >= systemPeriod ) {
        previousTime = currentTime;
        timer++;

        uint32_t potiResult = analogRead(CONTROLLINO_AI0);
        uint32_t pwmValue = potiResult / 4;

        switch (currentState) {
          case State::SETUP:
            currentState = State::IDLE;
            break;
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
            if (floatSwitch.readValue() == HIGH) {
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

    void run() {
      unsigned long currentTime = millis();
      if ( currentTime - previousTime >= systemPeriod ) {
        previousTime = currentTime;
        timer++;

        uint32_t potiResult = analogRead(CONTROLLINO_AI0);
        uint32_t pwmValue = potiResult / 4;

        switch (currentState) {
          case State::SETUP:
            _currentHour = Controllino_GetHour();
            _pumpHour = _currentHour + _pumpInterval - (_currentHour % _pumpInterval);
            pumpScheduler.setAlarm(_pumpHour, 0);
            currentState = State::IDLE;
            break;
          case State::IDLE:
            pump.deactivate();
            valveReservoir.deactivate();
            valveHyg.deactivate();
            if (pumpScheduler.getFlag()) {
              pumpScheduler.setFlag(false);
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
            if (floatSwitch.readValue() == HIGH) { //TO DO timeout implementieren
              currentState = State::HYGIENISIEREN;
              //pumpScheduler.setAlarm(Controllino_GetHour() + 1, Controllino_GetMinute() + 1);

              //test version
              pumpScheduler.setAlarm(Controllino_GetHour(), Controllino_GetMinute() + 2);
              
              timer = 0;
            }
            break;
          case State::HYGIENISIEREN:
            valveReservoir.deactivate();
            pump.deactivate();
            if (pumpScheduler.getFlag()) {
              pumpScheduler.setFlag(false);
              currentState = State::ABLASSEN;
              timer = 0;
            }
            break;
          case State::ABLASSEN:
            valveHyg.activate();
            if (timer >= timerDrain) {
              currentState = State::SETUP;
              timer = 0;
            }
            break;
        }
        if (currentState == State::PUMPEN) {
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
    }


};


#endif
