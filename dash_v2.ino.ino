#include <FastLED.h>


#include "Aqours.h"
//#include "Muse.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define VERSION "0.12b"

#define KEY_LEFT 9
#define KEY_RIGHT 7
#define KEY_SELECT 8

#define TONE_PITCH 3000

#define PIEZO 3

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// How many leds in your strip?
#define NUM_LEDS 3

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM warn[] =
{0x01, 0x80, 0x03, 0xc0, 0x02, 0x40, 0x06, 0x60, 0x04, 0x20, 0x0d, 0xb0, 0x09, 0x90, 0x19, 0x98, 0x11, 0x88, 0x31, 0x8c, 0x20, 0x04, 0x61, 0x86, 0x40, 0x02, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
static const unsigned char PROGMEM xiao[] =
{0x1 , 0x0 , 0x1 , 0x0 , 0x1 , 0x0 , 0x1 , 0x0 , 0x1 , 0x0 , 0x5 , 0x40 , 0x5 , 0x20 , 0x9 , 0x10 , 0x9 , 0x8 , 0x11 , 0x4 , 0x21 , 0x4 , 0x41 , 0x0 , 0x1 , 0x0 , 0x1 , 0x0 , 0x5 , 0x0 , 0x2 , 0x0};
static const unsigned char PROGMEM xin[] =
{0x2 , 0x0 , 0x1 , 0x0 , 0x0 , 0x80 , 0x0 , 0xc0 , 0x8 , 0x80 , 0x8 , 0x0 , 0x28 , 0x8 , 0x28 , 0x4 , 0x28 , 0x2 , 0x48 , 0x2 , 0x88 , 0x2 , 0x8 , 0x0 , 0x8 , 0x10 , 0x8 , 0x10 , 0x7 , 0xf0 , 0x0 , 0x0};
static const unsigned char PROGMEM gao[] =
{0x2 , 0x0 , 0x1 , 0x4 , 0xff , 0xfe , 0x0 , 0x0 , 0x1f , 0xf0 , 0x10 , 0x10 , 0x10 , 0x10 , 0x1f , 0xf0 , 0x0 , 0x4 , 0x7f , 0xfe , 0x40 , 0x4 , 0x4f , 0xe4 , 0x48 , 0x24 , 0x48 , 0x24 , 0x4f , 0xe4 , 0x40 , 0xc};
static const unsigned char PROGMEM wen[] =
{0x0 , 0x8 , 0x43 , 0xfc , 0x32 , 0x8 , 0x12 , 0x8 , 0x83 , 0xf8 , 0x62 , 0x8 , 0x22 , 0x8 , 0xb , 0xf8 , 0x10 , 0x0 , 0x27 , 0xfc , 0xe4 , 0xa4 , 0x24 , 0xa4 , 0x24 , 0xa4 , 0x24 , 0xa4 , 0x2f , 0xfe , 0x20 , 0x0};

int userSelection = 0;

int brightness = 64;

int idle = 0;

void showMenu(int index) {
  if (index > COLOUR_COUNT - 1) {
    return;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Charactor:"));
  display.println(names[index]);
  display.println(F(""));
  display.print(F("Brightness:"));
  double precent = (double)brightness / 255.0;
  display.print((int)(precent * 100));
  display.println(F("%"));
  display.println(F(""));
  display.print(F("dash.ino "));
  display.println(F(VERSION));
  //请尊重劳动成果，不要移除作者信息
  //Please respect the author and please do not remove the copyright information.
  display.print(F("(c)TEAM A72"));
  display.display();
}

void showWarning(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2 - 32,
    (display.height() - LOGO_HEIGHT) / 2 - 8,
    warn, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2 - 16,
    (display.height() - LOGO_HEIGHT) / 2 - 8,
    xiao, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2 - 8,
    xin, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2 + 16,
    (display.height() - LOGO_HEIGHT) / 2 - 8,
    gao, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2 + 32,
    (display.height() - LOGO_HEIGHT) / 2 - 8,
    wen, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setCursor(0, 32);
  display.println(F("    The LED can be"));
  display.println(F("  extremly hot while"));
  display.println(F("      operating."));
  display.display();
  delay(2000);
}

void showWelcome() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.println(F(" DASH.INO"));
  display.setTextSize(1);
  display.println(F("  open-source call"));
  display.println(F("   stick for ller!"));
  display.println(F(""));
  display.println(F(" Testing color LED..."));
  display.println(F(""));
  //请尊重劳动成果，不要移除作者信息
  //Please respect the author and please do not remove the copyright information.
  display.println(F("    2018 TEAM A72"));
  display.display();
  delay(0);
}

void setLed(CRGB colour) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = colour;
  }
  FastLED.show();
}

void testColour() {
  for (int index = 0; index < COLOUR_COUNT - 1; index++) {
    setLed(colour[index]);
    delay(100);
    // Now turn the LED off, then pause
    setLed(CRGB::Black);
    delay(100);
  }
}

void setup() {
  pinMode(KEY_SELECT, INPUT_PULLUP);
  pinMode(KEY_LEFT, INPUT_PULLUP);
  pinMode(KEY_RIGHT, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(100);
  tone(PIEZO, 261, 200);
  delay(200);
  tone(PIEZO, 329, 200);
  delay(200);
  tone(PIEZO, 392, 200);
  delay(200);
  tone(PIEZO, 523, 100);
  delay(100);
  tone(PIEZO, 510, 100);
  delay(100);
  tone(PIEZO, 523, 200);
  delay(200);
  display.clearDisplay();
  display.display();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);

  showWarning();
  showWelcome();


  testColour();
  setLed(colour[userSelection]);
  showMenu(userSelection);
}

bool keyPress(int key) {
  if (digitalRead(key) != LOW) {
    return false;
  } else {
    delay(20);
    if (digitalRead(key) != LOW) {
      return false;
    } else {
      return true;
    }
  }
}

void loop() {
  if(idle==300){
    display.clearDisplay();
    display.display();
  }else{
    idle++;
    }
  if (keyPress(KEY_RIGHT)) {
    idle=0;
    //Change charactor key
    userSelection++;
    if (userSelection > COLOUR_COUNT - 1) {
      userSelection = 0;
    }
    tone(PIEZO, TONE_PITCH, 100);
    showMenu(userSelection);
    delay(200);
  } else if (keyPress(KEY_LEFT)) {
    idle=0;
    //Change charactor key
    userSelection--;
    if (userSelection < 0) {
      userSelection = COLOUR_COUNT - 1;
    }
    tone(PIEZO, TONE_PITCH, 100);
    showMenu(userSelection);
    delay(200);
  } else if (keyPress(KEY_SELECT)) {
    idle=0;
    showMenu(userSelection);
    //Select charactor key
    tone(PIEZO, 2500, 100);
    setLed(colour[userSelection]);
    delay(100);
    tone(PIEZO, 3000, 100);
    delay(200);
  } else if (keyPress(10)) {
    idle=0;
    //Brightness up key
    brightness=brightness+8;
    if (brightness > 255) {
      //Check if brightness too much
      brightness = 255;
      FastLED.setBrightness(brightness);
      FastLED.show();
      tone(PIEZO, 500, 200);
      showMenu(userSelection);
      delay(200);
    } else {
      tone(PIEZO, TONE_PITCH, 100);
      FastLED.setBrightness(brightness);
      FastLED.show();
      showMenu(userSelection);
      delay(200);
    }
  }else if (keyPress(11)) {
    idle=0;
    //Brightness down key
    brightness=brightness-8;
    if (brightness < 0) {
      //Check if brightness too tow
      brightness = 0;
      FastLED.setBrightness(brightness);
      FastLED.show();
      tone(PIEZO, 500, 200);
      showMenu(userSelection);
      delay(200);
    } else {
      tone(PIEZO, TONE_PITCH, 100);
      FastLED.setBrightness(brightness);
      FastLED.show();
      showMenu(userSelection);
      delay(200);
    }
  }
  delay(20);
}
