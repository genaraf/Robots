#include <Servo.h>

#define ROBOT_NAME "RobotBob"

// Servos Matrix
const int ALLSERVOS = 4;             // 0 ~ 3 Servo
const int ALLMATRIX = ALLSERVOS + 1;  // servo + Run Time
const int PWMRES_Min = 1;             // PWM Resolution 1
const int PWMRES_Max = 120;           // PWM Resolution 120

int GPIO12_PWM;
int GPIO14_PWM;

Servo GPIO12SERVO;
Servo GPIO14SERVO;
Servo GPIO15SERVO;
Servo GPIO16SERVO;



/*============================================================================*/
// ES08MDII - No correction
///////////////////////////////////////////////////////    0    ,    1    ,    2    ,    3,  ms
int Servo_Act_Fix [ ] PROGMEM = {   0,        0,        0,        0,      0     };


/*============================================================================*/
// ES08MDII - Da Vinci posture
/////////////////////////////////////////////////////    0    ,    1    ,    2    ,    3,  ms
int Servo_Act_0 [ ] PROGMEM = {   65,      35,      80,      60,    0     };


/*============================================================================*/
// ES08MDII - stand at attention
/////////////////////////////////////////////////////    0    ,    1    ,    2    ,    3,  ms
int Servo_Act_1 [ ] PROGMEM = {   65,      35,      80,      60,        0     };


/*============================================================================*/
// Standby posture
int Servo_Prg_0 [][ALLMATRIX] PROGMEM = {
  //    0    ,    1    ,    2    ,    3    ,    ms
  {    65,      35,      80,      60,      500   }     // right now
};

void Set_PWM_to_Servo(int iServo, int iValue)
{
  int NewPWM = map(iValue, PWMRES_Min, PWMRES_Max, SERVOMIN, SERVOMAX);
  switch(iServo)
  {
    case 0:
      GPIO12SERVO.write(iValue);
      break;
    case 1:
      GPIO12SERVO.write(iValue);
      break;
    case 2:
      GPIO12SERVO.write(iValue);
      break;
    case 3:
      GPIO12SERVO.write(iValue);
      break;
  }
}

void servoInit()
{
    // Software PWM PIN
  GPIO12SERVO.attach(12);
  GPIO14SERVO.attach(14);

}


