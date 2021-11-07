#include "mbed.h"
// #include "TextLCD.h"

InterruptIn pid_button(PB_10);
InterruptIn level_button(PB_11);

Timeout timeout;

Serial pc(PA_9, PA_10);

void pid_rise();
void pid_fall();
void level_rise();

void set_pid_state();
void set_next_pid_state();
void set_level_state();
void set_state();
void set_pid();

static float debounce_time = 0.5;

enum
{
  INIT, OPERATION, SET_LEVEL,
  SET_KP, SET_KI, SET_KD
};

char states[6] = {'0', 'o', 'l', 'p', 'i', 'd'};
int state = INIT;
int next_state = OPERATION;


int main() {

  state = OPERATION;

  pid_button.rise(&pid_rise);
  pid_button.fall(&pid_fall);
  level_button.rise(&level_rise);
  
  while(1)
  {

  }
}

void pid_rise()
{
  timeout.attach(&set_next_pid_state, debounce_time);
}

void pid_fall()
{
  timeout.attach(&set_state, debounce_time);
}

void set_state()
{
  state = next_state;
  pc.printf("State: %c\n", states[state]);
}

void level_rise()
{
  timeout.attach(&set_level_state, debounce_time);
}

void set_level_state()
{
  if(state == OPERATION)
  {
    next_state = SET_LEVEL;
    set_state();
  }
  else if(state == SET_LEVEL)
  {
    next_state = OPERATION;
    set_state();
  }
}

void set_next_pid_state()
{
  if (state == OPERATION)
  {
    next_state = SET_KP;
    pc.printf("next state SET_KP\n");
  } 
  else if (state == SET_KP)
  {
    next_state = SET_KI;
    pc.printf("next state SET_KI\n");
    return;
  }
  else if (state == SET_KI)
  { 
    next_state = SET_KD;
    pc.printf("next state SET_KD\n");
    return;
  }
  else if (state == SET_KD){
    next_state = OPERATION;
    pc.printf("next state Operation\n");
    return;
  }
  timeout.attach(&set_pid_state, 3.0);
}

void set_pid_state()
{
  if(next_state == SET_KP)
  {
    next_state = SET_KI;
    pc.printf("NEXT STATE SET_KI\n");
    timeout.attach(&set_pid_state, 3.0);
    return;
  }
  else if(next_state == SET_KI)
  {
    next_state = SET_KD;
    pc.printf("NEXT STATE SET_KD\n");
    return;
  }

}