#include <Controllino.h>
#include "FloatSwitch.h"
#include "Valve.h"
#include "Pump.h"
#include "ReservoirController.h"
#include "MqttClientController.h"

//#define ARDUINOJSON_ENABLE_PROGMEM 0
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

FloatSwitch floatSwitch(11, FLOAT_SWITCH);

Valve valveReservoir(2, VALVE_RES_OPEN, VALVE_RES_CLOSE);
Valve valveHyg(3, VALVE_HYG_OPEN, VALVE_HYG_CLOSE);
Pump pump(1, PUMP_CONTROL, PUMP_FORWARD);

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 12);
IPAddress server(192, 168, 1, 11);
EthernetClient ethClient;
PubSubClient client(ethClient);
JsonDocument doc;

MqttClientController mqttClientCtrl(client, doc, pump, valveReservoir, valveHyg, floatSwitch, systemPeriod);

ReservoirController reservoirController(pump, valveReservoir, valveHyg, floatSwitch, systemPeriod);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup()
{
  Serial.begin(9600);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(2000);

}

void loop()
{
  mqttClientCtrl.run();
  //reservoirController.firstTest();
  //valveHyg.deactivate();
  //valveReservoir.activate();

//Serial.println(floatSwitch.readData());
//delay(500);

  /*
  uint32_t measurementResult = analogRead(CONTROLLINO_AI0);
  uint32_t motorRun = digitalRead(CONTROLLINO_DI3);

  //calculate and set run and speed of motor
  uint32_t pwmValue = measurementResult/4;
  analogWrite(PUMP_FORWARD, pwmValue);
  digitalWrite(PUMP_CONTROL, motorRun);
  
  //Print Motor Status
  if(motorRun){
    Serial.print("ON");
  }else{
    Serial.print("OFF");
  }
  Serial.print("\t");
  Serial.print(pwmValue*100/256);
  Serial.println("%");
  */
}
