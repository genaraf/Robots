/*
  Esplora Remote

 This sketch allows to test all the Esplora's peripherals.
 It is also used with the ProcessingStart sketch (for Processing).

 When uploaded, you can open the Serial monitor and write one of
 the following commands (without quotes) to get an answer:

 "D": prints the current value of all sensors, separated by a comma.
 See the dumpInputs() function below to get the meaning of
 each value.

 "Rxxx"
 "Gxxx"
 "Bxxx": set the color of the RGB led. For example, write "R255"
 to turn on the red to full brightness, "G128" to turn
 the green to half brightness, or "G0" to turn off
 the green channel.

 "Txxxx": play a tone with the buzzer. The number is the
 frequency, e.g. "T440" plays the central A note.
 Write "T0" to turn off the buzzer.


 Created on 22 november 2012
 By Enrico Gueli <enrico.gueli@gmail.com>
 Modified 23 Dec 2012
 by Tom Igoe
 */

#define rSerial Serial1 

#include <Esplora.h>

void setup() {
  Serial.begin(9600);
//  while (!Serial); // needed for Leonardo-based board like Esplora
  Serial1.begin(115200);
  delay(200);
  Serial.println('Remote Started Ver 1.0');
}


void loop() {
 if (rSerial.available())
    parseCommand();
}

/*
 * This function reads a character from the serial line and
 * decide what to do next. The "what to do" part is given by
 * function it calls (e.g. dumpInputs(), setRed() and so on).
 */
void parseCommand() {
  char cmd = rSerial.read();
  switch (cmd) {
    case 'D':
      dumpInputs();
      break;
    case 'O':
      dumpOtherInputs();
      break;
    case 'R':
      setRed();
      break;
    case 'G':
      setGreen();
      break;
    case 'B':
      setBlue();
      break;
    case 'T':
      setTone();
      break;
  }
}

void dumpInputs() {
  byte btn = 0;
  if(Esplora.readButton(SWITCH_1) == LOW)
    btn |= 0x01;
  if(Esplora.readButton(SWITCH_2) == LOW)
    btn |= 0x02;
  if(Esplora.readButton(SWITCH_3) == LOW)
    btn |= 0x04;
  if(Esplora.readButton(SWITCH_4) == LOW)
    btn |= 0x08;
  if(Esplora.readJoystickSwitch() == LOW)
    btn |= 0x10;
  rSerial.print(btn);
  rSerial.print(',');
  rSerial.print(Esplora.readJoystickX());
  rSerial.print(',');
  rSerial.print(Esplora.readJoystickY());
  rSerial.print(',');
  rSerial.print(Esplora.readAccelerometer(X_AXIS));
  rSerial.print(',');
  rSerial.print(Esplora.readAccelerometer(Y_AXIS));
  rSerial.print(',');
  rSerial.print(Esplora.readAccelerometer(Z_AXIS));
  rSerial.print(',');
  rSerial.print(Esplora.readSlider());
  rSerial.println();
}

void dumpOtherInputs() {
  rSerial.print(Esplora.readLightSensor());
  rSerial.print(',');
  rSerial.print(Esplora.readTemperature(DEGREES_C));
  rSerial.print(',');
  rSerial.print(Esplora.readMicrophone());
  rSerial.println();

}

void setRed() {
  Esplora.writeRed(rSerial.parseInt());
}

void setGreen() {
  Esplora.writeGreen(rSerial.parseInt());
}

void setBlue() {
  Esplora.writeBlue(rSerial.parseInt());
}

void setTone() {
  Esplora.tone(rSerial.parseInt());
}


