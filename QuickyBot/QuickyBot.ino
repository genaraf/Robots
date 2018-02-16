/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  NOTE: SmartConfig might not work in your environment.
        Please try basic ESP8266 SmartConfig examples
        before using this sketch!

  Change Blynk auth token to run :)

 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define LEFT_MOTOR_PIN  D6
#define RIGHT_MOTOR_PIN D5

#define BUZZER_PIN D1 // Piezo effects (sounds) 100om resistor on buzer plus
#define VBAT_PIN A0   // 150K 
#define RANGE_TRIG_PIN D2
#define RANGE_ECHO_PIN D7
#define RGB_LED_PIN D3 // 470om resistor
#define CHARGING_DETECT_PIN D8

#define LOW_BATTERY 644 // ~3.3V
#define MOTORS_STOP_INACTIVITY_TIMEOUT 10000 // 10sec
#define INACTIVITY_SOUND_TIMEOUT 100000      // 100sec
#define AUTOMATIC_PAUSE 2000

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <PiezoEffects.h>
#include <Adafruit_NeoPixel.h>

enum {
  MANUAL_MODE,
  AUTOMATIC1_MODE,
  AUTOMATIC2_MODE,
  GUARD_MODE  
} robotMode;

Servo leftMotor;
Servo rightMotor;
PiezoEffects mySounds( BUZZER_PIN ); //PiezoEffects object

#define BRIGHTNESS 50
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, RGB_LED_PIN, NEO_GRBW + NEO_KHZ800);

int ledtMotorOffset = 12;
int rightMotorOffset = 0;
int distance;
int voltage;
bool motorsOn = false;
unsigned long lastActivity;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "6c9e360e833d49d79626f9909c673be3";

void motorsAttach() {
  if(!leftMotor.attached()) leftMotor.attach(LEFT_MOTOR_PIN);
  if(!rightMotor.attached()) rightMotor.attach(RIGHT_MOTOR_PIN);
  leftMotor.write(90 + ledtMotorOffset);
  rightMotor.write(90 + rightMotorOffset);
  motorsOn = true;  
  
}

void motorsDetach() {
  if(leftMotor.attached()) leftMotor.detach();
  if(rightMotor.attached()) rightMotor.detach();
  motorsOn = false;
}

BLYNK_READ(V5) {
  // 820 - ~4.2V
  Blynk.virtualWrite(V5, voltage);
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); 
  switch(pinValue) {
    case 1:
      Serial.print("Mode: manual");
      strip.setPixelColor(0, strip.Color(0,150,0));
      strip.show();  
      lastActivity = millis();
      robotMode = MANUAL_MODE;
      break;
    
    case 2:
      Serial.print("Mode: automatic 1");
      randomSeed(millis());
      strip.setPixelColor(0, strip.Color(0,120,120, 50 ) );  
      strip.show();  
      lastActivity = millis() - AUTOMATIC_PAUSE;
      robotMode = AUTOMATIC1_MODE;
      motorsAttach();
      break;
    
    case 3:
      Serial.print("Mode: automatic 2");
      randomSeed(millis());
      strip.setPixelColor(0, strip.Color(0,0,250, 50 ) );  
      strip.show();  
      lastActivity = millis() - AUTOMATIC_PAUSE;
      robotMode = AUTOMATIC2_MODE;
      motorsAttach();
      break;

    case 4:
      Serial.print("Mode: guard");
      robotMode = GUARD_MODE;
      motorsDetach();
      strip.clear();  
      break;
   
  }
 }


BLYNK_WRITE(V6)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V6 Slider value is: ");
  Serial.println(pinValue);
  
  ledtMotorOffset += pinValue;
  leftMotor.write(90 + ledtMotorOffset);  
  Serial.println(ledtMotorOffset);
}


// Joystick
BLYNK_WRITE(V1) {
  int x = param[0].asInt();
  int y = param[1].asInt();

  // Do something with x and y
  Serial.print("X = ");
  Serial.print(x);
  Serial.print("; Y = ");
  Serial.println(y);
  if(robotMode != GUARD_MODE) { 
    x = (x -90)/3;
    if(!motorsOn) {
      motorsAttach();  
    }
    leftMotor.write( (180 - y) + x + ledtMotorOffset);
    rightMotor.write(y + x + rightMotorOffset);  
    lastActivity = millis();
  }
}

long getDistanceCentim() {

  digitalWrite(RANGE_TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  digitalWrite(RANGE_TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(RANGE_TRIG_PIN, LOW);
    
  return pulseIn(RANGE_ECHO_PIN, HIGH, 5000) / 58.2;
}

void chargingMode()
{
  Serial.println("Start Charging mode");
  if(Blynk.connected()) Blynk.disconnect();
  WiFi.mode(WIFI_OFF);

  motorsDetach();
  mySounds.play(soundLaugh);
  
  while(digitalRead(CHARGING_DETECT_PIN) == HIGH)
  {
      strip.setPixelColor(0, strip.Color(0,255,0, 50 ) );
      strip.show();
      delay(200);    
      strip.setPixelColor(0, strip.Color(0, 0 ,0, 0 ) );
      strip.show();    
      delay(1000);    
  }
  while(1) {
    mySounds.play(soundCuddly);
    delay(10000);
  }
  Serial.println("End Charging mode");
}

void LowBatteryMode()
{
  Serial.println("Start Charging mode");
  if(Blynk.connected()) Blynk.disconnect();
  WiFi.mode(WIFI_OFF);
  strip.clear();
  strip.show(); // ?? 
  motorsDetach();
  while(1) {
    mySounds.play( soundBeeps );
    delay(200); 
    mySounds.play( soundBeeps );
    delay(200); 
    mySounds.play( soundBeeps );
    delay(10000); 
  }
}

void ManualMode() {
  unsigned long timeOut = millis() - lastActivity; 
  if(timeOut > MOTORS_STOP_INACTIVITY_TIMEOUT) {
      if(motorsOn) {
        motorsDetach();
      } else {
        if(timeOut % INACTIVITY_SOUND_TIMEOUT)
          mySounds.play(soundSleeping);  
      }      
  }
}

void Automatic1Mode() {
  unsigned long timeOut = millis() - lastActivity;
  if(timeOut < AUTOMATIC_PAUSE)
    return;   
}
  
void Automatic2Mode() {
    
}

void GuardMode() {
  distance = getDistanceCentim();    
}

void connect() {
  mySounds.play( soundConnection );
  WiFi.mode(WIFI_STA);
  delay(500);
  int cnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (cnt++ >= 10) {
      mySounds.play( soundOhOoh );
      WiFi.beginSmartConfig();
      while (1) {
        delay(1000);
        if (WiFi.smartConfigDone()) {
          Serial.println();
          Serial.println("SmartConfig: Success");
          break;
        }
        Serial.print("|");
      }
    }
  }
  mySounds.play( soundSuperHappy );
  WiFi.printDiag(Serial);
  Blynk.config(auth);
  lastActivity = millis();
//  motorsAttach();
  strip.setPixelColor(0, strip.Color(0,150,0));
  strip.show(); 

}


void setup()
{
  // Debug console
  robotMode = MANUAL_MODE;
  Serial.begin(9600);
  delay(1000);
  pinMode(CHARGING_DETECT_PIN, INPUT); 
  pinMode(RANGE_TRIG_PIN, OUTPUT);
  pinMode(RANGE_ECHO_PIN, INPUT);

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.setPixelColor(0, strip.Color(255,0,0, 255 ) );
  strip.show();
  if(digitalRead(CHARGING_DETECT_PIN) == HIGH)   {
    chargingMode();
  } 
  if((voltage = analogRead(VBAT_PIN)) < LOW_BATTERY)   {
    LowBatteryMode();
  } 
  connect();
}

void loop()
{
  if(digitalRead(CHARGING_DETECT_PIN) == HIGH)   {
    chargingMode();
    connect();
  } 
  if((voltage = analogRead(VBAT_PIN)) < LOW_BATTERY)   {
    LowBatteryMode();
  } 

  switch(robotMode) {
    case MANUAL_MODE:
      
      break;
    
    case AUTOMATIC1_MODE:
      Automatic1Mode();
      break;
    
    case AUTOMATIC2_MODE:
      Automatic2Mode();
      break;

    case GUARD_MODE:
      GuardMode();
      break;
   
  }
  Blynk.run();
}

