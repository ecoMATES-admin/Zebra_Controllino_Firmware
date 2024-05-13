#include <Controllino.h>
#include <SPI.h>
#include "FloatSwitch.h"
#include "Valve.h"
#include "Pump.h"
#include "ReservoirController.h"
#include "FermenterController.h"
#include "MqttClientController.h"
#include "IndSensors.h"
#include "HeatingMat.h"
#include "PumpScheduler.h"

#include <ArduinoJson.h>

bool controller = true;
const unsigned long systemPeriod = 100; // milliseconds
const unsigned long loggingInterval = 1000;

/*Controllino*/
#define POTI CONTROLLINO_AI0
#define VALVE_RES_OPEN CONTROLLINO_DO4
#define VALVE_RES_CLOSE CONTROLLINO_DO5
#define VALVE_HYG_OPEN CONTROLLINO_DO2
#define VALVE_HYG_CLOSE CONTROLLINO_DO3
#define PUMP_CONTROL CONTROLLINO_DO7
#define PUMP_FORWARD CONTROLLINO_DO6
#define FLOAT_SWITCH CONTROLLINO_DI0
#define HEATING_MAT CONTROLLINO_R0

FloatSwitch floatSwitch(11, FLOAT_SWITCH);

Valve valveReservoir(2, VALVE_RES_OPEN, VALVE_RES_CLOSE);
Valve valveHyg(3, VALVE_HYG_OPEN, VALVE_HYG_CLOSE);
Pump pump(1, PUMP_CONTROL, PUMP_FORWARD);
HeatingMat heatingMat(4, HEATING_MAT);
IndSensors indSensors;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 12);
IPAddress server(192, 168, 1, 11);
EthernetClient ethClient;
PubSubClient client(ethClient);
JsonDocument docOut;
JsonDocument docIn;

MqttClientController mqttClientCtrl(client, docOut, pump, valveReservoir, valveHyg, floatSwitch, systemPeriod);
ReservoirController reservoirController(pump, valveReservoir, valveHyg, floatSwitch, systemPeriod);
FermenterController fermenterController(indSensors, heatingMat, systemPeriod);
PumpScheduler pumpScheduler;

/*RTC Test*/
int testState = 0;
int temp;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  DeserializationError error = deserializeJson(docIn, payload, 200);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }


  double tempFermenter = docIn["tempFermenter"];
  double tempReservoir = docIn["tempReservoir"];
  double hydPressure = docIn["hydPressure"];
  double weight = docIn["weight"];
  indSensors.setSensorValue(tempFermenter, 1);

  Serial.print("tempFermenter: ");
  Serial.println(tempFermenter);
  Serial.print("tempReservoir: ");
  Serial.println(tempReservoir);
  Serial.print("hydPressure: ");
  Serial.println(hydPressure);
  Serial.print("weight: ");
  Serial.println(weight);
}

void setup()
{
  Serial.begin(9600);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(2000);

  Controllino_RTC_init();
  Controllino_ClearAlarm();
  Controllino_PrintTimeAndDate();

  pinMode(CONTROLLINO_RTC_INTERRUPT, INPUT_PULLUP);


}

void loop()
{
    pumpScheduler.rtcAlarm();
  //sdmqttClientCtrl.run();
  //indSensors.testerFermenterController();
  //fermenterController.run();

  /*RTC Test*/
  switch (testState) {
    case 0:
      temp = Controllino_SetAlarm(14, 50);
      Serial.print("Alarm set: ");
      Serial.println(temp);
      testState++;
      break;
    case 1:
      if (pumpScheduler.getFlag()){
        pumpScheduler.setFlag(false);
        testState++;
      }
      break;
    case 2:
      temp = Controllino_SetAlarm(14, 52);
      Serial.print("Alarm set: ");
      Serial.println(temp);
      testState++;
      break;
    case 3:
      if (pumpScheduler.getFlag()){
        pumpScheduler.setFlag(false);
      }
      break;
    case 4:
      temp = Controllino_SetAlarm(14, 54);
      Serial.print("Alarm set: ");
      Serial.println(temp);
      testState++;
      break;
    case 5:
     break;

  }

}
