#ifndef FERMENTER_CONTROLLER_H
#define FERMENTER_CONTROLLER_H

#include "HeatingMat.h"
#include "IndSensors.h"

class FermenterController {
  private:
    enum class State {
      MAT_ON,
      MAT_OFF
    };

    unsigned long previousTime;
    unsigned long systemPeriod;

    IndSensors & _indSensors;
    HeatingMat & _heatingMat;
    State currentState;

    const int16_t targetFermenterTemp = 4000;
    const int16_t threshold = 30;
    const int16_t fermenterTempMax = targetFermenterTemp + threshold;
    const int16_t fermenterTempMin = targetFermenterTemp - threshold;

  public:
    // Constructor
    FermenterController(IndSensors &iS, HeatingMat &hM, unsigned long sysPeriod)
      : _indSensors(iS), _heatingMat(hM), systemPeriod(sysPeriod), previousTime(0), currentState(State::MAT_OFF) {}

    // Method to update the control state
    void run() {
      unsigned long currentTime = millis();
      if ( currentTime - previousTime >= systemPeriod ) {
        previousTime = currentTime;

        switch (currentState) {
          case State::MAT_OFF:
            _heatingMat.deactivate();
            if (_indSensors.getTempFermenterInt() < fermenterTempMin) {
              currentState = State::MAT_ON;
            }
            break;
          case State::MAT_ON:
            _heatingMat.activate();
            if (_indSensors.getTempFermenterInt() > fermenterTempMax) {
              currentState = State::MAT_OFF;
            }
            break;
          default:
            Serial.println("Error: FermenterController()");

        }



      }
    }

  private:

    // Additional control methods and logic can be added here
};


#endif
