#include <compiler_defs.h>
#include <C8051F580_defs.h> 


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

void PCA0_Init (void)
{
    U8 SFRPAGE_save = SFRPAGE;
    SFRPAGE = ACTIVE_PAGE;

    PCA0MD   &= ~0x40;

    SFRPAGE = SFRPAGE_save;
}

void SYSCLK_Init(void)
{
	U8 SFRPAGE_save = SFRPAGE;
    SFRPAGE = CONFIG_PAGE;

    OSCICN   |= 0x87;
	RSTSRC  = 0x04;
    SFRPAGE = SFRPAGE_save;
}