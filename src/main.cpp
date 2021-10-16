#include <mbed.h>

DigitalOut led(PC_13);

int main()
{
  led = 1;
  while (1)
  {
    led = !led;
    wait_ms(100);
  }
  
}
