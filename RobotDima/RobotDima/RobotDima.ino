#include <Wire.h>
#include <SoftwareSerial.h>

#include <Servo.h>
#include <EEPROM.h>
#include "Otto.h"

#define ROBOT_I2C_ADDR 0x8

#define ROBOT_RC_DUR_LSB_CMD_REG    0x40
#define ROBOT_RC_DUR_MSB_CMD_REG    0x41
#define ROBOT_RC_CMD_REG    0x42

#define ROBOT_CMD_REG       0x43
#define ROBOT_CMD_MOUTH     0x44
#define ROBOT_CMD_SOUND     0x45

#define ROBOT_CMD_FWD   1
#define ROBOT_CMD_BACK  2
#define ROBOT_CMD_LEFT  3
#define ROBOT_CMD_RIGHT 4
#define ROBOT_CMD_DIMA  5
#define ROBOT_CMD_STOP  6

Otto Ottobot;

int newMouth = -1;
int newSound = -1;
int newCommand = -1;
int newRcCommand = -1;
int newRcCommandDuration = 0;
bool waitController = true;
uint8_t robotReady = 1;
uint8_t i2cBuf[4];
uint8_t rcDurLSB = 0;
uint8_t rcDurMSB = 0;

MaxMatrix lc=MaxMatrix(12,10,11, 1);

void putMouth(unsigned long int mouth, bool predefined = true) {
   if (predefined){
      lc.writeFull(Ottobot.GetMouthShape(mouth));
   }
   else{
      lc.writeFull(mouth);
  }
}

void putAnimationMouth(unsigned long int aniMouth, int index){
   lc.writeFull(Ottobot.GetAnimShape(aniMouth,index));
}

int getAnimeSize(int id) {
  switch(id) 
  {
    case 0: return 8;  
    case 1: return 4;  
    case 2: return 6;  
    case 3: return 9;  
  }
  return -1;
}

void startAnime(int id, int d) {
    int size = getAnimeSize(id);
    if(size < 0) return;
    for(int i = 0; i < size; i++) {
      putAnimationMouth(id, i);
      delay(d);
    }
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  waitController = false;
  
  while (1 < Wire.available()) { // loop through all but the last
    i2cBuf[0] = Wire.read(); // receive byte as a character
  }
  i2cBuf[1] = Wire.read();    // receive byte as an integer
//  Serial.print(i2cBuf[0], HEX);         // print the character
//  Serial.print("H-");         // print the character
//  Serial.println(i2cBuf[1]);         // print the integer
  switch(i2cBuf[0]) {    

   case ROBOT_RC_DUR_LSB_CMD_REG:
      rcDurLSB = i2cBuf[1];
      break;
   
   case ROBOT_RC_DUR_MSB_CMD_REG:
      rcDurMSB = i2cBuf[1];
      break;
   
   case ROBOT_RC_CMD_REG:
      newRcCommandDuration = rcDurMSB;
      newRcCommandDuration <<= 8;
      newRcCommandDuration += rcDurLSB;
      newRcCommand = i2cBuf[1];
      Serial.print("RC:");         
      Serial.print(newRcCommand);  
      Serial.print(" Duration:");         
      Serial.println(newRcCommandDuration);  
      break;
    
    case ROBOT_CMD_REG:
      Serial.print("Command:");
      Serial.println(i2cBuf[1]);
      newCommand = i2cBuf[1];
      break;

    case ROBOT_CMD_MOUTH:
      if(i2cBuf[1] >=0 && i2cBuf[1] <=30)
        newMouth = i2cBuf[1];
      else
        newMouth = i2cBuf[1];      
      Serial.print("Mouth:");
      Serial.println(newMouth);
      break;
      
    case ROBOT_CMD_SOUND:
      if(i2cBuf[1] >= 0 && i2cBuf[1] <= 18)
        newSound = i2cBuf[1];
      else
        newSound = 0;
      Serial.print("Sound:");
      Serial.println(newSound);
      break;
  }
}

void setup(){
    
    Wire.begin(ROBOT_I2C_ADDR); 
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(115200);
    lc.init();
    lc.setIntensity(1);    // Poner el brillo a un valor intermedio
    Ottobot.init(2,3,4,5,6,7,true,A6,13,8,9);
    Ottobot.sing(6);
    Serial.println("waiting for connect...");
    while(waitController) {
      startAnime(0, 500);
    }
    Serial.println("connected.");
}

void DimaDemo() {
    robotReady = 0;
    for(int __i__=0;__i__<2;++__i__)
        Ottobot.move(12,1000,20);
    putMouth(11);
    Ottobot.sing(7);
    for(int __i__=0;__i__<3;++__i__)
        Ottobot.move(19,1000,20);
    putMouth(16);
    Ottobot.sing(11);
    for(int __i__=0;__i__<3;++__i__)
        Ottobot.move(6,1700,25);
    putAnimationMouth(0,0);
    putMouth(13);
    Ottobot.sing(0);
    for(int __i__=0;__i__<3;++__i__)
        Ottobot.move(7,1000,25);
    putMouth(14);
    Ottobot.sing(2);
    for(int __i__=0;__i__<3;++__i__)
        Ottobot.move(9,1700,25);
    Ottobot.move(15,1300,25);
    Ottobot.move(16,1300,25);
    Ottobot.sing(3);
    for(int __i__=0;__i__<2;++__i__)
        Ottobot.move(17,1300,25);
    
    Ottobot.move(0,1300,25); 
    
    for(int __i__=0;__i__<2;++__i__) 
        Ottobot.move(18,1300,25);
    
    putMouth(25);
    Ottobot.sing(4);    
    Ottobot.putAnimationMouth(3,0);
    for(int __i__=0;__i__<1;++__i__)
    {
        Ottobot.move(1,1300,25);
        _delay(0.5);
        Ottobot.move(0,1300,25);
    }
    robotReady = 1;
}

void loop(){
 
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
  if(newMouth >= 0) {
    putMouth(newMouth);
    newMouth = -1; 
  }
  
  if(newSound >= 0) {
    Ottobot.sing(newSound);
    newSound = -1;
  }
  
  if(newRcCommand >= 0) {
    newCommand = -1;
    Ottobot.move(newRcCommand, newRcCommandDuration, 25);
    newRcCommand = -1;   
  }
  
  int cmdDuration = 500;
  if(newCommand >= 0) {
    switch(newCommand) {
        case ROBOT_CMD_FWD:
            Ottobot.walk(4, 500, FORWARD);
            robotReady = 1;
            break;
            
        case ROBOT_CMD_BACK:
            Ottobot.walk(4, 500, BACKWARD);
            break;
            
        case ROBOT_CMD_LEFT:
            Ottobot.turn(4, 500, LEFT);
            break;
            
        case ROBOT_CMD_RIGHT:
            Ottobot.turn(4, 500, RIGHT);
            break;
            
        case ROBOT_CMD_DIMA:
            DimaDemo();  
            break;
            
        case ROBOT_CMD_STOP:
            Ottobot.home();
            newCommand = -1;
            robotReady = 1;
            break;
    }
  }
  delay(10);
}
