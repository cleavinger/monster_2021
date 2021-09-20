const int RELAY_FAN = 2;     //Pin for fan
const int RELAY_FOG = 5;     //Pin for Fog machine remote
const int PIN_LASER = 7;     // Pin for laser eyes
const int RELAY_LIGHT = 3;   //Pin for lightbulb
const int RELAY_MOTOR = 4;   //Pin for DC Motor
const int PIN_SOUND = 8;   //Pin for sound
const int PIN_SENSOR = 12;   // Doppler Input Pin

String bt_command; //string for command from bluetooth

/*Setup times*/
unsigned long milCurrent;
unsigned long milStartFan;
unsigned long milStartFog;
unsigned long milStartLaser;
unsigned long milStartLight;
unsigned long milStartMotor;
unsigned long milStartSound;

/*setup run variables*/
int runFan = 0;
int runFog = 0;
int runLaser = 0;
int runLight = 0;
int runMotor = 0;
int runSound = 0;


void setup() {
  // put your setup code here, to run once:
  /*Define the type of pin*/
  pinMode(RELAY_FAN, OUTPUT); 
  pinMode(RELAY_FOG, OUTPUT);
  pinMode(PIN_LASER, OUTPUT);
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_MOTOR, OUTPUT);
   

  /*Set everything to off*/
  digitalWrite(RELAY_FAN, HIGH);    //relay is set to High for off
  digitalWrite(RELAY_FOG, HIGH);    //relay is set to High for off
  digitalWrite(PIN_LASER, LOW);     //digital pin is set to Low for off
  digitalWrite(RELAY_LIGHT, HIGH);  //relay is set to High for off
  digitalWrite(RELAY_MOTOR, HIGH);  //relay is set to High for off
   

  bt_command == " "; //assign empty string
  Serial.begin(9600); //start the serial for bluetooth

}

void loop() {
  milCurrent = millis(); //get current time count

  /*check if button was pusehd from the app*/
  if(Serial.available() > 0 ){
    bt_command=Serial.read();
    Serial.println(bt_command);
    
    if(bt_command=="49"){//byte version of 1
      startFan();
      Serial.println("Spin that Fan");
      bt_command == " ";
    }

    if(bt_command=="50"){//byte version of 2
      startFog();
      Serial.println("Fog Me");
      bt_command == " ";
    }

    if(bt_command=="51"){//byte version of 3
      startLaser();
      Serial.println("Laser Eyes!");
      bt_command == " ";
    }

    if(bt_command=="53"){//byte version of 5
      startLight();
      Serial.println("Light Flasher");
      bt_command == " ";
    }    

    if(bt_command=="52"){//byte version of 4
      startMotor();
      Serial.println("Shake that lid");
      bt_command == " ";
    }

    if(bt_command=="54"){//byte version of 6
      startSound();
      Serial.println("Make some noise!");
      bt_command == " ";
    }      

    if(bt_command=="65"){//byte version of A
      stopAll();
      Serial.println("Just stop it all");
      bt_command == " ";
    }      
  }
  
  /*run functions if active*/
  if(runFan == 1){FanSpin();}
  if(runFog == 1){FogMe();}
  if(runLaser == 1){LaserEyes();}
  if(runLight == 1){LightFlash();}
  if(runMotor == 1){LidFlap();}
  //if(runSound == 1){MakeNoise();} - This isn't a trigger, we have to see how this works


}


/* define start functions */
void startFan(){runFan = 1; milStartFan = millis();}
void startFog(){runFog = 1; milStartFog = millis();}
void startLaser(){runLaser = 1; milStartLaser = millis();}
void startLight(){runLight = 1; milStartLight = millis();}
void startMotor(){runMotor = 1; milStartMotor = millis();}
void startSound(){runSound = 1; milStartSound = millis();}

/* define stop functions */
void stopAll(){
  runFan = 0;
  runFog = 0;
  runLaser = 0;
  runLight = 0;
  runMotor = 0;
  runSound = 0;
  Serial.println("Everything stopped");
}

void FanSpin(){
  if((milCurrent - milStartFan) > 1050){digitalWrite(RELAY_FAN, HIGH); runFan ==0;} //OFF
  else if((milCurrent - milStartFan) > 0){digitalWrite(RELAY_FAN, LOW); } //ON
  else{digitalWrite(RELAY_FAN, HIGH);} //OFF
}

void FogMe(){
  if((milCurrent - milStartFog) > 1050){digitalWrite(RELAY_FOG, HIGH); runFog ==0;} //OFF
  else if((milCurrent - milStartFog) > 0){digitalWrite(RELAY_FOG, LOW); } //ON
  else{digitalWrite(RELAY_FOG, HIGH); runFog ==0;} //OFF
}

void LaserEyes(){
  if((milCurrent - milStartLaser) > 5000){digitalWrite(PIN_LASER, LOW); runLaser ==0;} //OFF
  else if((milCurrent - milStartLaser) > 4000){digitalWrite(PIN_LASER, HIGH);} //ON
  else if((milCurrent - milStartLaser) > 3900){digitalWrite(PIN_LASER, LOW);} //OFF
  else if((milCurrent - milStartLaser) > 2800){digitalWrite(PIN_LASER, HIGH);} //ON
  else if((milCurrent - milStartLaser) > 2700){digitalWrite(PIN_LASER, LOW);} //OFF
  else if((milCurrent - milStartLaser) > 1500){digitalWrite(PIN_LASER, HIGH);} //ON
  else if((milCurrent - milStartLaser) > 1400){digitalWrite(PIN_LASER, LOW);} //OFF
  else if((milCurrent - milStartLaser) > 0){digitalWrite(PIN_LASER, HIGH); } //ON
  else{digitalWrite(PIN_LASER, LOW); runLaser ==0;} //OFF
  
}

void LightFlash(){
  if((milCurrent - milStartLight) > 3500){digitalWrite(RELAY_LIGHT, HIGH); runLight ==0;} //OFF
  else if((milCurrent - milStartLight) > 3350){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((milCurrent - milStartLight) > 3200){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((milCurrent - milStartLight) > 3000){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((milCurrent - milStartLight) > 2800){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((milCurrent - milStartLight) > 2450){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((milCurrent - milStartLight) > 2380){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((milCurrent - milStartLight) > 2050){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((milCurrent - milStartLight) > 1900){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((milCurrent - milStartLight) > 1750){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((milCurrent - milStartLight) > 1680){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((milCurrent - milStartLight) > 1400){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((milCurrent - milStartLight) > 1300){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((milCurrent - milStartLight) > 1100){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((milCurrent - milStartLight) > 1020){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((milCurrent - milStartLight) > 840){digitalWrite(RELAY_LIGHT, LOW);} //ON
  else if((milCurrent - milStartLight) > 500){digitalWrite(RELAY_LIGHT, HIGH);} //OFF
  else if((milCurrent - milStartLight) > 0){digitalWrite(RELAY_LIGHT, LOW); } //ON
  else{digitalWrite(RELAY_LIGHT, HIGH); runLight ==0;} //OFF
  
}

void LidFlap(){
  if((milCurrent - milStartMotor) > 100){digitalWrite(RELAY_MOTOR, HIGH); runMotor ==0;} //OFF
  else if((milCurrent - milStartMotor) > 0){digitalWrite(RELAY_MOTOR, LOW); } //ON
  else{digitalWrite(RELAY_MOTOR, HIGH); runMotor ==0;} //OFF
}


void MakeNoise(){
  if((milCurrent - milStartSound) > 15000){digitalWrite(PIN_SOUND, LOW); runMotor ==0;} //OFF
  else if((milCurrent - milStartSound) > 0){digitalWrite(PIN_SOUND, HIGH); } //ON
  else{digitalWrite(PIN_SOUND, LOW); runMotor ==0;} //OFF
}





