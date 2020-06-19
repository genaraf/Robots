// https://youtu.be/KfP_LfUiwdc

// Select Arduino nano / ATmega328P (Old Bootloader)
#include <Servo.h>
#include <FastLED.h>

Servo armServo;

#define SERVO_PIN 5
#define RGBLED_PIN 4
#define PROXIM_PIN A4

#define SERVO_TOP 160
#define SERVO_DOWN 90
#define SERVO_UP 120
#define SERVO_MIDLE (SERVO_UP-20)

#define NUM_LEDS 3
 
CRGB leds[NUM_LEDS];

void setup() {
  armServo.attach(SERVO_PIN);
  armServo.write(SERVO_DOWN);

  pinMode(PROXIM_PIN, INPUT)  ;
  pinMode(13, OUTPUT);
  
  FastLED.addLeds<WS2812, RGBLED_PIN, GRB>(leds, NUM_LEDS);
  setLeds(CRGB(0, 0, 0));
}

int sensorValue = 0;
int prevSensorValue = 0;
const int THRESHOLD = 360;
void loop() {
  
  sensorValue = analogRead(PROXIM_PIN);
  if (prevSensorValue <= THRESHOLD) {
    if (sensorValue > THRESHOLD) {
      action();
    }
  }
  
  prevSensorValue = sensorValue;
  delay(10);
}

void setLeds(CRGB rgb) {
  for(int i = 0; i < NUM_LEDS; i++)
    leds[i] = rgb;
    FastLED.show();    
}

void action() {
  led(true);

  //eating
  setLeds(CRGB(255, 0, 0));
  delay(1000);
  armServo.attach(SERVO_PIN);
  armServo.write(160);
  delay(300);

  setLeds(CRGB(0, 255, 0));
  armServo.write(SERVO_UP);
  delay(500);

  //after ate
  delay(100);
  armServo.write(SERVO_MIDLE);
  delay(250);
  armServo.write(SERVO_UP);
  delay(250);
  armServo.write(SERVO_MIDLE);
  delay(250);
  setLeds(CRGB(0, 100, 50));
  armServo.write(SERVO_UP);
  delay(250);
  armServo.write(SERVO_MIDLE);
  delay(250);
  armServo.write(SERVO_UP);
  delay(250);
  armServo.write(SERVO_MIDLE);
  delay(250);

  
  armServo.write(SERVO_DOWN);
  delay(250);
  //release arm's torque
  armServo.detach();
  setLeds(CRGB(0, 0, 0));
  led(false);
}

void led(bool onOff) {
  if (onOff) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}

