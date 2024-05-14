#ifndef RTC_SCHEDULER_H
#define RTC_SCHEDULER_H
#include <Controllino.h>
#include <SPI.h>

class PumpScheduler {

  private:
    bool _pumpFlag = false;
  public:
    

    bool rtcAlarm() {
      if (digitalRead(CONTROLLINO_RTC_INTERRUPT) == 0) {
        Controllino_ClearAlarm();
        Serial.print("Alarm triggered: ");
        Controllino_PrintTimeAndDate();
        _pumpFlag = true;
        return true;
      } else {
        return false;
      }
    }

    bool setAlarm(int h, int m){
      return Controllino_SetAlarm(h, m);
    }

    bool getFlag(){
      return _pumpFlag;
    }

    void setFlag(bool flag){
      _pumpFlag = flag;
    }
};

#endif
