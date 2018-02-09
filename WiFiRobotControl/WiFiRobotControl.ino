// Last edit 2016-1-18

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define DEBUG

#define MINI_PLAN_51MDII
//#define ROBOT_BOB

#ifdef MINI_PLAN_51MDII
  #include "MiniPlan51MDII.h"
#elif defined(ROBOT_BOB)
  #include "RobotBob.h"
#else
  #error "please define robot type" 
#endif

// Servo Delay Base Time
const int BASEDELAYTIME = 10;    // 10ms

// AP Password
const char WiFiAPPSK[] = "12345678";

int Servo_OE;
int Servo_PROGRAM;
int Servo_PROGRAM_Stack;
int GPIO_ID;

// Backup Servo Value
int Running_Servo_POS [ALLMATRIX];

#ifdef APDS9960_INT
#include <SparkFun_APDS9960.h>
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint8_t proximity_data = 0;
int proximity_isr_flag = 0;

void interruptRoutine() {
  proximity_isr_flag = 1;
}
#endif /* APDS9960_INT */

#ifdef EYES_LEDS_PIN
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(2, EYES_LEDS_PIN, NEO_GRB + NEO_KHZ800);
#endif /* EYES_LEDS_PIN */

ESP8266WebServer server(80);

#define STEP_NUMB(prog) (sizeof(prog) / (ALLMATRIX * sizeof(int)))

/*============================================================================*/
void Servo_PROGRAM_Zero()
{
  // Clear the current motor value
  for ( int Index = 0; Index < ALLMATRIX; Index++)
  {
    Running_Servo_POS[Index] = Servo_Act_0[Index] + (int8_t)EEPROM.read(Index);
  }

  // Reload the motor preset value
  for (int iServo = 0; iServo < ALLSERVOS; iServo++)
  {
    Set_PWM_to_Servo(iServo, Running_Servo_POS[iServo]);
    delay(10);
  }
}


/*============================================================================*/
void Servo_PROGRAM_Center()
{
  // Clear the current motor value
  for ( int Index = 0; Index < ALLMATRIX; Index++)
  {
    Running_Servo_POS[Index] = Servo_Act_1[Index] + (int8_t)EEPROM.read(Index);
  }

  // Reload the motor preset value
  for (int iServo = 0; iServo < ALLSERVOS; iServo++)
  {
    Set_PWM_to_Servo(iServo, Running_Servo_POS[iServo]);
    delay(10);
  }
}


/*============================================================================*/
void Servo_PROGRAM_Run(int iMatrix[][ALLMATRIX],  int iSteps)
{
  int INT_TEMP_A, INT_TEMP_B, INT_TEMP_C;

  for ( int MainLoopIndex = 0; MainLoopIndex < iSteps; MainLoopIndex++)  // iSteps Step the main loop
  {
    // InterTotalTime The total time for this step
    int InterTotalTime = iMatrix [ MainLoopIndex ] [ ALLMATRIX - 1 ] + (int8_t)EEPROM.read(ALLMATRIX - 1);

    // InterDelayCounter This step is basically delayed
    int InterDelayCounter = InterTotalTime / BASEDELAYTIME;

    // The number of bad times
    for ( int InterStepLoop = 0; InterStepLoop < InterDelayCounter; InterStepLoop++)
    {

      for (int ServoIndex = 0; ServoIndex < ALLSERVOS; ServoIndex++)  // Motor main loop
      {

        INT_TEMP_A = Running_Servo_POS[ServoIndex];                                           // The motor is now in position
        INT_TEMP_B = iMatrix[MainLoopIndex][ServoIndex] + (int8_t)EEPROM.read(ServoIndex);    // Motor target position

        if (INT_TEMP_A == INT_TEMP_B)         // The motor value remains unchanged
        {
          INT_TEMP_C = INT_TEMP_B;
        }

        else if (INT_TEMP_A > INT_TEMP_B)   // The motor value is reduced
        {
          // PWM internal difference value = map (execution time accumulation, start time, end time, internal difference start value, internal difference maximum value)
          INT_TEMP_C =  map(BASEDELAYTIME * InterStepLoop, 0, InterTotalTime, 0, INT_TEMP_A - INT_TEMP_B);

          if (INT_TEMP_A - INT_TEMP_C >= INT_TEMP_B)
          {
            Set_PWM_to_Servo(ServoIndex, INT_TEMP_A - INT_TEMP_C);
          }
        }

        else if (INT_TEMP_A < INT_TEMP_B)   // The motor value increases
        {
          // PWM internal difference value = map (execution time accumulation, start time, end time, internal difference start value, internal difference maximum value)
          INT_TEMP_C =  map(BASEDELAYTIME * InterStepLoop, 0, InterTotalTime, 0, INT_TEMP_B - INT_TEMP_A);

          if (INT_TEMP_A + INT_TEMP_C <= INT_TEMP_B)
          {
            Set_PWM_to_Servo(ServoIndex, INT_TEMP_A + INT_TEMP_C);
          }
        }

      }

      delay(BASEDELAYTIME);
    }

    // Back up the current motor value
    for ( int Index = 0; Index < ALLMATRIX; Index++)
    {
      Running_Servo_POS[Index] = iMatrix[MainLoopIndex][Index] + (int8_t)EEPROM.read(Index);
    }

  }

}


/*============================================================================*/
void handleAction(WiFiClient client, String req, HTTPMethod method)
{
  server.send(200, "text/plain", "Hello from " ROBOT_NAME " !");
}


/*============================================================================*/
void handleController()
{
  String pm = server.arg("pm");
  String pms = server.arg("pms");
  String servo = server.arg("servo");
  String setting = server.arg("setting");

  if (pm != "")
  {
    Servo_PROGRAM = pm.toInt();
  }

  if (pms != "")
  {
    Servo_PROGRAM_Stack = pms.toInt();
  }

  if (servo != "")
  {
    int Servo_ID = servo.toInt();
    String ival = server.arg("value");
    int Servo_PWM = ival.toInt() + (int8_t)EEPROM.read(Servo_ID);
    Set_PWM_to_Servo(Servo_ID, Servo_PWM);
  }

  if (setting != "")
  {
    int Servo_ID = setting.toInt();
    String ival = server.arg("value");
    int8_t Servo_PWM = ival.toInt();
    EEPROM.write(Servo_ID, Servo_PWM);
    EEPROM.commit();
  }

  server.send(200, "text/html", "(pm, pms)=(" + pm + "," + pms + ")");
}


/*============================================================================
  馬達歸零畫面
============================================================================*/
void handleZero()
{
  String content = "";
  content += "<html>";
  content += "<head>";
  content += "<title>";
  content +=  ROBOT_NAME;
  content +=  " Zero Check</title>";
  content += " <style type=\"text/css\">";
  content += "  body {";
  content += "    color: white;";
  content += "    background-color: #000000 }";

  content += "  .pm_btn {";
  content += "  width: 160px;";
  content += "  -webkit-border-radius: 5;";
  content += "  -moz-border-radius: 5;";
  content += "  border-radius: 5px;";
  content += "  font-family: Arial;";
  content += "  color: #ffffff;";
  content += "  font-size: 24px;";
  content += "  background: #3498db;";
  content += "  padding: 10px 20px 10px 20px;";
  content += "  text-decoration: none;";
  content += "}";
  content += ".pm_text {";
  content += "width: 160px;";
  content += "-webkit-border-radius: 5;";
  content += "-moz-border-radius: 5;";
  content += "border-radius: 5px;";
  content += "font-family: Arial;";
  content += "font-size: 24px;";

  content += "padding: 10px 20px 10px 20px;";
  content += "text-decoration: none;";
  content += "}";

  content += ".pm_btn:hover {";
  content += "  background: #3cb0fd;";
  content += "  background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "  text-decoration: none;";
  content += "}";

  content += ".pms_btn {";
  content += "width: 240px;";
  content += "-webkit-border-radius: 5;";
  content += "-moz-border-radius: 5;";
  content += "border-radius: 5px;";
  content += "font-family: Arial;";
  content += "color: #ffffff;";
  content += "font-size: 24px;";
  content += "background: #3498db;";
  content += "padding: 10px 20px 10px 20px;";
  content += "text-decoration: none;";
  content += "}";

  content += ".pms_btn:hover {";
  content += "background: #3cb0fd;";
  content += "background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "text-decoration: none;";
  content += "}";
  content += "  </style>";
  content += "</head>";
  content += "<body>";

  content += "<table>";
  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(14, 90)\">GPIO 14</button></td>";
  content += "<td><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(12, 90)\">GPIO 12</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "<table>";
  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(8,40)\">Servo 8</button></td>";
  content += "<td><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(7,80)\">Servo 7</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(9,100)\">Servo 9</button></td>";
  content += "<td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(6,20)\">Servo 6</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(10,20)\">Servo 10</button></td>";
  content += "<td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(5, 100)\">Servo 5</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "<table>";
  content += "<tr>";
  content += "<td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(11,30)\">Servo 11</button></td>";
  content += "<td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(4,80)\">Servo 4</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(12,55)\">Servo 12</button></td>";
  content += "<td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(3,60)\">Servo 3</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(13,35)\">Servo 13</button></td>";
  content += "<td><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(2,80)\">Servo 2</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(14,75)\">Servo 14</button></td>";
  content += "<td><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(1,35)\">Servo 1</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(15,50)\">Servo 15</button></td>";
  content += "<td><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(0,65)\">Servo 0</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "<table>";
  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #ed3db5;\" type=\"button\" onclick=\"controlPm(100)\">ALL</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "</body>";
  content += "<script>";
  content += "function controlServo(id, value) {";
  content += "  var xhttp = new XMLHttpRequest();";

  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";
  content += "     document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\", \"controller?servo=\"+id+\"&value=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";
  content += "function controlGpid(id, value) {";
  content += "  var xhttp = new XMLHttpRequest();";

  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";
  content += "     document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\", \"controller?gpid=\"+id+\"&value=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";

  content += "function controlPm(value) {";
  content += "  var xhttp = new XMLHttpRequest();";
  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";
  content += "     document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\", \"controller?pm=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";

  content += "</script>";
  content += "</html>";

  server.send(200, "text/html", content);
}


/*============================================================================
  簡易動作編輯畫面
============================================================================*/
void handleEditor()
{
  String content = "";
  content += "<html>";
  content += "<head>";
  content += "<title>";
  content += ROBOT_NAME;
  content += "Motion Editor</title>";
  content += "  <style type=\"text/css\">";
  content += "  body {";
  content += "    color: white;";
  content += "    background-color: #000000 }";

  content += "  .pm_btn {";
  content += "  width: 160px;";
  content += "  -webkit-border-radius: 5;";
  content += "  -moz-border-radius: 5;";
  content += "  border-radius: 5px;";
  content += "  font-family: Arial;";
  content += "  color: #ffffff;";
  content += "  font-size: 24px;";
  content += "  background: #3498db;";
  content += "  padding: 10px 20px 10px 20px;";
  content += "  text-decoration: none;";
  content += "}";
  content += ".pm_text {";
  content += "width: 160px;";
  content += "-webkit-border-radius: 5;";
  content += "-moz-border-radius: 5;";
  content += "border-radius: 5px;";
  content += "font-family: Arial;";
  content += "font-size: 24px;";

  content += "padding: 10px 20px 10px 20px;";
  content += "text-decoration: none;";
  content += "}";

  content += ".pm_btn:hover {";
  content += "  background: #3cb0fd;";
  content += "  background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "  text-decoration: none;";
  content += "}";

  content += ".pms_btn {";
  content += "width: 240px;";
  content += "-webkit-border-radius: 5;";
  content += "-moz-border-radius: 5;";
  content += "border-radius: 5px;";
  content += "font-family: Arial;";
  content += "color: #ffffff;";
  content += "font-size: 24px;";
  content += "background: #3498db;";
  content += "padding: 10px 20px 10px 20px;";
  content += "text-decoration: none;";
  content += "}";

  content += ".pms_btn:hover {";
  content += "background: #3cb0fd;";
  content += "background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "text-decoration: none;";
  content += "}";
  content += "  </style>";
  content += "</head>";
  content += "<body>";

  content += "<table>";
  content += "<tr>";
  content += "<td>GPIO 14  ,  Default value = 90<br/><input class=\"pm_text\" type=\"text\" id=\"gpid_14\" value=\"90\"><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(14, 'gpid_14')\">SEND</button></td>";
  content += "<td>GPIO 12  ,  Default value = 90<br/><input class=\"pm_text\" type=\"text\" id=\"gpid_12\" value=\"90\"><button class=\"pm_btn\" style=\"background: #6e6e6e;\" type=\"button\" onclick=\"controlGpid(12, 'gpid_12')\">SEND</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "<table>";
  content += "<tr>";
  content += "<td>Servo 8  ,  Default value = 40<br/><input class=\"pm_text\" type=\"text\" id=\"servo_8\" value=\"40\"><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(8,'servo_8')\">SEND</button></td>";
  content += "<td>Servo 7  ,  Default value = 80<br/><input class=\"pm_text\" type=\"text\" id=\"servo_7\" value=\"80\"><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(7, 'servo_7')\">SEND</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 9  ,  Default value = 100<br/><input class=\"pm_text\" type=\"text\" id=\"servo_9\" value=\"100\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(9,'servo_9')\">SEND</button></td>";
  content += "<td>Servo 6  ,  Default value = 20<br/><input class=\"pm_text\" type=\"text\" id=\"servo_6\" value=\"20\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(6,'servo_6')\">SEND</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 10  ,  Default value = 20<br/><input class=\"pm_text\" type=\"text\" id=\"servo_10\" value=\"20\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(10,'servo_10')\">SEND</button></td>";
  content += "<td>Servo 5  ,  Default value = 100<br/><input class=\"pm_text\" type=\"text\" id=\"servo_5\" value=\"100\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(5,'servo_5')\">SEND</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "<table>";
  content += "<tr>";
  content += "<td>Servo 11  ,  Default value = 30<br/><input class=\"pm_text\" type=\"text\" id=\"servo_11\" value=\"30\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(11,'servo_11')\">SEND</button></td>";
  content += "<td>Servo 4  ,  Default value = 80<br/><input class=\"pm_text\" type=\"text\" id=\"servo_4\" value=\"80\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(4,'servo_4')\">SEND</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 12  ,  Default value = 55<br/><input class=\"pm_text\" type=\"text\" id=\"servo_12\" value=\"55\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(12,'servo_12')\">SEND</button></td>";
  content += "<td>Servo 3  ,  Default value = 60<br/><input class=\"pm_text\" type=\"text\" id=\"servo_3\" value=\"60\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(3,'servo_3')\">SEND</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 13  ,  Default value = 35<br/><input class=\"pm_text\" type=\"text\" id=\"servo_13\" value=\"35\"><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(13,'servo_13')\">SEND</button></td>";
  content += "<td>Servo 2  ,  Default value = 80<br/><input class=\"pm_text\" type=\"text\" id=\"servo_2\" value=\"80\"><button class=\"pm_btn\" style=\"background: #f5da81;\" type=\"button\" onclick=\"controlServo(2,'servo_2')\">SEND</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 14  ,  Default value = 75<br/><input class=\"pm_text\" type=\"text\"  id=\"servo_14\" value=\"75\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(14,'servo_14')\">SEND</button></td>";
  content += "<td>Servo 1  ,  Default value = 35<br/><input class=\"pm_text\" type=\"text\" id=\"servo_1\" value=\"35\"><button class=\"pm_btn\" style=\"background: #bdbdbd;\" type=\"button\" onclick=\"controlServo(1,'servo_1')\">SEND</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 15  ,  Default value = 50<br/><input class=\"pm_text\" type=\"text\"  id=\"servo_15\" value=\"50\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(15,'servo_15')\">SEND</button></td>";
  content += "<td>Servo 0  ,  Default value = 65<br/><input class=\"pm_text\" type=\"text\" id=\"servo_0\" value=\"65\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(0,'servo_0')\">SEND</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "<table>";
  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #ed3db5;\" type=\"button\" onclick=\"controlPm(99)\">STANDBY POSE</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "</body>";
  content += "<script>";

  content += "function controlServo(id, textId) {";
  content += "  var xhttp = new XMLHttpRequest();";
  content += "  var value = document.getElementById(textId).value;";
  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";
  content += "     document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\",\"controller?servo=\"+id+\"&value=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";
  content += "function controlGpid(id, textId) {";
  content += "  var xhttp = new XMLHttpRequest();";
  content += "  var value = document.getElementById(textId).value;";
  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";
  content += "     document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\", \"controller?gpid=\"+id+\"&value=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";

  content += "function controlPm(value) {";
  content += "  var xhttp = new XMLHttpRequest();";
  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";
  content += "     document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\", \"controller?pm=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";

  content += "</script>";
  content += "</html>";
  server.send(200, "text/html", content);
}


/*============================================================================
  Motor correction screen
============================================================================*/
void handleSetting()
{
  String content = "";
  content += "<html>";
  content += "<head>";
  content += "  <title>";
  content += ROBOT_NAME;
  content += " Setting</title>";
  content += "  <style type=\"text/css\">";
  content += "  body {";
  content += "    color: white;";
  content += "    background-color: #000000 }";

  content += "  .pm_btn {";
  content += "  width: 120px;";
  content += "  -webkit-border-radius: 5;";
  content += "  -moz-border-radius: 5;";
  content += "  border-radius: 5px;";
  content += "  font-family: Arial;";
  content += "  color: #ffffff;";
  content += "  font-size: 24px;";
  content += "  background: #3498db;";
  content += "  padding: 10px 20px 10px 20px;";
  content += "  text-decoration: none;";
  content += "}";
  content += ".pm_text {";
  content += "width: 80px;";
  content += "-webkit-border-radius: 5;";
  content += "-moz-border-radius: 5;";
  content += "border-radius: 5px;";
  content += "font-family: Arial;";
  content += "font-size: 24px;";

  content += "padding: 10px 20px 10px 20px;";
  content += "text-decoration: none;";
  content += "}";

  content += ".pm_btn:hover {";
  content += "  background: #3cb0fd;";
  content += "  background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "  text-decoration: none;";
  content += "}";

  content += ".pms_btn {";
  content += "width: 160px;";
  content += "-webkit-border-radius: 5;";
  content += "-moz-border-radius: 5;";
  content += "border-radius: 5px;";
  content += "font-family: Arial;";
  content += "color: #ffffff;";
  content += "font-size: 24px;";
  content += "background: #3498db;";
  content += "padding: 10px 20px 10px 20px;";
  content += "text-decoration: none;";
  content += "}";

  content += ".pms_btn:hover {";
  content += "background: #3cb0fd;";
  content += "background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "text-decoration: none;";
  content += "}";
  content += "  </style>";
  content += "</head>";
  content += "<body>";

  content += "<table>";
  content += "<tr>";
  content += "<td>GPIO 14<br/><input class=\"pm_text\" type=\"text\" id=\"servo_17\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(17,'servo_17')\">SET</button></td>";
  content += "<td>GPIO 12<br/><input class=\"pm_text\" type=\"text\" id=\"servo_16\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(16,'servo_16')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 8<br/><input class=\"pm_text\" type=\"text\" id=\"servo_8\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(8,'servo_8')\">SET</button></td>";
  content += "<td>Servo 7<br/><input class=\"pm_text\" type=\"text\" id=\"servo_7\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(7,'servo_7')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 9<br/><input class=\"pm_text\" type=\"text\" id=\"servo_9\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(9,'servo_9')\">SET</button></td>";
  content += "<td>Servo 6<br/><input class=\"pm_text\" type=\"text\" id=\"servo_6\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(6,'servo_6')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 10<br/><input class=\"pm_text\" type=\"text\" id=\"servo_10\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(10,'servo_10')\">SET</button></td>";
  content += "<td>Servo 5<br/><input class=\"pm_text\" type=\"text\" id=\"servo_5\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(5,'servo_5')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 11<br/><input class=\"pm_text\" type=\"text\" id=\"servo_11\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(11,'servo_11')\">SET</button></td>";
  content += "<td>Servo 4<br/><input class=\"pm_text\" type=\"text\" id=\"servo_4\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(4,'servo_4')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 12<br/><input class=\"pm_text\" type=\"text\" id=\"servo_12\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(12,'servo_12')\">SET</button></td>";
  content += "<td>Servo 3<br/><input class=\"pm_text\" type=\"text\" id=\"servo_3\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(3,'servo_3')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 13<br/><input class=\"pm_text\" type=\"text\" id=\"servo_13\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(13,'servo_13')\">SET</button></td>";
  content += "<td>Servo 2<br/><input class=\"pm_text\" type=\"text\" id=\"servo_2\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(2,'servo_2')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 14<br/><input class=\"pm_text\" type=\"text\" id=\"servo_14\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(14,'servo_14')\">SET</button></td>";
  content += "<td>Servo 1<br/><input class=\"pm_text\" type=\"text\" id=\"servo_1\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(1,'servo_1')\">SET</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td>Servo 15<br/><input class=\"pm_text\" type=\"text\" id=\"servo_15\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(15,'servo_15')\">SET</button></td>";
  content += "<td>Servo 0<br/><input class=\"pm_text\" type=\"text\" id=\"servo_0\" value=\"0\"><button class=\"pm_btn\" type=\"button\" onclick=\"controlServo(0,'servo_0')\">SET</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "<table>";
  content += "<tr>";
  content += "<td><button class=\"pm_btn\" style=\"background: #ed3db5;\" type=\"button\" onclick=\"controlPm(999)\">RESET</button></td>";
  content += "</tr>";
  content += "</table>";

  content += "<br>";

  content += "</body>";
  content += "<script>";

  content += "function controlServo(id, textId) {";
  content += "  var xhttp = new XMLHttpRequest();";
  content += "  var value = document.getElementById(textId).value;";
  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";
  content += "     document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\",\"controller?setting=\"+id+\"&value=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";
  content += "function controlGpid(id, textId) {";
  content += "  var xhttp = new XMLHttpRequest();";
  content += "  var value = document.getElementById(textId).value;";
  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";
  content += "     document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\", \"controller?gpid=\"+id+\"&value=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";

  content += "function controlPm(value) {";
  content += "  var xhttp = new XMLHttpRequest();";
  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";
  content += "     document.getElementById(\"demo\").innerHTML = xhttp.responseText;";
  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\", \"controller?pm=\"+value, true);";
  content += "  xhttp.send();";
  content += "}";

  content += "</script>";
  content += "</html>";
  server.send(200, "text/html", content);
}


/*============================================================================
  Master screen
============================================================================*/
void handleIndex()
{
  String content = "";
  content += "<html>";

  content += "<head>";
  content += "  <title>";
  content += ROBOT_NAME;
  content += " Controller</title>";
  content += "  <style type=\"text/css\">";
  content += "  body {";
  content += "    color: purple;";
  content += "    background-color: #000000 }";

  content += "  .pm_btn {";
  content += "  width: 160px;";
  content += "  -webkit-border-radius: 5;";
  content += "  -moz-border-radius: 5;";
  content += "  border-radius: 5px;";
  content += "  font-family: Arial;";
  content += "  color: #ffffff;";
  content += " font-size: 24px;";
  content += "background: #3498db;";
  content += "  padding: 10px 20px 10px 20px;";
  content += "  text-decoration: none;";
  content += "}";

  content += ".pm_btn:hover {";
  content += "  background: #3cb0fd;";
  content += "  background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "  background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "  text-decoration: none;";
  content += "}";

  content += ".pms_btn {";
  content += "width: 240px;";
  content += "-webkit-border-radius: 5;";
  content += "-moz-border-radius: 5;";
  content += "border-radius: 5px;";
  content += "font-family: Arial;";
  content += "color: #ffffff;";
  content += "font-size: 24px;";
  content += "background: #3498db;";
  content += "padding: 10px 20px 10px 20px;";
  content += "text-decoration: none;";
  content += "}";

  content += ".pms_btn:hover {";
  content += "background: #3cb0fd;";
  content += "background-image: -webkit-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -moz-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -ms-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: -o-linear-gradient(top, #3cb0fd, #3498db);";
  content += "background-image: linear-gradient(to bottom, #3cb0fd, #3498db);";
  content += "text-decoration: none;";
  content += "}";
  content += " </style>";
  content += "</head>";


  content += "<body>";
  content += "<table>";
  content += "<tr>";
  content += "<td></td>";
  content += "<td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(1)\">Forward</button></td>";
  content += "<td></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(3)\">Left</button></td>";
  content += "<td><button  class=\"pm_btn\" style=\"background: #ed3db5;\" type=\"button\" onclick=\"controlPm(99)\">STOP</button></td>";
  content += "<td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(4)\">Right</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td></td>";
  content += "<td><button  class=\"pm_btn\" type=\"button\" onclick=\"controlPm(2)\">Backward</button></td>";
  content += "<td></td>";
  content += "</tr>";
  content += "</table>";

  content += "<table>";
  content += "<tr>";
  content += "<td><button class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(1)\">Bow</button></td>";
  content += "<td><button  class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(4)\">Apache</button></td>";
  content += "</tr>";
  content += "<tr>";
  content += "<td><button class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(2)\">Waving</button></td>";
  content += "<td><button  class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(5)\">Balance</button></td>";
  content += "</tr>";
  content += "<tr>";
  content += "<td><button class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(3)\">Iron Man</button></td>";
  content += "<td><button  class=\"pms_btn\" style=\"background: #ffbf00;\" type=\"button\" onclick=\"controlPms(6)\">Warm-Up</button></td>";
  content += "</tr>";

  content += "<tr>";
  content += "<td colspan=\"2\"><center><button  class=\"pms_btn\" style=\"background: #04b404;\" type=\"button\" onclick=\"controlPms(99)\">Auto</button></center></td>";
  content += "</tr>";
  content += "</table>";

  content += "</body>";
  content += "<script>";
  content += "function controlPm(id) {";
  content += "var xhttp = new XMLHttpRequest();";
  content += "xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";

  content += "    }";
  content += "  };";
  content += "  xhttp.open(\"GET\", \"controller?pm=\"+id, true);";
  content += "  xhttp.send();";
  content += "}";
  content += "function controlPms(id) {";
  content += "  var xhttp = new XMLHttpRequest();";
  content += "  xhttp.onreadystatechange = function() {";
  content += "    if (xhttp.readyState == 4 && xhttp.status == 200) {";

  content += " }";
  content += "  };";
  content += "  xhttp.open(\"GET\", \"controller?pms=\"+id, true);";
  content += "  xhttp.send();";
  content += "}";
  content += "</script>";
  content += "</html>";

  server.send(200, "text/html", content);
}


/*============================================================================*/
void enableWebServer()
{
  HTTPMethod postMethod = HTTP_GET;

  server.on("/controller", postMethod, handleController);

  server.on("/", postMethod, handleIndex);
  server.on("/editor", postMethod, handleEditor);
  server.on("/zero", postMethod, handleZero);
  server.on("/setting", postMethod, handleSetting);

  server.begin();
}

#ifdef APDS9960_INT
void initProximitySensor()
{
  pinMode(APDS9960_INT, INPUT);
  
  // Initialize interrupt service routine
  attachInterrupt(APDS9960_INT, interruptRoutine, FALLING);
  
  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    apds.setProximityGain(PGAIN_2X);
    apds.setProximityIntLowThreshold(PROX_INT_LOW);
    apds.setProximityIntHighThreshold(PROX_INT_HIGH);
    apds.enableProximitySensor(true);
    apds.enableGestureSensor(true);
#ifdef DEBUG
    Serial.println(F("APDS-9960 initialization complete"));
#endif
  } 
}

void handleProximitySensor() {
    
    if(!apds.readProximity(proximity_data))
      apds.clearProximityInt();  
    
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("UP");
        break;
      case DIR_DOWN:
        Serial.println("DOWN");
        break;
      case DIR_LEFT:
        Serial.println("LEFT");
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT");
        break;
      case DIR_NEAR:
        Serial.println("NEAR");
        break;
      case DIR_FAR:
        Serial.println("FAR");
        break;
      default:
        Serial.println("NONE");
    }
  }
}

#endif


/*============================================================================*/
void setup(void) {

  // PCA9685 OE PIN
  //Serial.begin(9600);
  //Serial.println("miniPlen start");
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // Initialize I2C
  Wire.begin(4, 5);

  servoInit();

  // AP SSID Name
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ROB" + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, AP_NameString.length() + 1, 0);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
  IPAddress myIP = WiFi.softAPIP();


  // EEPROM
  EEPROM.begin(512);
  delay(10);


  // Clear the current motor value
  for ( int Index = 0; Index < ALLMATRIX; Index++)
  {
    Running_Servo_POS[Index] = Servo_Act_1[Index] + (int8_t)EEPROM.read(Index);
  }


  // Page formation
  enableWebServer();
}



/*============================================================================*/
void loop(void) {

  server.handleClient();

  if (Servo_PROGRAM >= 1 )
  {
    switch (Servo_PROGRAM)
    {
      case 1:  // Go ahead
        Servo_PROGRAM_Run(Servo_Prg_10, STEP_NUMB(Servo_Prg_10));
        Servo_PROGRAM_Center();
        break;
      case 2:  // Backwards
        Servo_PROGRAM_Run(Servo_Prg_11, STEP_NUMB(Servo_Prg_11));
        Servo_PROGRAM_Center();
        break;
      case 3:  // Turn left
        Servo_PROGRAM_Run(Servo_Prg_12, STEP_NUMB(Servo_Prg_12));
        Servo_PROGRAM_Center();
        break;
      case 4:  // Turn right
        Servo_PROGRAM_Run(Servo_Prg_13, STEP_NUMB(Servo_Prg_13));
        Servo_PROGRAM_Center();
        break;
      case 99:  // Standby
        Servo_PROGRAM_Center();
        delay(300);
        break;
      case 100:  // Standby
        Servo_PROGRAM_Zero();
        delay(300);
        break;
      case 999:  // Clears the EEPROM correction value
        for ( int Index = 0; Index < ALLMATRIX; Index++)
        {
          EEPROM.write(Index, 0);
        }
        EEPROM.commit();
        break;
    }
    Servo_PROGRAM = 0;
  }

  if (Servo_PROGRAM_Stack >= 1)
  {
    switch (Servo_PROGRAM_Stack)
    {
      case 1:  // Action 1
        Servo_PROGRAM_Run(Servo_Prg_1, STEP_NUMB(Servo_Prg_1));
        Servo_PROGRAM_Center();
        break;
      case 2:  // Action 2
        Servo_PROGRAM_Run(Servo_Prg_2, STEP_NUMB(Servo_Prg_2));
        Servo_PROGRAM_Center();
        break;
      case 3:  // Action 3
        Servo_PROGRAM_Run(Servo_Prg_3, STEP_NUMB(Servo_Prg_3));
        Servo_PROGRAM_Center();
        break;
      case 4:  // Action 4
        Servo_PROGRAM_Run(Servo_Prg_4, STEP_NUMB(Servo_Prg_4));
        Servo_PROGRAM_Center();
        break;
      case 5:  // Action 5
        Servo_PROGRAM_Run(Servo_Prg_5, STEP_NUMB(Servo_Prg_5));
        Servo_PROGRAM_Center();
        break;
      case 6:  // Action 6
        Servo_PROGRAM_Run(Servo_Prg_6, STEP_NUMB(Servo_Prg_6));
        Servo_PROGRAM_Center();
        break;
      case 99:  // Action AUTO DEMO
        Servo_PROGRAM_Run(Servo_Prg_1, STEP_NUMB(Servo_Prg_1));
        Servo_PROGRAM_Center();
        delay(1000);
        Servo_PROGRAM_Run(Servo_Prg_2, STEP_NUMB(Servo_Prg_2));
        Servo_PROGRAM_Center();
        delay(1000);
        Servo_PROGRAM_Run(Servo_Prg_6, STEP_NUMB(Servo_Prg_6));
        Servo_PROGRAM_Center();
        delay(1000);
        Servo_PROGRAM_Run(Servo_Prg_3, STEP_NUMB(Servo_Prg_3));
        Servo_PROGRAM_Center();
        delay(1000);
        Servo_PROGRAM_Run(Servo_Prg_4, STEP_NUMB(Servo_Prg_4));
        Servo_PROGRAM_Center();
        delay(1000);
        Servo_PROGRAM_Run(Servo_Prg_5, STEP_NUMB(Servo_Prg_5));
        Servo_PROGRAM_Center();
        delay(1000);
        Servo_PROGRAM_Run(Servo_Prg_1, STEP_NUMB(Servo_Prg_1));
        Servo_PROGRAM_Center();
        break;
    }
    Servo_PROGRAM_Stack = 0;
  }

}



