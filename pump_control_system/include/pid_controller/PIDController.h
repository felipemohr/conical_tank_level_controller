#include "mbed.h"

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
  
  unsigned int _millis = 0;
  Ticker _ticker;

  void millisTicker();


public:
  PIDController(float kp, float ki, float kd);
  PIDController();
  ~PIDController();
  
  void setKPID(float kp, float ki, float kd);
  void setKp(float kp);
  void setKi(float ki);
  void setKd(float kd);

  void setSetpoint(float setpoint);
  float processPID(float curr_point);
};
