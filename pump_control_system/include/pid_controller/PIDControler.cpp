#include "pid_controller/PIDController.h"

PIDController::PIDController()
{
  this->_ticker.attach(PIDController::millisTicker, 0.001);
}

PIDController::PIDController(float kp, float ki, float kd)
  : _kp(kp), _ki(ki), _kd(kd)
{
  this->_ticker.attach(PIDController::millisTicker, 0.001);
}

PIDController::~PIDController()
{
}

void PIDController::millisTicker()
{
  this->_millis++;
}

void PIDController::setKPID(float kp, float ki, float kd)
{
  this->_kp = kp;
  this->_ki = ki;
  this->_kd = kd;
}

void PIDController::setKp(float kp)
{
  this->_kp = kp;
}

void PIDController::setKi(float ki)
{
  this->_ki = ki;
}

void PIDController::setKd(float kd)
{
  this->_kd = kd;
}

void PIDController::setSetpoint(float setpoint)
{
  this->_setpoint = setpoint;
}

float PIDController::processPID(float curr_point)
{
  this->_curr_time = this->_millis / 1000;
  this->_delta_time = this->_curr_time - this->_last_time;

  this->_error = curr_point - this->_setpoint;
  this->_sum_error  += this->_error;
  this->_delta_error = this->_error - this->_last_error;

  this->_pid_result  = this->_kp * this->_error
                     + this->_ki * this->_sum_error * this->_curr_time
                     + this->_kd * this->_delta_error / this->_delta_time;

  this->_last_time = this->_curr_time;
  this->_last_error = this->_error;

  return this->_pid_result;
}
