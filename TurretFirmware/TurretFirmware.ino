
#include <AccelStepper.h>

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        
boolean newData = false;

//Stored Values from serial
int xInt = 0;
int yInt = 0;
int solenoidState = 0;
boolean solenoidEnable = false;

//Stepper motor control pins
const int tStep = 54;
const int tDir = 55;
const int tEnable = 38;
const int pStep = 60;
const int pDir = 61;
const int pEnable = 56;

//Stepper MS Pins
const int tMS1 = 5;
const int tMS2 = 6;
const int pMS1 = 59;
const int pMS2 = 58;

//Solenoid Pin
const int sPin = 8; //Wired to heatbed pin

//create stepper objects
AccelStepper tStepper(1, tStep, tDir);
AccelStepper pStepper(1, pStep, pDir);
//============

void setup() {
  Serial.begin(250000); //Setup serial
  initializeComponents();
}

//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        processData();
        maybeShootTurret();
        newData = false;
    }
    runSteppers();
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index
 
    strtokIndx = strtok(tempChars,","); // this continues where the previous call left off
    xInt = atoi(strtokIndx);     // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    yInt = atoi(strtokIndx);     // convert this part to a float

    strtokIndx = strtok(NULL, ",");
    solenoidState = atoi(strtokIndx);

}

//============

void processData() {
  xInt = (xInt-200)*-10;
  yInt = (yInt-200)*6;
  if(solenoidState == 1){
    solenoidEnable = true;
    }
  else if(solenoidState == 0){
    solenoidEnable = false;
    }
}

void initializeComponents() {
  pinMode(sPin, OUTPUT); //Solenoid Pin
  pinMode(tEnable, OUTPUT);
  pinMode(pEnable, OUTPUT);
  pinMode(tMS1, OUTPUT);
  pinMode(tMS2, OUTPUT);
  pinMode(pMS1, OUTPUT);
  pinMode(pMS2, OUTPUT);

  //Enable Steppers
  digitalWrite(tEnable, LOW);
  digitalWrite(pEnable, LOW);

  //Configure for 1/4 microstepping
  digitalWrite(tMS1, LOW);
  digitalWrite(tMS2, HIGH);
  digitalWrite(pMS1, LOW);
  digitalWrite(pMS2, HIGH);

  //set stepper speeds and accel
  tStepper.setMaxSpeed(5000);
  pStepper.setMaxSpeed(2000);
  tStepper.setAcceleration(4000);
  pStepper.setAcceleration(4000);

}

void runSteppers(){
  pStepper.moveTo(yInt);
  tStepper.moveTo(xInt);
  tStepper.run();
  pStepper.run();
  
}

void maybeShootTurret(){
  if(solenoidEnable == true){
    digitalWrite(sPin, HIGH);
    Serial.println("shooting");
  }
  if(solenoidEnable == false){
    digitalWrite(sPin, LOW);
    Serial.println("off");
  }
}
  
