#include <compiler_defs.h>
#include <C8051F580_defs.h>

static unsigned long t3_us_count = 0;

void _CONFIG_TIMER3(void)
{
	U8 SFRPAGE_save = SFRPAGE;
    SFRPAGE = ACTIVE_PAGE;
	
	CKCON &= ~0x40;
    TMR3CN = 0x00;
    TMR3CN |= 0x04;

    TMR3RL = 256 - 6;
    TMR3 = TMR3RL;

    EIE1 = 0x40;
	SFRPAGE = SFRPAGE_save;
}
INTERRUPT (TIMER3_ISR, INTERRUPT_TIMER3)
{
    TMR3CN &= ~0x80;
    t3_us_count++;
}
void delay_t3_us(unsigned long us)
{
    unsigned long start = t3_us_count;
    while ((t3_us_count - start) < us);
}