
#include <compiler_defs.h>
#include <C8051F580_defs.h> 
#define SYSCLK             24000000

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
void Delay_ms (U8 time_ms)
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