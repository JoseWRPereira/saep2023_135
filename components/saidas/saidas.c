#include <stdio.h>
#include "saidas.h"

void saidas_init( void )
{
    gpio_reset_pin(T1E);    gpio_reset_pin(T1D);
    gpio_reset_pin(T2E);    gpio_reset_pin(T2D);
    gpio_reset_pin(T3E);    gpio_reset_pin(T3D);
    gpio_reset_pin(T4E);    gpio_reset_pin(T4D);

    gpio_set_direction(T1E,GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(T1D,GPIO_MODE_INPUT_OUTPUT);

    gpio_set_direction(T2E,GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(T2D,GPIO_MODE_INPUT_OUTPUT);

    gpio_set_direction(T3E,GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(T3D,GPIO_MODE_INPUT_OUTPUT);

    gpio_set_direction(T4E,GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(T4D,GPIO_MODE_INPUT_OUTPUT);

    gpio_set_level(T1E,0);  gpio_set_level(T1D,0);
    gpio_set_level(T2E,0);  gpio_set_level(T2D,0);
    gpio_set_level(T3E,0);  gpio_set_level(T3D,0);
    gpio_set_level(T4E,0);  gpio_set_level(T4D,0);
}

void saida_set( gpio_num_t gpionum, uint32_t b )
{
    if( gpionum == T1E || gpionum == T1D )
    {
        gpio_set_level(T1E, b);
        gpio_set_level(T1D, b);
    }
    else if( gpionum == T2E || gpionum == T2D )
    {
        gpio_set_level(T2E, b);
        gpio_set_level(T2D, b);
    }
    else if( gpionum == T3E || gpionum == T3D )
    {
        gpio_set_level(T3E, b);
        gpio_set_level(T3D, b);
    }
    else if( gpionum == T4E || gpionum == T4D )
    {
        gpio_set_level(T4E, b);
        gpio_set_level(T4D, b);
    }
}

int saida_get( gpio_num_t gpionum )
{
    return( gpio_get_level( gpionum ) );
}

