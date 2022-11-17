#ifndef SAIDAS_H
#define SAIDAS_H

#include "driver/gpio.h"


#define T1E         GPIO_NUM_13
#define T1D         GPIO_NUM_15

#define T2E         GPIO_NUM_12
#define T2D         GPIO_NUM_2

#define T3E         GPIO_NUM_14
#define T3D         GPIO_NUM_4

#define T4E         GPIO_NUM_27
#define T4D         GPIO_NUM_16

void saidas_init( void );
void saida_set( gpio_num_t gpionum, uint32_t b );
int saida_get( gpio_num_t gpionum );

#endif
