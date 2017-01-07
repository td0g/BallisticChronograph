/*
 Commands:
  ab      A to B speed test
  ba      B to A speed test
  ca / cb   Write live data to pin13
  lf      Live Sensor Feed
  p       Port Data
  t       Test trigger
  tt      Switch to / display time delay
  ttXXXX  Set time delay
  td      Switch to / display distance delay
  tdXXXX  Set distance delay
  tc      Display time delay compensation
  tcXXXX  Set time delay compensation
  to      Trigger off
  s       Set timeout in milliseconds (default 1 hour)
  z       Display current clock time

  End commands with forward slash character '/'
 */
 
#include <eRCaGuy_Timer2_Counter.h>

  //Debugging
boolean debugging = true;

  //Pins Setup
#define gatePort PINC
const byte gateA = 15;
const byte gateB = 14;
const byte triggerPin = 2;
byte pinReadValue;

  //Chronograph Values
unsigned long startGate;
unsigned long startGateB;
unsigned long stopGate;
unsigned long stopGateB;

  //Trigger Setup
boolean distanceTrigger = false;
boolean timeTrigger = false;
unsigned long triggerDelay;
unsigned long triggerTime;
unsigned long triggerComp;

  //Timeout Setup
unsigned long timeTimeout = 3600000;
unsigned long millisTimeout;

  //Serial Commander
String COMstring;
unsigned long COMvalue;
const char terminator = '/';

  //Other
const String indent = "   ";
const String dindent = "    ";



void setup() {
  timer2.setup();               //eRCGuy's Lib
  Serial.begin(9600);      
  for (byte i = 14; i < 20; i++){  //Keep all unused pins LOW
  pinMode(i, OUTPUT);
  digitalWrite(i, LOW);
  }

  pinMode(gateA, INPUT);        //Setup input and output pins
  pinMode(gateB, INPUT);
  digitalWrite(gateA, HIGH);
  digitalWrite(gateB, HIGH);
  pinMode(triggerPin, OUTPUT);

  Serial.println("Ballistic Chronograph - READY");  //Hello world message
  Serial.println();
 
  }



void loop(){
  if (getCommand()){                          //Serial Commander
  if (COMstring == "ab") speedTEST(COMvalue, 0, 1);
  else if (COMstring == "ba") speedTEST(COMvalue, 1, 0);
  else if (COMstring == "lf") liveFeed();
  else if (COMstring == "ca") LEDout(1);
  else if (COMstring == "cb") LEDout(2);
  else if (COMstring == "p") readAllPorts();
  else if (COMstring == "s") setupTimeout(COMvalue);
  else if (COMstring == "t") testTrigger();
  else if (COMstring == "tt") setupTimeTrigger(COMvalue);
  else if (COMstring == "td") setupDistanceTrigger(COMvalue);
  else if (COMstring == "tc") setupTriggerDelayComp(COMvalue);
  else if (COMstring == "to") disableTrigger();
  else if (COMstring == "z") displayClock();
  else {
    Serial.println(" Command not recognized");
    Serial.println(COMstring);
  }
  Serial.println();
  }
  if (gatePort == 3) digitalWrite(13, HIGH);
  else digitalWrite(13, LOW);
}



boolean getCommand() {
  static byte i = 0;
  static char stringChar[20];
  boolean newCommand = false;
  COMstring = "";
  COMvalue = 0;
  if (Serial.available()){
  char COMinput = Serial.read();
    
  if (COMinput == terminator) newCommand = true;
  else if (COMinput > 47){
    stringChar[i] = COMinput;
    i++;
  }
    
  if (newCommand){
    for(byte j=0; j<i; j++){
      if (stringChar[j] >= 57) COMstring += String(stringChar[j]);
      else COMvalue = COMvalue * 10 + stringChar[j] - '0';
      stringChar[j] = 0;
    }
    i=0;
  }
  }
  return newCommand;
}




void speedTEST(unsigned long temp, byte inGate, byte outGate){
unsigned long timeToCalc;

  if (temp == 0) temp = 1;
  Serial.print("Performing ");
  Serial.print(temp);
  Serial.println(" tests");
  while (temp > 0){
  Serial.println("Ready for AB test");
  pinReadValue = 255;
  while (bitRead(pinReadValue, inGate)) pinReadValue = gatePort;
    startGate = micros();
    startGateB = timer2.get_count();
    millisTimeout = millis() + timeTimeout;   //Timeout calc
    while(bitRead(pinReadValue, outGate) && (millis() < millisTimeout)) pinReadValue = gatePort;
    if (!bitRead(pinReadValue, outGate)){
      stopGate = micros();
      stopGateB = timer2.get_count();
      if (timeTrigger){
      triggerTime = stopGate + (triggerDelay - triggerComp)*2;
      while (triggerTime > micros()){};
      digitalWrite(triggerPin, HIGH);
      delay(250);
      digitalWrite(triggerPin, LOW);
    }
    if (distanceTrigger){
      triggerTime = stopGate - startGate;
      triggerTime *= triggerDelay;
      triggerTime /= 500;
      triggerTime += stopGate - triggerComp;  
      while (triggerTime > micros()){};
      digitalWrite(triggerPin, HIGH);
      delay(250);
      digitalWrite(triggerPin, LOW);
    }
    double objectSpeed = 500000;
    objectSpeed /=(stopGate - startGate);
    
    delay(200);
    digitalWrite(triggerPin, LOW);
    Serial.print(indent);
    Serial.print("Time in uS: ");
    Serial.println(stopGate - startGate);
    Serial.print(indent);
    Serial.print(indent);
    Serial.print(indent);
    Serial.print(indent);
    Serial.print(indent);
    Serial.print(indent);
    Serial.print("  Speed in m/s: ");
    Serial.print(objectSpeed);
    objectSpeed *= 3.2808;
    Serial.print(indent);
    Serial.print("  (");
    Serial.print(objectSpeed);
    Serial.println(" fps)");
   
    double objectSpeedB = 1000000;
    objectSpeedB /= (stopGateB - startGateB);

    Serial.print(indent);
    Serial.print("Speed in m/s (using eRCGuy Lib): ");
    Serial.println(objectSpeedB);
    if (distanceTrigger){
      Serial.print(indent);
      Serial.print("Delay in uS: ");
      Serial.println(triggerTime - stopGate);
    }
    if (debugging){
      Serial.print(indent);
      Serial.println("DEBUGGING");
      Serial.print(dindent);
      Serial.print("startGate: ");
      Serial.println(startGate);
      Serial.print(dindent);
      Serial.print("stopGate: ");
      Serial.println(stopGate);
      Serial.print(dindent);
      Serial.print("  dif: ");
      Serial.println(stopGate - startGate);
      Serial.print(dindent);
      Serial.print("startGateB: ");
      Serial.println(startGateB);
      Serial.print(dindent);
      Serial.print("stopGateB: ");
      Serial.println(stopGateB);
      Serial.print(dindent);
      Serial.print("  dif: ");
      Serial.println(stopGateB - startGateB);
    }
    temp--;
  }
  }
}



void LEDout(byte temp){
  Serial.print("Output Gate ");
  byte readPin;
  if (temp == 1) {
  readPin = gateA;
  Serial.print("A");
  }
  else {
  readPin = gateB;
  Serial.print("B");
  }
  Serial.println(" to Onboard LED");
  Serial.println("Send '/' character to end");
  unsigned long endLiveFeed = millis() + 10000;
  delay(100);
  while (Serial.available()) Serial.read();
  while (!Serial.available()){
  digitalWrite(13, digitalRead(readPin));
  }
  Serial.read();
  Serial.println("End of live feed");
  Serial.println();
  digitalWrite(13, HIGH);
}



void testTrigger(){
  digitalWrite(triggerPin, HIGH);
  Serial.print(indent);
  Serial.println("Flash test");
  delay(500);
  digitalWrite(triggerPin, LOW);
}



void liveFeed(){
  unsigned long endLiveFeed = millis() + 10000;
  unsigned long nextPrint = millis()+100;
  boolean Ahigh = false;
  boolean Bhigh = false;
  boolean Alow = false;
  boolean Blow = false;
  boolean Afail = false;
  boolean Bfail = false;
  while (millis() < endLiveFeed){
  while (millis() < nextPrint){
    if (PINB) Ahigh = true;
    else Alow = true;
    if (PINC) Bhigh = true;
    else Blow = true;
  }
  Serial.print(Ahigh);
  Serial.print(" Ahigh / Alow ");
  Serial.print(Alow);
  Serial.print(dindent);
  Serial.print("  ");
  Serial.print(Bhigh);
  Serial.print(" Bhigh / Blow ");
  Serial.println(Blow);
  if(Alow) Afail = true;
  if (Blow) Bfail = true;
  Ahigh = false;
  Alow = false;
  Bhigh = false;
  Blow = false;
  nextPrint += 500;
  }
  Serial.println("End of live feed");
  Serial.println();
  Serial.print("Gate A ");
  if (Afail)  Serial.println("FAIL");
  else Serial.println("PASS");
  Serial.print("Gate B ");
  if (Bfail) Serial.println("FAIL");
  else Serial.println("PASS");
}



void readAllPorts(){
  Serial.print("Port B: ");
  Serial.println(PINB);
  Serial.print("Port C: ");
  Serial.println(PINC);
  Serial.print("Port D: ");
  Serial.println(PIND);
}



void setupDistanceTrigger(unsigned long temp){
  triggerDelay = temp;
  distanceTrigger = true;
  timeTrigger = false;
  Serial.print("Trigger - Distance delay: ");
  Serial.print(triggerDelay);
  Serial.println(" mm");
}



void setupTimeTrigger(unsigned long temp){
  triggerDelay = temp;
  distanceTrigger = false;
  timeTrigger = true;
  Serial.print("Trigger - Time delay: ");
  Serial.print(triggerDelay);
  Serial.println(" uS");
}



void setupTriggerDelayComp(unsigned long temp){
  triggerComp = temp;
  Serial.print("Trigger - Delay compensation: ");
  Serial.print(triggerComp);
  Serial.println(" uS");
}



void disableTrigger(){
  distanceTrigger = false;
  timeTrigger = false;
  Serial.println("Trigger - DISABLED");
}



void setupTimeout(unsigned long temp){
  if (temp){
  timeTimeout = temp;
  Serial.print("New timeout time (ms): ");
  Serial.print(timeTimeout);
}
  else {
  Serial.print("Current timeout set at: ");
  Serial.print(timeTimeout);
  }
  Serial.print(" ms       =");
  double tempa = 500;
  tempa /= timeTimeout;
  Serial.print(tempa);
  Serial.println(" m/s MINIMUM");
}



void displayClock(){
  unsigned long cou = timer2.get_count();
  unsigned long mic = micros();
  unsigned long mil = millis();
  Serial.println("Current time:");
  Serial.print(indent);
  Serial.print("milliseconds: ");
  Serial.println(mil);
  Serial.print(indent);
  Serial.print("microseconds: ");
  Serial.println(mic);
  Serial.print(indent);
  Serial.print("timer2 count: ");
  Serial.println(cou);
}

