
#include <compiler_defs.h>
#include <C8051F580_defs.h> 
#include "uart.h"

#define TIMOUT_BYTE 4
#define SYSCLK 24000000

volatile unsigned long timeout_counter = 0;
volatile unsigned long last_timeout_counter = 0;

extern U8 UART1_Buffer_Size;
extern bit UART1_Received_Full;
//----------------------------Timer0----------------------------
void Timer0_Init(void)
{
	
	TMOD &= 0xF0;
	TMOD |= 0x01; // timer 16bit
	CKCON |= 0x02;  // SYSCLK/48 = 500 kHz
	TH0 = (65536 - 500) >> 8;
	TL0 = (655536 -500) & 0xFF;
    ET0 = 1;               
    TR0 = 1;               
}

INTERRUPT(Timer0_ISR, INTERRUPT_TIMER0)
{	
	TH0 = (65536 - 500) >> 8;
    TL0 = (65536 - 500) & 0xFF;

    timeout_counter++;
	/*if ((timeout_counter - last_timeout_counter) >= TIMOUT_BYTE) // 
    {
        if (UART1_Buffer_Size > 0 && UART1_Buffer_Size < UART1_BUFFERSIZE)
        {
            UART1_Received_Full = 2;
        }
    }*/
}

void delay_ms(unsigned int ms)
{
    unsigned long start = timeout_counter;
    while ((timeout_counter - start) < ms);
}


//----------------------------Timer2----------------------------
void TIMER2_Init (void)
{
   // CKCON is available on all pages
   CKCON    |= 0x10;
}
void Delay_us (U8 time_us)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE_PAGE;

   TR2   = 0;                          // Stop timer
   TF2H  = 0;                          // Clear timer overflow flag
   TMR2  = -((U16)(SYSCLK / 1000000) * (U16)(time_us));
   TR2   = 1;                          // Start timer
   while (!TF2H);                      // Wait till timer overflow occurs
   TR2   = 0;                          // Stop timer

   SFRPAGE = SFRPAGE_save;
}

void Delay_t2_ms (U8 time_ms)
{
   U8 i;

   while(time_ms--) 
   {
      for(i = 0; i< 10; i++)           // 10 * 100 microsecond delay
      {         
         Delay_us (100); 
      }
   }
}
