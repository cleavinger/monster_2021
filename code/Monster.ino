
/*
If not connecting to a computer, after powering on you need to reset the board with the button to get it to work.
It appears to be an issue with the DFRobot mp3 player and SoftwareSerial.h
*/

#include "Arduino.h"
#include "SoftwareSerial.h" //need this library for mp3 module communication
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(8, 9); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
//void printDetail(uint8_t type, int value);

/* Setting up Pins*/
const int RELAY_FAN = 2;     //Pin for fan
const int RELAY_LIGHT_GREEN = 3;   //Pin for green lightbulb
const int RELAY_MOTOR = 4;   //Pin for DC Motor
const int RELAY_FOG = 5;     //Pin for Fog machine remote
const int PIN_LASER = 6;     // Pin for laser eyes
const int RELAY_LIGHT_RED = 7;   //Pin for red lightbulb


const int PIR_PIN = 10;   // PIR input Pin



String bt_command; //string for command from bluetooth

/*Setup times*/
unsigned long milCurrent;
unsigned long milStartFan;
unsigned long milStartFog;
unsigned long milStartLaser;
unsigned long milstartLightGreen;
unsigned long milstartLightRed;
unsigned long milStartMotor;
unsigned long milStartSound;
unsigned long milScene;
unsigned long milStartScene;
unsigned long milStartSleep;

/*setup run variables - lets loop know if it should trigger one of these*/
int runFan = 0;
int runFog = 0;
int runLaser = 0;
int runLightGreen = 0;
int runLightRed = 0;
int runMotor = 0;
int runSound = 0;
int runScene1 = 0;
int runScene2 = 0;
int runScene3 = 0;
int runSleep = 0;

int pirStatus = 0;   // PIR Status to read from digital pin
bool Motion_Mode = false; // false = phone control only; true = motion trigger on

int int_song = 3; //set default song

void setup() {
  // put your setup code here, to run once:
  /*Define the type of pin*/
  pinMode(RELAY_FAN, OUTPUT); 
  pinMode(RELAY_FOG, OUTPUT);
  pinMode(PIN_LASER, OUTPUT);
  pinMode(RELAY_LIGHT_GREEN, OUTPUT);
  pinMode(RELAY_LIGHT_RED, OUTPUT);
  pinMode(RELAY_MOTOR, OUTPUT);
  pinMode(PIR_PIN, INPUT);
   

  /*Set everything to off*/
  digitalWrite(RELAY_FAN, HIGH);    //relay is set to High for off
  digitalWrite(RELAY_FOG, HIGH);    //relay is set to High for off
  digitalWrite(PIN_LASER, LOW);     //digital pin is set to Low for off
  digitalWrite(RELAY_LIGHT_GREEN, HIGH);  //relay is set to High for off
  digitalWrite(RELAY_LIGHT_RED, HIGH);  //relay is set to High for off
  digitalWrite(RELAY_MOTOR, HIGH);  //relay is set to High for off
   

  bt_command == " "; //assign empty string
  mySoftwareSerial.begin(9600); //serial for mp3 module
  Serial.begin(9600); //start the serial for bluetooth


  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  /*Need to have the true false to avoid having to manually restart Arduino to get it to load with the sketch the first time
  Without this you will have to manually have access to the Arduino on every reboot - super annoying*/
  if (!myDFPlayer.begin(mySoftwareSerial, true, false)) {  //Use softwareSerial to communicate with mp3 module.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);{delay(0);}
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
    
    if(bt_command=="49"){//byte version of 1 Fan
      startFan();
      bt_command = " ";
    }

    if(bt_command=="50"){//byte version of 2 Fog
      startFog();
      bt_command = " ";
    }

    if(bt_command=="51"){//byte version of 3 Laser
      startLaser();
      bt_command = " ";
    }

    if(bt_command=="53"){//byte version of 5 Green Light
      startLightGreen();
      bt_command = " ";
    }    

    if(bt_command=="71"){//byte version of G Red Light
      startLightRed();
      bt_command = " ";
    }   

    if(bt_command=="52"){//byte version of 4 Motor
      startMotor();
      bt_command = " ";
    }

    if(bt_command=="54"){//byte version of 6 Roaring Sound
      startSound();
      bt_command = " ";
    }      

    if(bt_command=="65"){//byte version of A Stop all
      stopAll();
      bt_command = " ";
    }   

    if(bt_command=="66"){//byte version of B Scene 1
      stopAll();
      startScene1();
      bt_command = " ";
    }      

    if(bt_command=="67"){//byte version of C Scene 2
      stopAll();
      startScene2();
      bt_command = " ";
    }

    if(bt_command=="68"){//byte version of D Scene 3
      stopAll();
      startScene3();
      bt_command = " ";
    }

    if(bt_command=="69"){//byte version of E Motion
      		Motion_Mode = !Motion_Mode;
          if(Motion_Mode == true){Serial.println("Motion mode on");}
          else{Serial.println("Motion mode off");}
      		bt_command = " ";
    }

    if(bt_command=="70"){//byte version of F Sleep Mode
      stopAll();
      startSleep();
      bt_command = " ";
    }

    
    //sounds 
    if(bt_command=="72"){//byte version of H Laugh
      myDFPlayer.playMp3Folder(6); //Serial.println("Laugh should be playing");
      bt_command = " ";
    }   

    if(bt_command=="73"){//byte version of I Chains
      myDFPlayer.playMp3Folder(7); //Serial.println("Chains should be playing");
      bt_command = " ";
    }     

    if(bt_command=="74"){//byte version of J Ghosts
      myDFPlayer.playMp3Folder(8); //Serial.println("Ghosts should be playing");
      bt_command = " ";
    }   

    if(bt_command=="75"){//byte version of K Moaning
      myDFPlayer.playMp3Folder(9); //Serial.println("Moaning should be playing");
      bt_command = " ";
    }   

    if(bt_command=="76"){//byte version of L Monster Growl!
      myDFPlayer.playMp3Folder(10); //Serial.println("Monster Growl should be playing");
      bt_command = " ";
    }   

    if(bt_command=="77"){//byte version of M Dog Growl
      myDFPlayer.playMp3Folder(11); //Serial.println("Dog Growl should be playing");
      bt_command = " ";
    } 

    if(bt_command=="78"){//byte version of N Fingernails on Chalkboard
      myDFPlayer.playMp3Folder(12); //Serial.println("Fingernails on Chalkboardshould be playing");
      bt_command = " ";
    } 

    if(bt_command=="79"){//byte version of O Monkeys
      myDFPlayer.playMp3Folder(13); //Serial.println("Monkeys should be playing");
      bt_command = " ";
    } 

    if(bt_command=="80"){//byte version of P Happy Halloween
      myDFPlayer.playMp3Folder(14); //Serial.println("Happy Halloween should be playing");
      bt_command = " ";
    } 

    if(bt_command=="81"){//byte version of Q Welcome
      myDFPlayer.playMp3Folder(15); //Serial.println("Welcome should be playing");
      bt_command = " ";
    } 

    if(bt_command=="82"){//byte version of R Trick or Treat
      myDFPlayer.playMp3Folder(16); //Serial.println("Trick or Treat should be playing");
      bt_command = " ";
    } 

    if(bt_command=="83"){//byte version of S Treat it is
      myDFPlayer.playMp3Folder(17); //Serial.println("Treat it is should be playing");
      bt_command = " ";
    } 

    if(bt_command=="84"){//byte version of T I see you
      myDFPlayer.playMp3Folder(18); //Serial.println("I see you should be playing");
      bt_command = " ";
    } 

    if(bt_command=="85"){//byte version of U Bye
      myDFPlayer.playMp3Folder(19); //Serial.println("Bye should be playing");
      bt_command = " ";
    } 

    if(bt_command=="86"){//byte version of V See you later alligator
      myDFPlayer.playMp3Folder(20); //Serial.println("See you later alligator should be playing");
      bt_command = " ";
    } 

    if(bt_command=="87"){//byte version of W Come here, don't be scared
      myDFPlayer.playMp3Folder(21); //Serial.println("Come here, don't be scaredshould be playing");
      bt_command = " ";
    } 

    if(bt_command=="88"){//byte version of X Feed Me
      myDFPlayer.playMp3Folder(22); //Serial.println("Feed Me should be playing");
      bt_command = " ";
    } 

    if(bt_command=="89"){//byte version of Y Monster Mash
      myDFPlayer.playMp3Folder(23); //Serial.println("Monster Mash should be playing");
      bt_command = " ";
    } 

    if(bt_command=="90"){//byte version of Z This is Halloween
      myDFPlayer.playMp3Folder(24); //Serial.println("This is Halloween should be playing");
      bt_command = " ";
    } 

    if(bt_command=="97"){//byte version of a Toccata and Fugue in D minor
      myDFPlayer.playMp3Folder(25); //Serial.println("Toccata and Fugue in D minor should be playing");
      bt_command = " ";
    } 

    if(bt_command=="98"){//byte version of b Danse Macabre
      myDFPlayer.playMp3Folder(26); //Serial.println("Danse Macabre should be playing");
      bt_command = " ";
    } 

    if(bt_command=="99"){//byte version of c In the Hall of Mountain King
      myDFPlayer.playMp3Folder(27); //Serial.println("In the Hall of the Mountain King should be playing");
      bt_command = " ";
    } 

    if(bt_command=="48"){//byte version of 0 Night on Bald Mountain
      myDFPlayer.playMp3Folder(28); //Serial.println("Night on Bald Mountain should be playing");
      bt_command = " ";
    } 

    if(bt_command=="55"){//byte version of 7 Dies Irae
      myDFPlayer.playMp3Folder(29); //Serial.println("Dies Irae should be playing");
      bt_command = " ";
    } 

    if(bt_command=="56"){//byte version of 8 Phantom of the Opera
      myDFPlayer.playMp3Folder(30); //Serial.println("Phantom of the Opera should be playing");
      bt_command = " ";
    } 

    if(bt_command=="57"){//byte version of 9 Stranger Things
      myDFPlayer.playMp3Folder(31); //Serial.println("Stranger Things should be playing");
      bt_command = " ";
    } 

  }



  /*run functions if active*/
  if(runFan == 1){FanSpin();}
  if(runFog == 1){FogMe();}
  if(runLaser == 1){LaserEyes();}
  if(runLightGreen == 1){LightFlashGreen();}
  if(runLightRed == 1){LightFlashRed();}
  if(runMotor == 1){LidFlap();}
  if(runSound == 1){MakeNoise();} //This isn't a trigger, we have to see how this works
  if(runScene1 == 1){scene1();}
  if(runScene2 == 1){scene2();}
  if(runScene3 == 1){scene3();}
  if(runSleep == 1){Sleeper();}


/*Motion detection*/
  pirStatus = digitalRead(PIR_PIN); 
  //if(pirStatus == HIGH){Serial.println("Motion detected");}
  //if(Motion_Mode == true && pirStatus== HIGH){Serial.println("Motion mode on and motion detected");}
  if(Motion_Mode == true 
    && pirStatus == HIGH 
    && runFan == 0
    && runFog == 0
    && runLaser == 0
    && runLightGreen == 0
    && runLightRed == 0
    && runMotor == 0
    && runSound == 0
    && runScene1 == 0 
    && runScene2 == 0 
    && runScene3 == 0){Serial.println("Motion mode on & motion detected & nothing else running"); startScene1();}
  else if(Motion_Mode == true 
    && pirStatus == LOW 
    && runFan == 0
    && runFog == 0
    && runLaser == 0
    && runLightGreen == 0
    && runLightRed == 0
    && runMotor == 0
    && runSound == 0
    && runSleep == 0
    && runScene1 == 0 
    && runScene2 == 0 
    && runScene3 == 0){Serial.println("Go to sleeeeep"); startSleep();}

}



/* define start functions */
void startFan(){runFan = 1; milStartFan = (millis() - 1); Serial.println("Spin that Fan");}
void startFog(){runFog = 1; milStartFog = (millis() - 1); Serial.println("Fog Me");}
void startLaser(){runLaser = 1; milStartLaser = (millis() - 1); Serial.println("Laser Eyes!");}
void startLightGreen(){runLightGreen = 1; milstartLightGreen = (millis() - 1); Serial.println("There's a GREEN flasher");}
void startLightRed(){runLightRed = 1; milstartLightRed = (millis() - 1); Serial.println("There's a RED flasher");}
void startMotor(){runMotor = 1; milStartMotor = (millis() - 1); Serial.println("Shake that lid");}
void startSound(){runSound = 1; milStartSound = (millis() - 1); Serial.println("Make some noise!");}

void startSleep(){runSleep = 1; milStartSleep = (millis() - 1); Serial.println("Go to bed");}

void startScene1(){runScene1 = 1; runScene2 = 0; runScene3 = 0; milStartScene = (millis() - 1); Serial.println("Scene 1");}
void startScene2(){runScene1 = 0; runScene2 = 1; runScene3 = 0; milStartScene = (millis() - 1); Serial.println("Scene 2");}
void startScene3(){runScene1 = 0; runScene2 = 0; runScene3 = 1; milStartScene = (millis() - 1); Serial.println("Scene 3");}



/* define stop functions */
void stopAll(){
  runFan = 0; digitalWrite(RELAY_FAN, HIGH);
  runFog = 0; digitalWrite(RELAY_FOG, HIGH);
  runLaser = 0; digitalWrite(PIN_LASER, LOW);
  runLightGreen = 0; digitalWrite(RELAY_LIGHT_GREEN, HIGH);
  runLightRed = 0; digitalWrite(RELAY_LIGHT_RED, HIGH);
  runMotor = 0; digitalWrite(RELAY_MOTOR, HIGH);
  runSound = 0; myDFPlayer.pause();
  runScene1 = 0; 
  runScene2 = 0; 
  runScene3 = 0;
  runSleep = 0; 
  Serial.println("Stop it all");
}




void FanSpin(){
  if((millis() - milStartFan) > 1050){digitalWrite(RELAY_FAN, HIGH);} //OFF
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

void LightFlashGreen(){
  if((millis() - milstartLightGreen) > 3500){digitalWrite(RELAY_LIGHT_GREEN, HIGH); runLightGreen = 0;} //OFF
  else if((millis() - milstartLightGreen) > 3350){digitalWrite(RELAY_LIGHT_GREEN, LOW);} //ON
  else if((millis() - milstartLightGreen) > 3200){digitalWrite(RELAY_LIGHT_GREEN, HIGH);} //OFF
  else if((millis() - milstartLightGreen) > 3000){digitalWrite(RELAY_LIGHT_GREEN, LOW);} //ON
  else if((millis() - milstartLightGreen) > 2800){digitalWrite(RELAY_LIGHT_GREEN, HIGH);} //OFF
  else if((millis() - milstartLightGreen) > 2450){digitalWrite(RELAY_LIGHT_GREEN, LOW);} //ON
  else if((millis() - milstartLightGreen) > 2380){digitalWrite(RELAY_LIGHT_GREEN, HIGH);} //OFF
  else if((millis() - milstartLightGreen) > 2050){digitalWrite(RELAY_LIGHT_GREEN, LOW);} //ON
  else if((millis() - milstartLightGreen) > 1900){digitalWrite(RELAY_LIGHT_GREEN, HIGH);} //OFF
  else if((millis() - milstartLightGreen) > 1750){digitalWrite(RELAY_LIGHT_GREEN, LOW);} //ON
  else if((millis() - milstartLightGreen) > 1680){digitalWrite(RELAY_LIGHT_GREEN, HIGH);} //OFF
  else if((millis() - milstartLightGreen) > 1400){digitalWrite(RELAY_LIGHT_GREEN, LOW);} //ON
  else if((millis() - milstartLightGreen) > 1300){digitalWrite(RELAY_LIGHT_GREEN, HIGH);} //OFF
  else if((millis() - milstartLightGreen) > 1100){digitalWrite(RELAY_LIGHT_GREEN, LOW);} //ON
  else if((millis() - milstartLightGreen) > 1020){digitalWrite(RELAY_LIGHT_GREEN, HIGH);} //OFF
  else if((millis() - milstartLightGreen) > 840){digitalWrite(RELAY_LIGHT_GREEN, LOW);} //ON
  else if((millis() - milstartLightGreen) > 500){digitalWrite(RELAY_LIGHT_GREEN, HIGH);} //OFF
  else if((millis() - milstartLightGreen) > 0){digitalWrite(RELAY_LIGHT_GREEN, LOW); } //ON
  else{digitalWrite(RELAY_LIGHT_GREEN, HIGH); runLightGreen = 0;} //OFF
}

void LightFlashRed(){
  if((millis() - milstartLightRed) > 3400){digitalWrite(RELAY_LIGHT_RED, HIGH); runLightRed = 0;} //OFF
  else if((millis() - milstartLightRed) > 3250){digitalWrite(RELAY_LIGHT_RED, LOW);} //ON
  else if((millis() - milstartLightRed) > 3100){digitalWrite(RELAY_LIGHT_RED, HIGH);} //OFF
  else if((millis() - milstartLightRed) > 2920){digitalWrite(RELAY_LIGHT_RED, LOW);} //ON
  else if((millis() - milstartLightRed) > 2700){digitalWrite(RELAY_LIGHT_RED, HIGH);} //OFF
  else if((millis() - milstartLightRed) > 2540){digitalWrite(RELAY_LIGHT_RED, LOW);} //ON
  else if((millis() - milstartLightRed) > 2370){digitalWrite(RELAY_LIGHT_RED, HIGH);} //OFF
  else if((millis() - milstartLightRed) > 2150){digitalWrite(RELAY_LIGHT_RED, LOW);} //ON
  else if((millis() - milstartLightRed) > 2010){digitalWrite(RELAY_LIGHT_RED, HIGH);} //OFF
  else if((millis() - milstartLightRed) > 1850){digitalWrite(RELAY_LIGHT_RED, LOW);} //ON
  else if((millis() - milstartLightRed) > 1630){digitalWrite(RELAY_LIGHT_RED, HIGH);} //OFF
  else if((millis() - milstartLightRed) > 1450){digitalWrite(RELAY_LIGHT_RED, LOW);} //ON
  else if((millis() - milstartLightRed) > 1200){digitalWrite(RELAY_LIGHT_RED, HIGH);} //OFF
  else if((millis() - milstartLightRed) > 1080){digitalWrite(RELAY_LIGHT_RED, LOW);} //ON
  else if((millis() - milstartLightRed) > 820){digitalWrite(RELAY_LIGHT_RED, HIGH);} //OFF
  else if((millis() - milstartLightRed) > 740){digitalWrite(RELAY_LIGHT_RED, LOW);} //ON
  else if((millis() - milstartLightRed) > 200){digitalWrite(RELAY_LIGHT_RED, HIGH);} //OFF
  else if((millis() - milstartLightRed) > 0){digitalWrite(RELAY_LIGHT_RED, LOW); } //ON
  else{digitalWrite(RELAY_LIGHT_RED, HIGH); runLightRed = 0;} //OFF
}


void LidFlap(){
  if((millis() - milStartMotor) > 100){digitalWrite(RELAY_MOTOR, HIGH); runMotor = 2;} //OFF
  else if((millis() - milStartMotor) > 0){digitalWrite(RELAY_MOTOR, LOW);} //ON
  else{digitalWrite(RELAY_MOTOR, HIGH); runMotor = 0;} //OFF
}


void MakeNoise(){
  if((millis() - milStartSound) > 6000){myDFPlayer.pause(); runSound = 0;} //Stop Sound
  else if((millis() - milStartSound) > 0){myDFPlayer.playMp3Folder(int_song); Serial.println("song should be playing");runSound=2;} //Play Sound runSound set to 2 so it won't keep restarting
  else{myDFPlayer.pause(); runSound = 0;} //Stop Sound
}


void Sleeper(){
  if((millis() - milStartSleep) > 60000){Serial.println("Slept for awhile"); myDFPlayer.pause(); stopAll(); startFog(); startSleep();} //restart the sleeping sound
  else if((millis() - milStartSleep) > 0){myDFPlayer.playMp3Folder(4); Serial.println("song should be playing");runSleep = 2;} //Play Sound runSound set to 2 so it won't keep restarting
  else{Serial.println("else stop"); runSleep = 0; stopAll();} //OFF
}



/*
Creating the scenes
*/

//Sound keeps restarting, need to only start if not started
void scene1(){
  int_song = 3;
  if((millis() - milStartScene) > 12000){Serial.println("Scene 1 Completed"); runScene1 = 0; stopAll(); } //OFF
  else if((millis() - milStartScene) > 10500 ){}//{Serial.println("6th pause");} // +1500
  else if((millis() - milStartScene) > 9500 ){startFan();
                                              if(runMotor==0){runMotor=1; startMotor();} else{};}
  else if((millis() - milStartScene) > 8500 ){runMotor=0;}//{Serial.println("5th pause");} // +1000
  else if((millis() - milStartScene) > 7500 ){startFog();
                                              if(runMotor==0){runMotor=1; startMotor();} else{};}
  else if((millis() - milStartScene) > 6000 ){runMotor=0;}//{Serial.println("4th pause");} // +1500
  else if((millis() - milStartScene) > 5000 ){if(runMotor==0){runMotor=1; startMotor();} else{};}
  else if((millis() - milStartScene) > 4500 ){runMotor=0;}//{Serial.println("3rd pause");} // +500
  else if((millis() - milStartScene) > 3500 ){if(runMotor==0){runMotor=1; startMotor();} else{};
                                              startFog(); startLaser(); startFan();} //
  else if((millis() - milStartScene) > 2500 ){runMotor=0;}//{Serial.println("2nd pause");} // +1000
  else if((millis() - milStartScene) > 1500){if(runMotor==0){runMotor=1; startMotor();} else{};
                                             if(runSound==0){int_song=3;runSound=1;startSound();}else{};}
  else if((millis() - milStartScene) > 1000 ){runMotor=0;}//{Serial.println("1st pause");} // +500
  else if((millis() - milStartScene) > 0){startFog(); 
                                          if(runMotor==0){runMotor=1; startMotor();} else{};
                                          startLightGreen(); 
                                          startLightRed();} //Fog, Lid, Flash
  else{Serial.println("else stop"); runScene1 = 0; stopAll();} //OFF
}

void scene2(){
  if((millis() - milStartScene) > 12000){Serial.println("Scene 1 Completed"); runScene2 = 0; stopAll(); } //OFF
  else if((millis() - milStartScene) > 10500 ){}//{Serial.println("6th pause");} // +1500
  else if((millis() - milStartScene) > 9500 ){startLightRed(); startFan();
                                              if(runMotor==0){runMotor=1; startMotor();} else{};}
  else if((millis() - milStartScene) > 8500 ){runMotor=0;}//{Serial.println("5th pause");} // +1000
  else if((millis() - milStartScene) > 7500 ){startLightRed(); startFog();
                                              if(runMotor==0){runMotor=1; startMotor();} else{};}
  else if((millis() - milStartScene) > 6000 ){runMotor=0;}//{Serial.println("4th pause");} // +1500
  else if((millis() - milStartScene) > 5000 ){startLightRed(); startFog(); startFan();}
  else if((millis() - milStartScene) > 4500 ){runMotor=0;}//{Serial.println("3rd pause");} // +500
  else if((millis() - milStartScene) > 3500 ){if(runMotor==0){runMotor=1; startMotor();} else{};
                                              startFog(); startLaser(); startFan();} //
  else if((millis() - milStartScene) > 2500 ){runMotor=0;}//{Serial.println("2nd pause");} // +1000
  else if((millis() - milStartScene) > 1500){startLightRed();startLaser();
                                             if(runSound==0){int_song=10;runSound=1;startSound();}else{};}
  else if((millis() - milStartScene) > 1000 ){runMotor=0;}//{Serial.println("1st pause");} // +500
  else if((millis() - milStartScene) > 0){startFog(); 
                                          if(runMotor==0){runMotor=1; startMotor();} else{};
                                          startLightRed();} //Fog, Lid, Flash
  else{Serial.println("else stop"); runScene2 = 0; stopAll();} //OFF
}

void scene3(){
  if((millis() - milStartScene) > 12000){Serial.println("Scene 1 Completed"); runScene3 = 0; stopAll(); } //OFF
  else if((millis() - milStartScene) > 10500 ){}//{Serial.println("6th pause");} // +1500
  else if((millis() - milStartScene) > 9500 ){startLightGreen(); startFan();
                                              if(runMotor==0){runMotor=1; startMotor();} else{};}
  else if((millis() - milStartScene) > 8500 ){runMotor=0;}//{Serial.println("5th pause");} // +1000
  else if((millis() - milStartScene) > 7500 ){startLightGreen(); startFog();
                                              if(runMotor==0){runMotor=1; startMotor();} else{};}
  else if((millis() - milStartScene) > 6000 ){runMotor=0;}//{Serial.println("4th pause");} // +1500
  else if((millis() - milStartScene) > 5000 ){startLightGreen(); startFog(); startFan();}
  else if((millis() - milStartScene) > 4500 ){runMotor=0;}//{Serial.println("3rd pause");} // +500
  else if((millis() - milStartScene) > 3500 ){if(runMotor==0){runMotor=1; startMotor();} else{};
                                              startFog(); startLaser(); startFan();} //
  else if((millis() - milStartScene) > 2500 ){runMotor=0;}//{Serial.println("2nd pause");} // +1000
  else if((millis() - milStartScene) > 1500){startLightGreen();startLaser();
                                             if(runSound==0){int_song=11;runSound=1;startSound();}else{};}
  else if((millis() - milStartScene) > 1000 ){runMotor=0;}//{Serial.println("1st pause");} // +500
  else if((millis() - milStartScene) > 0){startFog(); 
                                          if(runMotor==0){runMotor=1; startMotor();} else{};
                                          startLightGreen();} //Fog, Lid, Flash
  else{Serial.println("else stop"); runScene3 = 0; stopAll();} //OFF
}


