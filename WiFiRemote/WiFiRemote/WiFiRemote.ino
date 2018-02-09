#include <SPI.h>  
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

const int sclPin = D1;
const int sdaPin = D2;

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
int red = 0;
unsigned int raw;
 
void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);
  Wire.begin(sdaPin, sclPin);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello from:");
  display.println("http://arduino-er.blogspot.com/");
  display.display();

  Serial.swap();

}

void loop() {
  
  // put your main code here, to run repeatedly:
  raw = analogRead(A0);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Voltage:");
  display.println(((float)raw * 11.0) / 1024);
  display.println("Raw:");
  display.println(raw);
  display.display();
  Serial.print("R");
  Serial.println(red);
  red++;
  delay(1500);
}

