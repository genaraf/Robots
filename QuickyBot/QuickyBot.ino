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


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <PiezoEffects.h>
#include <Adafruit_NeoPixel.h>


Servo leftMotor;
Servo rightMotor;
PiezoEffects mySounds( BUZZER_PIN ); //PiezoEffects object

#define BRIGHTNESS 50
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, RGB_LED_PIN, NEO_GRBW + NEO_KHZ800);

int ledtMotorOffset = 12;
int rightMotorOffset = 0;
int distance;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "6c9e360e833d49d79626f9909c673be3";

BLYNK_READ(V5) {
  Blynk.virtualWrite(V5, distance);
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V2 Button is: ");
  Serial.println(pinValue);
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

BLYNK_WRITE(V7)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V7 Slider value is: ");
  Serial.println(pinValue);
}

BLYNK_WRITE(V1) {
  int x = param[0].asInt();
  int y = param[1].asInt();

  // Do something with x and y
  Serial.print("X = ");
  Serial.print(x);
  Serial.print("; Y = ");
  Serial.println(y);
  x = (x -90)/3;
  
  leftMotor.write( (180 - y) + x + ledtMotorOffset);
  rightMotor.write(y + x + rightMotorOffset);  
  
}

long getDistanceCentim() {

  digitalWrite(RANGE_TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  digitalWrite(RANGE_TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(RANGE_TRIG_PIN, LOW);
    
  return pulseIn(RANGE_ECHO_PIN, HIGH) / 58.2;
}

void setup()
{
  // Debug console
  Serial.begin(9600); 
  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.setPixelColor(0, strip.Color(255,0,0, 255 ) );
  strip.show(); // Initialize all pixels to 'off'
  mySounds.play( soundConnection );
  WiFi.mode(WIFI_STA);
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

  pinMode(RANGE_TRIG_PIN, OUTPUT);
  pinMode(RANGE_ECHO_PIN, INPUT);

  leftMotor.attach(LEFT_MOTOR_PIN);
  rightMotor.attach(RIGHT_MOTOR_PIN);
  leftMotor.write(90 + ledtMotorOffset);
//  rightMotor.write(90 + rightMotorOffset);

  strip.setPixelColor(0, strip.Color(0,150,0));
  strip.show(); 
}

void loop()
{
  Blynk.run();
  distance = getDistanceCentim();
}

