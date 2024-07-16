const int echoPinUltra = 4;
const int trigPinUltra = 2;
const int moRelayPin = 7;
const int moStopDis = 5 ;
const int PumpStsrtDistance = 7;
const int irSignalPin = 3;
const int gateRelaySignalPin = 9;
const long OpenGateIrRemoteValue = 4060792887;
const long CloseGateRemoteIrValue = 4060752087;
const long servrDelayTime = 20;
const int LEDpin = 5;
const int PIRsigmalPin = 6;
const int DelayTime = 300;
char value;
const int HomeLight01 = 8;
const int HomeLight02 = 10;
const int HomeLight03 = 11;
const int HomeLight04 = 12;
const int HomeLight05 = 13;
const int RainDropA0 = A0;
const int RainDropSignalPin = A1;
const int RainWarerSensitivity = 250;


#include<IRremote.h>
IRrecv irObj(irSignalPin);
decode_results results;
#include <Servo.h>
Servo servoMotor;

void setup() {
  //waterPump
  pinMode(trigPinUltra, OUTPUT);
  pinMode(echoPinUltra, INPUT);
  pinMode(moRelayPin, OUTPUT);
  Serial.begin(9600);

  //IrGate
  irObj.enableIRIn();
  servoMotor.attach(gateRelaySignalPin);
  pinMode(gateRelaySignalPin, OUTPUT);
  servoMotor.write(0);

  //securitySystemPIRsensor
  pinMode(PIRsigmalPin, INPUT);     
  pinMode(LEDpin, OUTPUT);   

  //HomeLightContBT
  pinMode(HomeLight01, OUTPUT);
  pinMode(HomeLight02, OUTPUT);
  pinMode(HomeLight03, OUTPUT);
  pinMode(HomeLight04, OUTPUT);
  pinMode(HomeLight05, OUTPUT);

  //RainRoof
  pinMode(RainDropSignalPin, OUTPUT);
}


void loop() {

//////////////////////////// 
  IrGate();
  waterPump();
  securitySystemPIRsensor();
  homeLightContBT(); 
  rainRoof();
////////////////////////////
}



//IrGate
void waterPump() {
  if(irObj.decode(&results)){
    Serial.println(results.value, DEC);
    irObj.resume();
  }
  delay (80);
  long readValue = results.value;
  if (readValue == OpenGateIrRemoteValue){
    servoMotor.write(90);
  }
  else if(readValue == CloseGateRemoteIrValue){
    servoMotor.write(0);
  }
}

//waterPump
void IrGate() {
  digitalWrite(trigPinUltra, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinUltra, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPinUltra, LOW);
  
  int distanceCM = (pulseIn(echoPinUltra, HIGH)) / 29 / 2;

  Serial.print("Distance : ");
  Serial.print(distanceCM);
  Serial.println(" cm");
  if (distanceCM > PumpStsrtDistance) {
    digitalWrite(moRelayPin, HIGH);
    }
  else if(distanceCM < moStopDis) {
    digitalWrite(moRelayPin, LOW);
    }
  delay(100); // change the value 300 to 100
}

//securitySystemPIRsensor
void securitySystemPIRsensor(){
  int val = digitalRead(PIRsigmalPin);
  if(val == 1){
    digitalWrite(LEDpin, HIGH);
    /*
    for(int i = 0; i<15; i++){
      digitalWrite(LEDpin, HIGH);
      delay(DelayTime);
      digitalWrite(LEDpin, LOW);
      delay(DelayTime);
    }
    */
  }
  else{
    digitalWrite(LEDpin, LOW);
  }
}


//HomeLightContBT
void homeLightContBT(){
  if (Serial.available()) {
    value = Serial.read();
    Serial.print("Bluetooth value : ");
    Serial.println(value);   
  }

  switch (value) {
    case '1':
      digitalWrite(HomeLight01, HIGH);
      break;
    case '2':
      digitalWrite(HomeLight01, LOW);
      break;
    case '3':
      digitalWrite(HomeLight02, HIGH);
      break;
    case '4':
      digitalWrite(HomeLight02, LOW);
      break;
    case '5':
      digitalWrite(HomeLight03, HIGH);
      break;
    case '6':
      digitalWrite(HomeLight03, LOW);
      break;
    case '7':
      digitalWrite(HomeLight04, HIGH);
      break;
    case '8':
      digitalWrite(HomeLight04, LOW);
      break;
    case '9':
      digitalWrite(HomeLight05, HIGH);
      break;
    case '0':
      digitalWrite(HomeLight05, LOW);
      break;
  }
}

//RainRoof
void rainRoof(){
  int value = analogRead(RainDropA0);
  Serial.print("RainWarerSensitivity value : ");
  Serial.println(value);

  if (value < RainWarerSensitivity) {
    digitalWrite(RainDropSignalPin, HIGH);
  } else {
    digitalWrite(RainDropSignalPin, LOW);
  }
}






