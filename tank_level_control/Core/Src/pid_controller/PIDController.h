/*
 * PIDController.h
 *
 *  Created on: Nov 26, 2021
 *      Author: Felipe Mohr
 */

#ifndef SRC_PID_CONTROLLER_PIDCONTROLLER_H_
#define SRC_PID_CONTROLLER_PIDCONTROLLER_H_

class PIDController
{
private:

  float _kp = 1.0;
  float _ki = 0.0;
  float _kd = 0.0;

  float _error;
  float _sum_error = 0.0;
  float _last_error = 0.0;
  float _delta_error;

  float _last_time = 0.0;
  float _curr_time;
  float _delta_time;

  float _setpoint;
  float _pid_result;

  float _pid_max;
  float _pid_min;

  unsigned int _millis = 0;

public:
  PIDController(float kp, float ki, float kd);
  PIDController();
  ~PIDController();

  void setKPID(float kp, float ki, float kd);
  void setKp(float kp);
  void setKi(float ki);
  void setKd(float kd);

  void setSetpoint(float setpoint);
  void setPIDLimits(float min, float max);
  float processPID(float curr_point);

  float getError();

  void millisTicker(void);

};
#endif /* SRC_PID_CONTROLLER_PIDCONTROLLER_H_ */
