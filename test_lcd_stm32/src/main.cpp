#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(PA_8, PA_9, PA_10, PA_11, PA_12, PA_15);

void setup() {
  lcd.begin(16,2);
  lcd.print("hello, world!");
}

void loop() {

}
