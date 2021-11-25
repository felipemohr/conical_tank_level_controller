#include "mbed.h"
#include "TextLCD.h"

#include "height_sensor/HeightSensor.cpp"
#include "pid_controller/PIDController.cpp"

#include <stdint.h>

#define SAMPLING_FREQUENCY 100.0f
#define LCD_UPDATE_TIME 1.0f

// Serial pc(PB_6, PB_7);


// Ticker
Ticker millis_ticker;
void tickerMillis();

// LCD
TextLCD lcd(PA_8, PA_9, PA_10, PA_11, PA_12, PA_15);
int lcd_millis_cont = 0;

// Interface
AnalogIn setpoint_pot(PA_0);
DigitalIn setpoint_but(PA_3);
// InterruptIn setpoint_but(PA_3);
float desired_height;

// Height Sensor
HeightSensor usensor(PB_10, PB_11);
// Ticker sensor_ticker;
// void readHeight();

// Valve
DigitalOut valve_direction(PB_12);
PwmOut valve_pwm(PB_13);
void valveInitialOpening();

// Pump
PIDController pump_pid_controller;
DigitalOut pump_en(PB_5);
PwmOut pump_pwm(PB_15);


int main()
{

  // sensor_ticker.attach(&readHeight, 1/SAMPLING_FREQUENCY);
  millis_ticker.attach(&tickerMillis, 0.001);


  // Read setpoint
  while(!setpoint_but.read())
  {
    desired_height = 100*(0.35 + 0.30*setpoint_pot.read());
    lcd.cls();
    lcd.printf("Setpoint: ");
    lcd.locate(9,1);
    lcd.printf("%.1f cm", desired_height);
    wait_ms(300);

  }

  pump_pid_controller.setKPID(1.0, 0.002, 0.0);
  pump_pid_controller.setPIDLimits(0.5f, 1.0f);
  pump_pid_controller.setSetpoint(desired_height/100);

  // Set valve initial opening
  valveInitialOpening();

  pump_en = 1;
  pump_pwm = 0.0;

  while(1)
  {

    usensor.readSensor();

    // readHeight();
    // Pump Controller
    pump_pwm = pump_pid_controller.processPID(usensor.getWaterHeight()/100);
    if (pump_pwm.read() < 0.5) pump_en = 0;
    else pump_en = 1;


    if (lcd_millis_cont > 1000*int(LCD_UPDATE_TIME))
    {
      lcd_millis_cont = 0;
      lcd.cls();
      lcd.printf("H: %.1f", usensor.getWaterHeight());
      lcd.locate(0,1);
      lcd.printf("PID: %.3f", pump_pwm.read());
    }

    wait_ms(1000/SAMPLING_FREQUENCY);
  }

}


void valveInitialOpening()
{
  // valve_direction = 1;
  // valve_pwm = 1;
  // wait_ms(10000);
  // valve_direction = 0;
  // wait_ms(4000);
  // valve_pwm = 1;
}

void tickerMillis()
{
  pump_pid_controller.millisTicker();
  lcd_millis_cont++;
}
