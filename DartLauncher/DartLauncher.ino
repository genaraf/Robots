#include <Servo.h>
#include <Wire.h>

Servo elevation;
Servo feeder;
Servo motors;

#define DEFAULT_ELEVATION 15
#define DEFAULT_MOTOR_SPEED 100
#define DEFAULT_MOTOR_DELAY 2000
#define DEFAULT_LAUNCH_DELAY 500

#define ADDR_I2C 4


#define FEEDER_START 1000
#define FEEDER_STOP  2000
#define FEDER_DELAY  1000 

#define ELEVATION_MIN 2000
#define ELEVATION_MAX 1000
#define ELEVATION_MAX_AGLE 30

#define LAUNCHER_SPEED_MIN 1000
#define LAUNCHER_SPEED_MAX 1700

#define MOTORS_PIN    5
#define FEEDER_PIN    6
#define ELEVATION_PIN 7

#define AUXOUT_PIN    8
#define TESTBTN_PIN   9

static int motorPwm = LAUNCHER_SPEED_MIN;
static bool fireStartOlsState = false;

static int elevAngle = DEFAULT_ELEVATION;
static int motorSpeed = DEFAULT_MOTOR_SPEED;
static int motorDelayMs = DEFAULT_MOTOR_DELAY;
static int launchDelayMs = DEFAULT_LAUNCH_DELAY;
static int launchCnt = 0;

void setDefault() {
  elevAngle = DEFAULT_ELEVATION;
  motorSpeed = DEFAULT_MOTOR_SPEED;
  motorDelayMs = DEFAULT_MOTOR_DELAY;
  launchDelayMs = DEFAULT_LAUNCH_DELAY;
  launchCnt = 0;
}

void requestEvent() {
   char c = Wire.read();
   Wire.write(&c, 1);  
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  char c;
  while(1 < Wire.available()) // loop through all but the last
  {
    c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  switch(c) {
    case 1:
      break;
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}


void setup() {
  Serial.begin(115200);
  Wire.begin(ADDR_I2C);                
  
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  
  pinMode(AUXOUT_PIN, INPUT ); 
  digitalWrite(AUXOUT_PIN, HIGH );           

  pinMode(TESTBTN_PIN, INPUT);           
  digitalWrite(TESTBTN_PIN, HIGH); // turn on pullup resistors
  
  elevation.attach(ELEVATION_PIN);
  elevation.writeMicroseconds(ELEVATION_MIN);
  
  feeder.attach(FEEDER_PIN, 1000, 2000);
  feeder. writeMicroseconds(FEEDER_STOP);
  
  motors.attach(MOTORS_PIN);
  motorPwm = LAUNCHER_SPEED_MIN;
  motors.writeMicroseconds(motorPwm);

  pinMode(TESTBTN_PIN,INPUT_PULLUP);

  delay(200);
}

bool ifStart() {
  return (digitalRead(TESTBTN_PIN) == LOW) || launchCnt > 0;
}
/*
void motorsSpeed(int speed) {
  static int stepVal, stepNumb, pwm; 
  speed = constrain(speed, 0, 100);
  pwm = LAUNCHER_SPEED_MIN + ((LAUNCHER_SPEED_MAX - LAUNCHER_SPEED_MIN) * speed) / 100; 
  Serial.print("speed:");
  Serial.println(speed);
  Serial.print("pwm:");
  Serial.println(pwm);
  Serial.print("motorPwm:");
  Serial.println(motorPwm);
  stepVal = 50;
  if(motorPwm < pwm) {
    stepNumb = (pwm - motorPwm) / stepVal;
  } else {
    stepNumb = (motorPwm - pwm) / stepVal;
    stepVal = -stepVal;
  }
  stepNumb += 1;
  for(int i = 0; i < stepNumb; i++) {
    motorPwm = constrain(motorPwm + stepVal, LAUNCHER_SPEED_MIN, LAUNCHER_SPEED_MAX);  
    motors.writeMicroseconds(motorPwm);
    delay(100);
    Serial.print("motor:");
    Serial.println(motorPwm);
  }
}
*/

void auxOutput(bool on) {
  if(on) {
    pinMode(AUXOUT_PIN, OUTPUT); 
    digitalWrite(AUXOUT_PIN, LOW);           
  } else {
    pinMode(AUXOUT_PIN, INPUT); 
    digitalWrite(AUXOUT_PIN, HIGH);           
  }
  
}

void motorsSpeed(int speed) {
  static int stepVal, stepNumb, pwm; 
  speed = constrain(speed, 0, 100);
  pwm = LAUNCHER_SPEED_MIN + ((LAUNCHER_SPEED_MAX - LAUNCHER_SPEED_MIN) * speed) / 100; 
  motors.writeMicroseconds(pwm);
  Serial.print("motor:");
  Serial.println(motorPwm);
  delay(100);
}

void dartPush() {
  feeder.writeMicroseconds(FEEDER_START);
  delay(FEDER_DELAY);  
  feeder.writeMicroseconds(FEEDER_STOP);
}


void elevationSet(int angle) {
  int pwm = map(angle, 0, ELEVATION_MAX_AGLE, ELEVATION_MIN, ELEVATION_MAX); 
  elevation.writeMicroseconds(pwm);
}


void loop() {
  if(ifStart()) {
    fireStartOlsState = true;
    Serial.print("Pli\r\n");
    elevationSet(elevAngle);
    motorsSpeed(motorSpeed);  
    delay(motorDelayMs);
    while(ifStart()) {
      if(launchCnt > 0) launchCnt--;
      dartPush();
      delay(launchDelayMs);
    }  
    feeder.writeMicroseconds(FEEDER_STOP);
    motorsSpeed(0);
    delay(100);
    elevationSet(0);
    fireStartOlsState = false;
  }
  delay(10);
}
