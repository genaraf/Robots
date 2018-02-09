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

void setup() {
  pinMode(A0, INPUT);
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

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int raw = AnalogRead(A0);
  display.setCursor(0,0);
  display.println("Voltage:", ((float)raw * 8.4) / 1024);
  display.println("Raw:", raw);
}

