#include <Device/I2C/IO_4Bit/IO_4Bit.h>
#include <Core/Debug/Error_check.h>

#define INPUT_MASK 0xF0

void IO_4Bit_init(IO_4Bit_t * data, uint8_t i2c_addr){
	I2C_8574_Init();

	if (i2c_addr < 10) {
		data->addr = ((0x70 >> 1) + i2c_addr)<<1 ;
	}else{
		data->addr = i2c_addr;
	}
	data->data = 0xff;
	data->error = SUCCESS;
	error_check_add(TYPE_STATUS,&data->error,ERROR_IO_4BIT_I2C);
	return;
}

Status IO_4Bit_Set_output(IO_4Bit_t * data, uint8_t ouput){
	data->data &= ~(ouput);
	data->error = I2C_8574_WriteOutput (data->addr, &data->data, INPUT_MASK);
	return data->error;
}

Status IO_4Bit_Clear_output(IO_4Bit_t * data, uint8_t ouput){
	data->data |= (ouput);
	data->error = I2C_8574_WriteOutput (data->addr, &data->data, INPUT_MASK);
	return data->error;
}

bool IO_4Bit_Get_input(IO_4Bit_t * data,uint8_t input){
	data->error = I2C_8574_ReadInput (data->addr, &data->data, INPUT_MASK);
	return ((~data->data)&(input<<4));
}
