#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS1307 rtc;

void setup() {
  Serial.begin(115200);

  // I2C: SDA = 21, SCL = 19
  Wire.begin(21, 19);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);
  }

  // RTC init
  if (!rtc.begin()) {
    displayError("RTC not found");
    for (;;);
  }

  // Set RTC time ONCE if not running
  if (!rtc.isrunning()) {
    // Sets RTC to compile time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  DateTime now = rtc.now();
  drawTime(now);
  delay(1000);
}

void drawTime(DateTime now) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ESP32 RTC Clock");

  display.setTextSize(2);
  display.setCursor(5, 25);

  if (now.hour() < 10) display.print("0");
  display.print(now.hour());
  display.print(":");

  if (now.minute() < 10) display.print("0");
  display.print(now.minute());
  display.print(":");

  if (now.second() < 10) display.print("0");
  display.print(now.second());

  display.display();
}

void displayError(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 30);
  display.println(msg);
  display.display();
}
