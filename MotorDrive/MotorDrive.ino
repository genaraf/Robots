/*  MonsterMoto Shield Example Sketch
  date: 5/24/11
  code by: Jim Lindblom
  hardware by: Nate Bernstein
  SparkFun Electronics
 This is really simple example code to get you some basic
 functionality with the MonsterMoto Shield. The MonsterMote uses
 two VNH2SP30 high-current full-bridge motor drivers.
 
 Use the motorGo(uint8_t motor, uint8_t direct, uint8_t pwm) 
 function to get motors going in either CW, CCW, BRAKEVCC, or 
 BRAKEGND. Use motorOff(int motor) to turn a specific motor off.
 
 The motor variable in each function should be either a 0 or a 1.
 pwm in the motorGo function should be a value between 0 and 255.
 
This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!
Distributed as-is; no warranty is given.
 */
#include <Wire.h>
 
#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3

#define DEFAULT_CS_THRESHOLD 100

#define MOTORDRV_I2C_ADDR 0x0F

/* VNH2SP30 pin definitions
 xxx[0] controls '1' outputs
 xxx[1] controls '2' outputs */
int inApin[2] = {7, 4};  // INA: Clockwise input
int inBpin[2] = {8, 9}; // INB: Counter-clockwise input
int pwmpin[2] = {5, 6}; // PWM input
int cspin[2] = {2, 3}; // CS: Current sense ANALOG input
int enpin[2] = {0, 1}; // EN: Status of switches output (Analog pin)


int statpin = 13;

uint16 MotorCurent[2];
uint16 CSTrashhold = CS_THRESHOLD;

uint8_t i2cBuf[4];

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
/*
  
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
*/
}

void setup()
{

  Wire.begin(MOTORDRV_I2C_ADDR); 
  Wire.onReceive(receiveEvent); // register I2C event
  Serial.begin(115200);
  
  // Set D5, D5 PWM Frequency
  TCCR0B = TCCR0B & 0b11111000 | 2;
  
  // Initialize digital pins as outputs
  pinMode(statpin, OUTPUT);
  for (int i=0; i<2; i++)
  {
    pinMode(inApin[i], OUTPUT);
    pinMode(inBpin[i], OUTPUT);
    pinMode(pwmpin[i], OUTPUT);
  }
  
  // Initialize braked
  for (int i=0; i<2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
    analogWrite(pwmpin[i], 0);
  }
}

void loop()
{
  motorGo(0, CW, 1023);
  motorGo(1, CCW, 1023);
  delay(30000);

  motorGo(0, CCW, 50);
  motorGo(1, CW, 50);
  delay(30000);
  
  if ((analogRead(cspin[0]) < CS_THRESHOLD) && (analogRead(cspin[1]) < CS_THRESHOLD))
    digitalWrite(statpin, HIGH);
  else 
    digitalWrite(statpin, LOW);

  motorOff(0);
  motorOff(1);
  delay(1000);
}

void motorOff(int motor)
{
  // Initialize braked
  digitalWrite(inApin[motor], LOW);
  digitalWrite(inBpin[motor], LOW);
  analogWrite(pwmpin[motor], 0);
}

/* motorGo() will set a motor going in a specific direction
 the motor will continue going in that direction, at that speed
 until told to do otherwise.
 
 motor: this should be either 0 or 1, will selet which of the two
 motors to be controlled
 
 direct: Should be between 0 and 3, with the following result
 0: Brake to VCC
 1: Clockwise
 2: CounterClockwise
 3: Brake to GND
 
 pwm: should be a value between ? and 1023, higher the number, the faster
 it'll go
 */
void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
{
  if (motor <= 1)
  {
    if (direct <=4)
    {
      // Set inA[motor]
      if (direct <=1)
        digitalWrite(inApin[motor], HIGH);
      else
        digitalWrite(inApin[motor], LOW);

      // Set inB[motor]
      if ((direct==0)||(direct==2))
        digitalWrite(inBpin[motor], HIGH);
      else
        digitalWrite(inBpin[motor], LOW);

      analogWrite(pwmpin[motor], pwm);
    }
  }
}
