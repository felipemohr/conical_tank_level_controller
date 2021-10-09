#include "mbed.h"

class PIDController
{
private:
  float __kp = 1.0;
  float __ki = 0.0;
  float __kd = 0.0;

  float __error;
  float __sum_error = 0.0;
  float __last_error = 0.0;
  float __delta_error;
  
  float __last_time = 0.0;
  float __curr_time;
  float __delta_time;

  float __setpoint;
  float __pid_result;
  
  int __millis = 0;
  Ticker __ticker;

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
