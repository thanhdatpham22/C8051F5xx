#include <compiler_defs.h>             // Compiler declarations
#include <C8051F580_defs.h>  
         
#define SYSCLK  24000000/8

SBIT (LEDYELLOW, SFR_P2, 0);
SBIT (LEDGREEN, SFR_P2, 1);
SBIT (LEDRED, SFR_P2, 2);

void OSCILLATOR_Init(void);
void PORT_Init(void);
void TIMER2_Init (U16 counts);

void TIMER1_Init (void);
static int count  = 0;
static int led_state = 0;

void main()

{
	SFRPAGE = ACTIVE_PAGE;
	PCA0MD &= ~0x40;

	OSCILLATOR_Init();
	PORT_Init();
	TIMER2_Init(SYSCLK /12 /10);

	EA =1;
	while(1);

}

void OSCILLATOR_Init(void)
{
 	U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x84;

   SFRPAGE = SFRPAGE_save;	
}

void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   P2MDOUT |= 0x07;                    
   P2SKIP  |= 0xFF;                    
   XBR2     = 0x40;                    

   SFRPAGE = SFRPAGE_save;
}

void TIMER2_Init (U16 counts)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE_PAGE;

   TMR2CN  = 0x00;                     // Stop Timer2; Clear TF2;
                                       // use SYSCLK/12 as timebase
   CKCON  &= ~0x60;                    // Timer2 clocked based on T2XCLK;

   TMR2RL  = -counts;                  // Init reload values
   TMR2    = 0xFFFF;                   // Set to reload immediately
   ET2     = 1;                        // Enable Timer2 interrupts
   TR2     = 1;                        // Start Timer2

   SFRPAGE = SFRPAGE_save;
}


INTERRUPT(Timer2_ISR, INTERRUPT_TIMER2)
{
   TF2H = 0;                           // Clear Timer2 interrupt flag
   count++;
   if(count >= 1000)
   {
   		LEDYELLOW =!LEDYELLOW;
		
   }          
	if(count >= 2000)
	{
		LEDGREEN = !LEDGREEN;
		count = 0;
	}          // Change state of LED
}
