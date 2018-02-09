#include <Arduino.h>

/***********************************************************************************
 *                  RobotGeek 'Chip-E' Biped - Basic Walking
 *
 *                               ______________
 *                              /              |
 *                             |   _________   |
 *                             |  [_________]  |
 *                             |               |
 *                             |               |
 *                             |               |
 *                             |       O       |
 *                             |               |
 *                              ---------------
 *                                 ||     ||
 *                                 ||     ||
 *                                 ||     ||
 *                            -------     -------
 *                            -------     -------
 *
 *  Wiring
 *
 *    Buzzer - Digital Pin 2 (D4)
 *    Left Hip Servo - Digital Pin 10
 *    Right Hip Servo - Digital Pin 9
 *    Left Ankle Servo - Digital Pin 6
 *    Right Ankle Servo - Digital Pin 5
 *
 *    Jumper for pins 9/10/11 should be set to 'VIN'
 *    Jumper for pins 3/5/6 should be set to 'VIN'
 *
 *  Control Behavior:
 *
 *    Power on Chip-E for a short demonstration.
 *
 ***********************************************************************************/

#include <ChipE.h>
#include <PiezoEffects.h>

//--Based Chip-E
/*
 *          ______________
 *         /              |
 *        |   _________   |
 *        |  [_________]  |
 *        |               |
 *        |               |
 *        |               |
 *        |       O       |
 * YR ==> |               | <== YL
 *         ---------------
 *            ||     ||
 *            ||     ||
 *            ||     ||
 * RR ==>   -----   ------  <== RL
 *          -----   ------
*/
const int PIN_RR = 13; // D7 Roll Right
const int PIN_RL = 14; // D5 Roll Left
const int PIN_YR = 15; // D8 Yaw Right
const int PIN_YL = 12; // D6 Yaw Left
const int TRIM_RR = 10; //Trim on the right ankle (adjust +/- as necessary)
const int TRIM_RL = -5; //Trim on the left ankle (adjust +/- as necessary)
const int TRIM_YR = -1; //Trim on the right hip (adjust +/- as necessary)
const int TRIM_YL = -1; //Trim on the left hip (adjust +/- as necessary)

ChipE myChipE; //Chip-E object (your robot!:)

const int BUZZER_PIN = 2; // D4 Piezo effects (sounds)
const int TUCH_PIN = 4; // D2
const int VBAT_PIN = A0;
const int RANGE_TRIG_PIN = 16; // D0
const int RANGE_ECHO_PIN = 5;  // D1


PiezoEffects mySounds( BUZZER_PIN ); //PiezoEffects object

void setup()
{
  Serial.begin(115200);
//  ESP.wdtDisable();

  //--Chip-E initialization
  myChipE.init( PIN_YL, PIN_YR, PIN_RL, PIN_RR );
  myChipE.setTrims( TRIM_YL, TRIM_YR, TRIM_RL, TRIM_RR ); //Set the servo trims
  myChipE.home(); //Move feet to home position
  delay( 500 );

  //--Sound initialization
  mySounds.play( soundHappy );
  myChipE.home();
}

void loop()
{
//  processSerialCommand();
  myChipE.flapping( 10, 600, 10, FORWARD );
 /* 
  myChipE.home();
  delay( 500 );

  myChipE.walk( 4, 2000, FORWARD );
  myChipE.walk( 4, 1000, FORWARD );

  myChipE.home();
  mySounds.play( soundWhistle );

  myChipE.walk( 4, 1000, BACKWARD );
  myChipE.walk( 4, 2000, BACKWARD );

  myChipE.home();
  mySounds.play( soundWhistle );

  myChipE.turn( 3, 2000, RIGHT );
  myChipE.turn( 3, 2000, LEFT );

  mySounds.play( soundUp );
  myChipE.updown( 6, 500, SMALL );
  mySounds.play( soundUp );
  myChipE.updown( 6, 500, MEDIUM );
  mySounds.play( soundDown );
  myChipE.updown( 2, 4000, BIG );

  myChipE.moonwalker( 3, 2000, 30, LEFT );
  myChipE.moonwalker( 3, 2000, 30, RIGHT );

  myChipE.swing( 6, 600, 10 );
  myChipE.swing( 4, 2000, 40 );

  myChipE.crusaito( 6, 2000, 40, LEFT );
  myChipE.crusaito( 6, 2000, 40, RIGHT );

  myChipE.flapping( 10, 600, 10, FORWARD );
  myChipE.flapping( 10, 600, 10, BACKWARD );

  mySounds.play( soundBeeps );
  */

}

void processSerialCommand()
{
  char buffer[64] = {0};
  int i = 0;

  if ( Serial.available() )
  {
    delay(100); //wait for remaining bytes
    while( Serial.available() && i < sizeof(buffer) )
    {
      buffer[i++] = Serial.read();
    }
    buffer[i++] = '\0';
  }

  if( i > 0 )
  {
    //Compare strings and perform actions
    if (strncmp(buffer, "walk", 4) == 0)
    {
      myChipE.walk( 2, 2000, FORWARD );
      myChipE.walk( 2, 1000, FORWARD );
      myChipE.home();
      mySounds.play( soundCuddly );
    } else if (strncmp(buffer, "hop", 3) == 0)
    {
      mySounds.play( soundUp );
      myChipE.updown( 4, 500, SMALL );
      mySounds.play( soundUp );
      myChipE.updown( 4, 500, MEDIUM );
      mySounds.play( soundDown );
      myChipE.updown( 1, 4000, BIG );
      myChipE.home();
    } else if (strncmp(buffer, "moonwalk", 8) == 0 || strncmp(buffer, "moon%20walk", 11) == 0)
    {
      myChipE.moonwalker( 2, 2000, 30, LEFT );
      myChipE.moonwalker( 2, 2000, 30, RIGHT );
      myChipE.home();
    } else if (strncmp(buffer, "turn", 4) == 0)
    {
      myChipE.turn( 3, 2000, RIGHT );
      myChipE.turn( 3, 2000, LEFT );
      myChipE.home();
    } else if (strncmp(buffer, "flapping", 4) == 0)
    {
       myChipE.flapping( 10, 600, 10, FORWARD );
       myChipE.home();
    } else if (strncmp(buffer, "sing", 4) == 0)
    {
      mySounds.play( soundBeeps );
    } else if (strncmp(buffer, "whistle", 7) == 0)
    {
      mySounds.play( soundWhistle );
    }
  }
}
