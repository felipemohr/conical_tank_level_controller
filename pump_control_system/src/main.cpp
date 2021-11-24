#include "mbed.h"
#include "TextLCD.h"

#include "height_sensor/HeightSensor.h"
#include "pid_controller/PIDController.h"

#include <stdint.h>

#define SAMPLING_FREQUENCY 100.0f
#define LCD_UPDATE_TIME 1.0f

// Serial pc(PB_6, PB_7);

// LCD
TextLCD lcd(PA_8, PA_9, PA_10, PA_11, PA_12, PA_15);
Ticker lcd_ticker;
void updateLCD();
bool update_lcd = true;

// Interface
AnalogIn setpoint_pot(PA_0);
InterruptIn setpoint_but(PA_3);
float desired_height;

// Height Sensor
HeightSensor usensor(PB_10, PB_11);
Ticker sensor_ticker;
void readHeight();

// Valve
DigitalOut valve_direction(PB_12);
PwmOut valve_pwm(PB_13);
void valveInitialOpening();

// Pump
PIDController pump_pid_controller;
DigitalOut pump_en(PB_5);
PwmOut pump_pwm(PB_15);
float pump_pid_val;


int main()
{

  sensor_ticker.attach(&readHeight, 1/SAMPLING_FREQUENCY);
  lcd_ticker.attach(&updateLCD, LCD_UPDATE_TIME);



  // Read setpoint
  while(!valve_direction.read())
  {
    desired_height = 100*(0.35 + 0.30*setpoint_pot.read());
    lcd.cls();
    lcd.printf("Setpoint: %.1f cm", desired_height);
    wait_ms(300);

  }

  pump_pid_controller.setKPID(1.0, 0.0, 0.0);
  pump_pid_controller.setSetpoint(50.0);

  // Set valve initial opening
  valveInitialOpening();

  pump_en = 1;
  pump_pwm = 0.0;

  while(1)
  {
    if (update_lcd)
    {
      lcd.cls();
      lcd.printf("H: %.1f cm", usensor.getWaterHeight());
      update_lcd = false;
    }

    // Pump Controller
    pump_pid_val = pump_pid_controller.processPID(usensor.getWaterHeight());
    pump_pwm = max(0.5f, min(pump_pid_val, 1.0f) );
    if (pump_pwm < 0.5) pump_en = 0;
    else pump_en = 1;

    wait_ms(100);
  }

}




void updateLCD()
{
  update_lcd = true;
}

void readHeight()
{
  usensor.readSensor();
}

void valveInitialOpening()
{

}
