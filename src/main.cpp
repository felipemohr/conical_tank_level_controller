#include "mbed.h"
#include "TextLCD.h"

Serial pc(PA_9, PA_10);

DigitalOut led(PC_13);
AnalogIn pot1(PA_6);
AnalogIn pot2(PA_2);

int main() {

  led = 0;
  while(1)
  {
    led = !led;
    pc.printf("Pot 1 = %.2f \n", pot1.read()); 
    pc.printf("Pot 2 = %.2f \n\n", pot2.read()); 
    wait_ms(500);
  }
}
