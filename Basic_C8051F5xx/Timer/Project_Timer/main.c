#include <compiler_defs.h>
#include <C8051F580_defs.h>

#include "config.h"
#include "Timer_0.h"
#include "Timer_1.h"
#include "Timer_2.h"
#include "Timer_3.h"

SBIT(LEDY, SFR_P2, 0); 
SBIT(LEDG, SFR_P2, 1); 
SBIT(LEDD, SFR_P2, 2); 

void main()
{
	WATCH_DOG_Init();
	OSCILLATOR_Init();
	PORT_Init();
	//_CONFIG_TIMER0();
	//_CONFIG_TIMER1();
	//_CONFIG_TIMER2();
	_CONFIG_TIMER3();
	EA =1;
	while(1)
	{
		LEDG= !LEDG;
		delay_t3_us(1000);

	}
}