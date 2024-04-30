#ifndef IND_SENSORS_H
#define IND_SENSORS_H


class IndSensors {
  private:
    double _tempFermenter; //ID1
    double _tempReservoir; //ID2
    double _hydPressure; //ID3
    double _weight; //ID4

  public:
    // Constructor
    IndSensors(){}

    bool setSensorValue(double value, int id){
      if(id == 1){
        _tempFermenter = value;
        return true;
      }else if(id == 2){
        _tempReservoir = value;
        return true;
      }else if(id == 3){
        _hydPressure = value;
        return true;
      }else if(id == 4){
        _weight = value;
        return true;
      }else{
        return false;
      } 
    }
    int getTempFermenterInt(){
      return static_cast<int16_t>(_tempFermenter*100);
    }

};


#endif
