#include "pid_controller/PIDController.h"

PIDController::PIDController()
{
  this->__ticker.attach(PIDController::millisTicker, 0.001);
}

PIDController::PIDController(float kp, float ki, float kd)
  : __kp(kp), __ki(ki), __kd(kd)
{
  this->__ticker.attach(PIDController::millisTicker, 0.001);
}

PIDController::~PIDController()
{
}

void PIDController::millisTicker()
{
  this->__millis++;
}

void PIDController::setKPID(float kp, float ki, float kd)
{
  this->__kp = kp;
  this->__ki = ki;
  this->__kd = kd;
}

void PIDController::setKp(float kp)
{
  this->__kp = kp;
}

void PIDController::setKi(float ki)
{
  this->__ki = ki;
}

void PIDController::setKd(float kd)
{
  this->__kd = kd;
}

void PIDController::setSetpoint(float setpoint)
{
  this->__setpoint = setpoint;
}

float PIDController::processPID(float curr_point)
{
  this->__curr_time = this->__millis / 1000;
  this->__delta_time = this->__curr_time - this->__last_time;

  this->__error = curr_point - this->__setpoint;
  this->__sum_error += this->__error;
  this->__delta_error = this->__error - this->__last_error;

  this->__pid_result = this->__kp * this->__error
                     + this->__ki * this->__sum_error * this->__curr_time
                     + this->__kd * this->__delta_error / this->__delta_time;

  this->__last_time = this->__curr_time;
  this->__last_error = this->__error;

  return this->__pid_result;
}
