#ifndef TIMER0_H
#define TIMER0_H

#define MODE0 0x01 // 8_Bit counter and reload
#define MODE1 0x02 // 13_Bit counter and reload
#define MODE2 0x03 // 16_Bit counter and reload
#define MODE3 0x04 // Two 8_Bit counter and reload

#define SYSCLK       24000000/8 
#define BLINK_RATE   1   
#define TIMER0_RL_HIGH  (0xFFFF -((SYSCLK/48/BLINK_RATE >> 8) & 0x00FF))
#define TIMER0_RL_LOW   (0xFFFF -(SYSCLK/48/BLINK_RATE & 0x00FF))

#define TIMER_PRESCALER            48  // Based on Timer2 CKCON and TMR2CN settings
#define TIMER_RELOAD  (U16)(0xFF - (SYSCLK/TIMER_PRESCALER/1000)) // Reload value for TL0 or TH0



void _CONFIG_TIMER0(void);
void delay_t0_us(unsigned long us);

#endif TIMER0_H
