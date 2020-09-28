#ifndef __PID_H__
#define __PID_H__

#include <Core/Type/type.h>
#include <Core/Timer/timer.h>

typedef struct{
  float previous_error;
  float integral;
}PID_data_t;

typedef struct{
  int32_t actual_value;
  int32_t setpoint;
  clock_time_t interval;
  float P;
  float I;
  float I_max;
  float D;
  PID_data_t data;
}PID_t;

uint32_t PID_Calc(PID_t PID);

#endif /* __PID_H__ */
