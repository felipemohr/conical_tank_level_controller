#include "Arduino.h"

float flow_frequency = 0.0f; // Measures flow sensor pulses
float l_hour; // Calculated litres/hour
float total = 0.0;
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

void flow () // Interrupt function
{
  flow_frequency += 1.0;
}

void setup()
{
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
}

void loop ()
{
  currentTime = millis();
  cloopTime = currentTime;          // Updates cloopTime
  l_hour = 100.0 * flow_frequency / 7.5;  // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
  flow_frequency = 0;               // Reset Counter
  Serial.println(l_hour);      // Print litres/hour
  delay(10);
}
