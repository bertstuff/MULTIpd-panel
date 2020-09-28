#include "I2C_Dev_Init.h"
#include <Driver/include/lpc17xx_i2c.h>
#include <Driver/include/lpc17xx_pinsel.h>

//////////////Global variables////////////
 uint32_t I2C_Cur_Speed = 0;
 
/////////////Global functions////////////
 void I2C_Dev_Init(uint32_t Max_speed_Hz)
 {
  if((I2C_Cur_Speed == 0)||(I2C_Cur_Speed > Max_speed_Hz) )
  {
    PINSEL_CFG_Type pinsel;  
    I2C_Init(LPC_I2C1, Max_speed_Hz);
    I2C_Cur_Speed = Max_speed_Hz;
    //p0[19]->sda1
    pinsel.Portnum = PINSEL_PORT_0;
    pinsel.Pinnum = PINSEL_PIN_19;
    pinsel.Funcnum = PINSEL_FUNC_3;
    pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;
    pinsel.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
    PINSEL_ConfigPin(&pinsel);

    //p0[20]->scl1
    pinsel.Pinnum = PINSEL_PIN_20;
    PINSEL_ConfigPin(&pinsel);
    
    I2C_Cmd(LPC_I2C1, ENABLE);	 
  }
  return;
}
