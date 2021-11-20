#include "mbed.h"
#include "TextLCD.h"

TextLCD lcd(PA_8, PA_9, PA_10, PA_11, PA_12, PA_15);

DigitalOut led(PC_13);
AnalogIn pot(PA_0);
DigitalIn bot(PA_3);

int main()
{
  led = 0;

  while (1)
  {
    lcd.cls();
    
    // lcd.printf("Pot: %.3f", pot.read());
    // lcd.locate(0,1);
    // lcd.printf("Bot: %d", bot.read());



    led = !led;
    wait_ms(1000);
  }
}
