#include "mbed.h"


// Valve
DigitalOut valve_direction(PB_12);
PwmOut valve_pwm(PB_13);

int main()
{


  valve_direction = 1;
  valve_pwm = 0.1;
  wait_ms(10000);
  valve_direction = 0;
  wait_ms(10000);
  valve_pwm = 0.1;


  while(1)
  {

    wait_ms(10);
  }

}


