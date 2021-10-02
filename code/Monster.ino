
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


const int RELAY_FAN = 2;     //Pin for fan
const int RELAY_FOG = 5;     //Pin for Fog machine remote
const int PIN_LASER = 7;     // Pin for laser eyes
const int RELAY_LIGHT = 3;   //Pin for lightbulb
const int RELAY_MOTOR = 4;   //Pin for DC Motor
const int PIN_SOUND = 8;   //Pin for sound
const int PIR_PIN = 12;   // PIR Input Pin


String bt_command; //string for command from bluetooth

/*Setup times*/
unsigned long milCurrent;
unsigned long milStartFan;
unsigned long milStartFog;
unsigned long milStartLaser;
unsigned long milStartLight;
unsigned long milStartMotor;
unsigned long milStartSound;
unsigned long milScene;
unsigned long milStartScene;

/*setup run variables*/
int runFan = 0;
int runFog = 0;
int runLaser = 0;
int runLight = 0;
int runMotor = 0;
int runSound = 0;
int runScene1 = 0;
int runScene2 = 0;
int runScene3 = 0;

int pirStatus = 0;   // PIR Status
bool Motion_Mode = false; // false = phone control only; true = motion trigger on


void setup() {
  // put your setup code here, to run once:
  /*Define the type of pin*/
  pinMode(RELAY_FAN, OUTPUT); 
  pinMode(RELAY_FOG, OUTPUT);
  pinMode(PIN_LASER, OUTPUT);
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_MOTOR, OUTPUT);
  pinMode(PIR_PIN, INPUT);
   

  /*Set everything to off*/
  digitalWrite(RELAY_FAN, HIGH);    //relay is set to High for off
  digitalWrite(RELAY_FOG, HIGH);    //relay is set to High for off
  digitalWrite(PIN_LASER, LOW);     //digital pin is set to Low for off
  digitalWrite(RELAY_LIGHT, HIGH);  //relay is set to High for off
  digitalWrite(RELAY_MOTOR, HIGH);  //relay is set to High for off
   

  bt_command == " "; //assign empty string
  mySoftwareSerial.begin(9600); //serial for mp3
  Serial.begin(9600); //start the serial for bluetooth


  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(15);  //Set volume value. From 0 to 30

  randomSeed(analogRead(A0)); //get randomseed for random number

}

void loop() {
  /*check if button was pusehd from the app*/
  if(Serial.available() > 0 ){
    bt_command=Serial.read();
    Serial.println(bt_command);
    
    if(bt_command=="49"){//byte version of 1
      startFan();
      bt_command = " ";
    }

    if(bt_command=="50"){//byte version of 2
      startFog();
      bt_command = " ";
    }

    if(bt_command=="51"){//byte version of 3
      startLaser();
      bt_command = " ";
    }

    if(bt_command=="53"){//byte version of 5
      startLight();
      bt_command = " ";
    }    

    if(bt_command=="52"){//byte version of 4
      startMotor();
      bt_command = " ";
    }

    if(bt_command=="54"){//byte version of 6
      startSound();
      /*
      delay(3000);
      if (myDFPlayer.available()) {
          printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
        }*/
      bt_command = " ";
    }      

    if(bt_command=="65"){//byte version of A
      stopAll();
      bt_command = " ";
    }   

    if(bt_command=="66"){//byte version of B
      startScene1();
      bt_command = " ";
    }      

    if(bt_command=="67"){//byte version of C
      startScene2();
      bt_command = " ";
    }

    if(bt_command=="68"){//byte version of D
      startScene3();
      bt_command = " ";
    }

    if(bt_command=="69"){//byte version of E
      		Motion_Mode = !Motion_Mode;
          if(Motion_Mode == true){Serial.println("Motion mode on");}
          else{Serial.println("Motion mode on");}
      		bt_command = " ";
    }

  }



  /*run functions if active*/
  if(runFan == 1){FanSpin();}
  if(runFog == 1){FogMe();}
  if(runLaser == 1){LaserEyes();}
  if(runLight == 1){LightFlash();}
  if(runMotor == 1){LidFlap();}
  if(runSound == 1){MakeNoise();} //This isn't a trigger, we have to see how this works
  if(runScene1 == 1){scene1();
    Serial.println(millis()); 
    Serial.println(milScene); 
    Serial.println(milStartScene);
    }

  if(runScene2 == 1){scene2();}
  if(runScene3 == 1){scene3();}


/*Motion detection*/
  pirStatus = digitalRead(PIR_PIN); 
  //if(pirStatus == HIGH){Serial.println("Motion detected");}
  //if(Motion_Mode == true && pirStatus== HIGH){Serial.println("Motion mode on and motion detected");}
  if(Motion_Mode == true 
    && pirStatus == HIGH 
    && runFan == 0
    && runFog == 0
    && runLaser == 0
    && runLight == 0
    && runMotor == 0
    && runSound == 0
    && runScene1 == 0 
    && runScene2 == 0 
    && runScene3 == 0){Serial.println("Motion mode on & motion detected & nothing else running"); startScene1();}

}



/* define start functions */
void startFan(){runFan = 1; milStartFan = (millis() - 1); Serial.println("Spin that Fan");}
void startFog(){runFog = 1; milStartFog = (millis() - 1); Serial.println("Fog Me");}
void startLaser(){runLaser = 1; milStartLaser = (millis() - 1); Serial.println("Laser Eyes!");}
void startLight(){runLight = 1; milStartLight = (millis() - 1); Serial.println("There's a flasher");}
void startMotor(){runMotor = 1; milStartMotor = (millis() - 1); Serial.println("Shake that lid");}
void startSound(){runSound = 1; milStartSound = (millis() - 1); Serial.println("Make some noise!");}

void startScene1(){runScene1 = 1; runScene2 = 0; runScene3 = 0; milStartScene = (millis() - 1); Serial.println("Scene 1");}
void startScene2(){runScene1 = 0; runScene2 = 1; runScene3 = 0; milStartScene = (millis() - 1); Serial.println("Scene 2");}
void startScene3(){runScene1 = 0; runScene2 = 0; runScene3 = 1; milStartScene = (millis() - 1); Serial.println("Scene 3");}

/* define stop functions */
void stopAll(){
  runFan = 0; digitalWrite(RELAY_FAN, HIGH);
  runFog = 0; digitalWrite(RELAY_FOG, HIGH);
  runLaser = 0; digitalWrite(PIN_LASER, LOW);
  runLight = 0; digitalWrite(RELAY_LIGHT, HIGH);
  runMotor = 0; digitalWrite(RELAY_MOTOR, HIGH);
  runSound = 0; myDFPlayer.pause();
  runScene1 = 0; 
  runScene2 = 0; 
  runScene3 = 0; 
  Serial.println("Stop it all");
}




void FanSpin(){
  if((millis() - milStartFan) > 1050){digitalWrite(RELAY_FAN, HIGH); runFan = 0;} //OFF
  else if((millis() - milStartFan) > 0){digitalWrite(RELAY_FAN, LOW); } //ON
  else{digitalWrite(RELAY_FAN, HIGH);} //OFF
}

void FogMe(){
  if((millis() - milStartFog) > 500){digitalWrite(RELAY_FOG, HIGH); runFog = 0;} //OFF
  else if((millis() - milStartFog) > 0){digitalWrite(RELAY_FOG, LOW); } //ON
  else{digitalWrite(RELAY_FOG, HIGH); runFog = 0;} //OFF
}

void LaserEyes(){
  if((millis() - milStartLaser) > 5000){digitalWrite(PIN_LASER, LOW); runLaser = 0;} //OFF
  else if((millis() - milStartLaser) > 4000){digitalWrite(PIN_LASER, HIGH);} //ON
  else if((millis() - milStartLaser) > 3700){digitalWrite(PIN_LASER, LOW);} //OFF
  else if((millis() - milStartLaser) > 2800){digitalWrite(PIN_LASER, HIGH);} //ON
  else if((millis() - milStartLaser) > 2400){digitalWrite(PIN_LASER, LOW);} //OFF
  else if((millis() - milStartLaser) > 1500){digitalWrite(PIN_LASER, HIGH);} //ON
  else if((millis() - milStartLaser) > 1100){digitalWrite(PIN_LASER, LOW);} //OFF
  else if((millis() - milStartLaser) > 0){digitalWrite(PIN_LASER, HIGH); } //ON
  else{digitalWrite(PIN_LASER, LOW); runLaser = 0;} //OFF
}

void LightFlash(){
  if((millis() - milStartLight) > 3500){digitalWrite(RELAY_LIGHT, HIGH); runLight = 0;} //OFF
  else if((millis() - milStartLight) > 3350){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((millis() - milStartLight) > 3200){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((millis() - milStartLight) > 3000){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((millis() - milStartLight) > 2800){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((millis() - milStartLight) > 2450){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((millis() - milStartLight) > 2380){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((millis() - milStartLight) > 2050){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((millis() - milStartLight) > 1900){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((millis() - milStartLight) > 1750){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((millis() - milStartLight) > 1680){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((millis() - milStartLight) > 1400){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((millis() - milStartLight) > 1300){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((millis() - milStartLight) > 1100){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((millis() - milStartLight) > 1020){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((millis() - milStartLight) > 840){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((millis() - milStartLight) > 500){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((millis() - milStartLight) > 0){digitalWrite(RELAY_LIGHT, LOW); } //ON
  else{digitalWrite(RELAY_LIGHT, HIGH); runLight = 0;} //OFF
}

void LidFlap(){
  if((millis() - milStartMotor) > 100){digitalWrite(RELAY_MOTOR, HIGH); runMotor = 0;} //OFF
  else if((millis() - milStartMotor) > 0){digitalWrite(RELAY_MOTOR, LOW); } //ON
  else{digitalWrite(RELAY_MOTOR, HIGH); runMotor = 0;} //OFF
}


void MakeNoise(){
  if((millis() - milStartSound) > 6000){myDFPlayer.pause(); runSound = 0;} //Stop Sound
  else if((millis() - milStartSound) > 0){myDFPlayer.playMp3Folder(3); Serial.println("song should be playing");runSound=2;} //Play Sound runSound set to 2 so it won't keep restarting
  else{myDFPlayer.pause(); runSound = 0;} //Stop Sound
}

/*
Fan     1050
Fog     500
Lasers  5000
Light   3500
Motor   100
Sound
*/

//Sound keeps restarting, need to only start if not started
void scene1(){
  if((millis() - milStartScene) > 12000){Serial.println("Scene 1 Completed"); runScene1 = 0; stopAll(); } //OFF
  else if((millis() - milStartScene) > 10500 ){}//{Serial.println("6th pause");} // +1500
  else if((millis() - milStartScene) > 9500 ){startFan(); startMotor();}
  else if((millis() - milStartScene) > 8500 ){}//{Serial.println("5th pause");} // +1000
  else if((millis() - milStartScene) > 7500 ){startFog(); startMotor();}
  else if((millis() - milStartScene) > 6000 ){}//{Serial.println("4th pause");} // +1500
  else if((millis() - milStartScene) > 5000 ){startMotor();}
  else if((millis() - milStartScene) > 4500 ){}//{Serial.println("3rd pause");} // +500
  else if((millis() - milStartScene) > 3500 ){startMotor(); startFog(); startLaser(); startFan();} //
  else if((millis() - milStartScene) > 2500 ){}//{Serial.println("2nd pause");} // +1000
  else if((millis() - milStartScene) > 1500){startMotor();if(runSound==0){runSound=1;startSound();}else{};}
  else if((millis() - milStartScene) > 1000 ){}//{Serial.println("1st pause");} // +500
  else if((millis() - milStartScene) > 0){startFog(); startMotor(); startLight();} //Fog, Lid, Flash
  else{Serial.println("else stop"); runScene1 = 0; stopAll();} //OFF
}

void scene2(){
  if((millis() - milStartScene) > 12000){stopAll(); runScene2 = 0;} //OFF
  else if((millis() - milStartScene) > 10500 ){} // +1500
  else if((millis() - milStartScene) > 9500 ){startFan(); startMotor();  (milScene + 1000);}
  else if((millis() - milStartScene) > 8500 ){} // +1000
  else if((millis() - milStartScene) > 7500 ){startFog(); startMotor();  (milScene + 1000);}
  else if((millis() - milStartScene) > 6000 ){} // +1500
  else if((millis() - milStartScene) > 5000 ){startMotor(); (milScene + 1000);}
  else if((millis() - milStartScene) > 4500 ){} // +500
  else if((millis() - milStartScene) > 3500 ){startMotor(); startFog(); startLaser(); startFan(); (milScene + 1000);} //
  else if((millis() - milStartScene) > 2500 ){} // +1000
  else if((millis() - milStartScene) > 1500){startMotor(); (milScene + 1000);}
  else if((millis() - milStartScene) > 1000 ){} // +500
  else if((millis() - milStartScene) > 0){startFog(); startMotor(); startLight(); (milScene + 1000);} //Fog, Lid, Flash
  else{stopAll(); runScene2 = 0;} //OFF
}

void scene3(){
  if((millis() - milStartScene) > 12000){stopAll(); runScene3 = 0;} //OFF
  else if((millis() - milStartScene) > 10500 ){} // +1500
  else if((millis() - milStartScene) > 9500 ){startFan(); startMotor();  (milScene + 1000);}
  else if((millis() - milStartScene) > 8500 ){} // +1000
  else if((millis() - milStartScene) > 7500 ){startFog(); startMotor();  (milScene + 1000);}
  else if((millis() - milStartScene) > 6000 ){} // +1500
  else if((millis() - milStartScene) > 5000 ){startMotor(); (milScene + 1000);}
  else if((millis() - milStartScene) > 4500 ){} // +500
  else if((millis() - milStartScene) > 3500 ){startMotor(); startFog(); startLaser(); startFan(); (milScene + 1000);} //
  else if((millis() - milStartScene) > 2500 ){} // +1000
  else if((millis() - milStartScene) > 1500){startMotor(); (milScene + 1000);}
  else if((millis() - milStartScene) > 1000 ){} // +500
  else if((millis() - milStartScene) > 0){startFog(); startMotor(); startLight(); (milScene + 1000);} //Fog, Lid, Flash
  else{stopAll(); runScene3 = 0;} //OFF
}




void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

