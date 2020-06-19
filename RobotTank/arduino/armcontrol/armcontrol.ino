#include <Servo.h>

int angle_init[7]	  = {90, 101, 165, 153, 90, 120, 0}; // when motor stands straight. In web, the angle when motor stands straight is {0, 90, 130, 180, 0, 0};
int angle_min[7]    = { 0, 0, 0, 20, 0, 75, 0};
int angle_max[7]    = {180, 180, 160, 120, 180, 137, 180};
int angle_offset[7] = {0, 11, -15, -27, 0, 137, 0}; // offset between real servo motor and angle on web
int direction[7]		= {1, 1, 1, 1, 1 ,-1, 1};
int angleSteps[7]	  = {3, 2, 2, 2, 4 ,4, 2}; // moving steps of each motor (degree)

int cur_angles[7]; // current angles of six motors (degree) 
int dest_angles[7]; // destined angles

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;

Servo servo[7] = {servo1, servo2, servo3, servo4, servo5, servo6, servo7};

int stepNum = 0;
int newCommand = 0;

void setup(){
	Serial.begin(9600);
 
	servo1.attach(2);  // attaches the servo on pin 2 to the servo object
	servo2.attach(3);  // attaches the servo on pin 3 to the servo object
	servo3.attach(4);  // attaches the servo on pin 4 to the servo object
	servo4.attach(5);  // attaches the servo on pin 5 to the servo object
	servo5.attach(6);  // attaches the servo on pin 6 to the servo object
	servo6.attach(7);  // attaches the servo on pin 7 to the servo object
  servo7.attach(8);  // attaches the servo on pin 7 to the servo object

	for(int i = 0; i < 7; i++) {
		servo[i].write(angle_init[i]);
    dest_angles[i] = cur_angles[i] = angle_init[i];
	}
}


void loop() {
  int servo = 0;
  int angle = 0;
  int i;


	if(newCommand) {
    dest_angles[servo] = angle * direction[servo] + angle_offset[servo];
		stepNum = 0;
    
		// move motors in many small steps to make motor move smooth, avoiding move motor suddently. The below is step calculation
		for(i = 0; i < 7; i++) {
			int dif = abs(cur_angles[i] - dest_angles[i]);
			int step = dif / angleSteps[i];

			if(stepNum < step)
				stepNum = step;
		}
	}

	// move motors step by step
	if(stepNum > 0) {
		for(i = 0; i < 7; i++) {
			int angleStepMove = (dest_angles[i] - cur_angles[i]) / stepNum;
			cur_angles[i] += angleStepMove;

			if(cur_angles[i] > angle_max[i])
				cur_angles[i] = angle_max[i];
			else if(cur_angles[i] < angle_min[i])
				cur_angles[i] = angle_min[i];
      
      switch(i) {
        case 0: servo1.write(cur_angles[i]); break;
        case 1: servo2.write(cur_angles[i]); break;
        case 2: servo3.write(cur_angles[i]); break;
        case 3: servo4.write(cur_angles[i]); break;
        case 4: servo5.write(cur_angles[i]); break;
        case 5: servo6.write(cur_angles[i]); break;
        case 6: servo7.write(cur_angles[i]); break;
      }
//			servo[i].write(cur_angles[i]);
		}

		stepNum--;
		delay(20);
	}
}
