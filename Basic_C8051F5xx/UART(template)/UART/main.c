
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F580_defs.h>            // SFR declarations
#include <stdio.h>

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define SYSCLK      24000000           // SYSCLK frequency in Hz
#define BAUDRATE      115200 		// Baud rate of UART in bps
#define UART1_BUFFERSIZE 64

SBIT (LEDY, SFR_P2, 0);                 
SBIT (LEDG, SFR_P2, 1);
SBIT (SW2, SFR_P1, 1);

volatile unsigned long timeout_counter = 0;

U8 UART1_Buffer[UART1_BUFFERSIZE];

U8 UART1_Buffer_Size = 0;
bit UART1_Received_Full = 0; 
static U8 Byte;
//--------------spi_d

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SYSCLK_Init (void);
void UART1_Init (void);
void PORT_Init (void);
void delay_ms(unsigned int ms);
void Timer0_Init(void);


//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
	U8 i;

   SFRPAGE = ACTIVE_PAGE;

   PCA0MD &= ~0x40;                    // Disable watchdog timer
   PORT_Init();                        // Initialize Port I/O
   SYSCLK_Init ();                     
   UART1_Init();
   Timer0_Init();
   EA =1;
   SFRPAGE = ACTIVE2_PAGE;             // Switch page for UART1 communication

   while (1)
   {	
        for(i = 0 ; i < UART1_BUFFERSIZE; i++)
		{
			printf("%c", UART1_Buffer[i]);
		}
		printf("\r\n");
		delay_ms(1000);
   }
   
}


//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

    P1MDOUT |= 0x08;                    // Enable UART1 TX as push-pull output
    P0SKIP  = 0xFF;                     // Skip to P0, and P1.1,2,3
	P1SKIP = 0xE7;						// Enable UART1 on P1.3(TX) and P1.4(RX)

	P2MDOUT |= 0x03;                    // Enable LED as a push-pull output
    P2SKIP  |= 0xFC;
	//XBR0      = 0x04;
    XBR2    = 0x42;                     
                                       // Enable crossbar and weak pull-ups
    SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------

void SYSCLK_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN |= 0x87;                     // Configure internal oscillator for
                                       // its maximum frequency
   RSTSRC  = 0x04;                     // Enable missing clock detector

   SFRPAGE = SFRPAGE_save;
}
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
}
//-----------------------------------------------------------------------------
// UART1_Init
//-----------------------------------------------------------------------------

void UART1_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = ACTIVE2_PAGE;

   SCON1 = 0x10;                       // SCON1: 8-bit variable bit rate

   if (SYSCLK / BAUDRATE / 2 / 256 < 1) 
   {
      TH1 = -(SYSCLK / BAUDRATE / 2);
      CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
      CKCON |=  0x08;
   } 
   else if (SYSCLK / BAUDRATE / 2 / 256 < 4) 
   {
      TH1 = -(SYSCLK / BAUDRATE / 2 / 4);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
      CKCON |=  0x01;
   } 
   else if (SYSCLK / BAUDRATE / 2 / 256 < 12) 
   {
      TH1 = -(SYSCLK / BAUDRATE / 2 / 12);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
   } 
   else 
   {
      TH1 = -(SYSCLK / BAUDRATE / 2 / 48);
      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
      CKCON |=  0x02;
   }

    TL1 = TH1;                          // Init Timer1
    TMOD &= ~0xF0;                      // TMOD: timer 1 in 8-bit autoreload
    TMOD |=  0x20;
    TR1 = 1;                            // START Timer1

    TI1 = 1;                            // Indicate TX0 ready (SCON1)
	EIE2 |= 0x08;
    SFRPAGE = SFRPAGE_save;
}

INTERRUPT(UART1_ISR, INTERRUPT_UART1)
{ 
	U8 SFRPAGE_save = SFRPAGE;
    SFRPAGE = ACTIVE2_PAGE;
	if (RI1 == 1)
    {
	    Byte = SBUF1;                    
	    if (UART1_Buffer_Size < UART1_BUFFERSIZE)
	    {
	       UART1_Buffer[UART1_Buffer_Size] = Byte; // Store in array
	       UART1_Buffer_Size++;	
		   //last_timeout_counter = timeout_counter; 	// Update array's size      		 
	    }

		else
		{
			UART1_Buffer_Size =0;
		}

		RI1 = 0;  
		     
    }
	SFRPAGE = SFRPAGE_save;
}



void delay_ms(unsigned int ms)
{
    unsigned long start = timeout_counter;
    while ((timeout_counter - start) < ms);
}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------