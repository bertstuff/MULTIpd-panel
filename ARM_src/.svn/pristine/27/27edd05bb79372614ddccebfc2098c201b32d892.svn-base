#include "PID.h"

uint32_t PID_Calc(PID_t PID){
  float derivative, dt, output, error;

  dt = (PID.interval / CLOCK_SECOND);
  error = PID.setpoint - PID.actual_value;
  PID.data.integral += (error*dt);
  if(PID.data.integral > PID.I_max){
	  PID.data.integral = PID.I_max;
  }
  if(PID.data.integral < (0-PID.I_max)){
  	  PID.data.integral = 0-PID.I_max;
  }

  derivative = (error - PID.data.previous_error)/dt;
  output = (PID.P*error) + (PID.I*PID.data.integral) + (PID.D*derivative);
  PID.data.previous_error = error;

  return (uint32_t)output;
} 
