
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "compiler_defs.h"
#include "C8051F580_defs.h"            // SFR declarations

//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void CAN0_Init (void);
void Timer0_Init(void);
void delay_ms(unsigned int ms);
void CAN0_Transmit_Polling(void);
INTERRUPT_PROTO (CAN0_ISR, INTERRUPT_CAN0);
void Receive_Message(void);
//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24000000          // System clock speed in Hz
#define MESSAGE_SIZE        8          // Size in bytes of each CAN message
                                       // Range is 1-8
//-----------------------------------------------------------------------------
// Bit Definition Masks
//-----------------------------------------------------------------------------

// CAN0STAT
#define BOff  0x80                     // Busoff Status
#define EWarn 0x40                     // Warning Status
#define EPass 0x20                     // Error Passive
#define RxOk  0x10                     // Receive Message Successfully
#define TxOk  0x08                     // Transmitted Message Successfully
#define LEC   0x07                     // Last Error Code

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT (LEDY, SFR_P2, 0);                 // LED = 1 turns on the LED
SBIT (LEDG, SFR_P2, 1);                 // LED = 1 turns on the LED
SBIT (LEDR, SFR_P2, 2);
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

bit CAN_ERROR = 0;                     // 0 = No Errors occurred
U8 CAN_RX_COMPLETE = 0;  
U8 buffer[8];  
volatile unsigned long timeout_counter = 0;    
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{ 
    U16 iter;                           // Loop counter

    SFRPAGE = ACTIVE_PAGE;              // Set for PCA0MD

    PCA0MD &= ~0x40;                    // Disable Watchdog Timer

    OSCILLATOR_Init ();                 // Initialize oscillator
    PORT_Init ();                       // Initialize crossbar and GPIO
    CAN0_Init ();                       // Start CAN peripheral
	Timer0_Init();
	LEDG = 0;
	LEDY = 0;
	LEDR = 1;
    EIE2 |= 0x02;                       // Enable CAN interupts
    EA = 1;                             // Enable global interrupts

   while (1)
   {
		if (CAN_RX_COMPLETE == 0)
		{
		    LEDG = !LEDG;
		  	delay_ms(1000);
		}
		else
		{
			LEDY = 0;
			while(1)
			{
				CAN0_Transmit_Polling();
				delay_ms(1000);
			} 
		}
   }

}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initialize the internal oscillator to 24 MHz
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x87;                      // Set internal oscillator divider to 1

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.6   digital  push-pull        CAN TX
// P0.7   digital  open-drain       CAN RX
//
// P2.0,1,2   digital  push-pull        LED
//
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE  = CONFIG_PAGE;             // Port SFR's on Configuration page

   P0MDOUT  = 0x40;
   P0SKIP   = 0x3F;
   P1SKIP   = 0xFF;                   // P0.6 (CAN0 TX) is push-pull
   P2MDOUT  = 0x07;                   // P2.0 (LED) and LED2.1 is push-pull
   XBR0     = 0x02;                    // Enable CAN0 on Crossbar
   XBR2     = 0x40;                    // Enable Crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// CAN0_Init
//-----------------------------------------------------------------------------


void CAN0_Init (void)
{
    U8 SFRPAGE_save = SFRPAGE;
    SFRPAGE  = CAN0_PAGE;               // All CAN register are on page 0x0C

    CAN0CN |= 0x01;                     // Start Intialization mode

    //---------Initialize general CAN peripheral settings

    CAN0CN |= 0x4E;                   
    //CAN0BT = 0x1402; // 1Mbit/s (BRP = 3; PROP = 4; TSG1 = 1; TSEG2 = 2; SJW = 1)
	CAN0BT = 0x58C2; //500kBIT                 
    //---------Initialize settings common to all message objects

    // Command Mask Register
    CAN0IF1CM = 0x00F0;                
	CAN0IF1MC = 0x0080 | MESSAGE_SIZE;

    CAN0IF1M1 = 0x0000;
    CAN0IF1M2 = 0x5FFC;
                    
    CAN0IF1A1 = 0x0000;                 
//-----------------Message Transmit1----------------------------------
	CAN0IF1A2 = 0xA000 | (0x796 << 2);   
	CAN0IF1CR = 0x05;           
	while (CAN0IF1CRH & 0x80) {}
//-----------------Message Transmit2-----------------------------------
	CAN0IF1A2 = 0xA000 | (0x541 << 2);   
	CAN0IF1CR = 0x06;           
	while (CAN0IF1CRH & 0x80) {}

//-----------------Message Recieve-------------------------------------
    CAN0IF1MC = 0x1480 | MESSAGE_SIZE;  
    CAN0IF1A2 = 0x8000 | (0x76E << 2);  
    CAN0IF1CR = 0x10;                  
    while (CAN0IF1CRH & 0x80) {}       

//-----------------Initialize settings for unused message objects
    CAN0CN &= ~0x41;                   
    EIE2 |= 0x02;                       
    SFRPAGE = SFRPAGE_save;
}


void CAN0_Transmit_Polling(void)
{

    U8 SFRPAGE_save = SFRPAGE;
    SFRPAGE = CAN0_PAGE;      

    CAN0IF1DA1L = buffer[0];  
    CAN0IF1DA1H = buffer[1];  
    CAN0IF1DA2L = buffer[2];  
    CAN0IF1DA2H = buffer[3];  
    CAN0IF1DB1L = buffer[4];  
    CAN0IF1DB1H = buffer[5];  
    CAN0IF1DB2L = buffer[6];  
    CAN0IF1DB2H = buffer[7];  

    CAN0IF1CM = 0x0087;                                                     
    CAN0IF1CR = 0x05;
	//CAN0IF1CR = 0x06; 
	         
    while (CAN0IF1CRH & 0x80) {} 
    while (!(CAN0STAT & TxOk))   
    {
        if (CAN0STAT & (BOff | LEC))  
        {
            CAN_ERROR = 1;        
            break;                
        }
    }
    if (!CAN_ERROR)
    {
        LEDY = 0;                  
    }
    else
    {
        LEDY = 1;                  // Turn off LED if error occurred
    }
    SFRPAGE = SFRPAGE_save;
}
//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
INTERRUPT (CAN0_ISR, INTERRUPT_CAN0)
{

    U8 status, Interrupt_ID;
	SFRPAGE = CAN0_PAGE;               

	status = CAN0STAT;
	Interrupt_ID = CAN0IID;
	   	      
    CAN0IF1CM = 0x007F;           
    CAN0IF1CR = Interrupt_ID;             	  	
    while (CAN0IF1CRH & 0x80) {}        

    if (status & RxOk)                  
    {
       
       buffer[0] = CAN0IF1DA1L;    
	   buffer[1] = CAN0IF1DA1H;    
       buffer[2] = CAN0IF1DA2L;    
	   buffer[3] = CAN0IF1DA2H;    
       buffer[4] = CAN0IF1DB1L;    
	   buffer[5] = CAN0IF1DB1H;    
       buffer[6] = CAN0IF1DB2L;    
	   buffer[7] = CAN0IF1DB2H;    

	   CAN_RX_COMPLETE = 1;

    }

    if (status & LEC)                 
    {

       if ((status & LEC) != 0x07) 
       {
          CAN_ERROR = 1;
		  LEDY = 1; 
       }
    }

    if (status & BOff)
    {
      	CAN_ERROR = 1;
	  	LEDY = 1;

    }

    if (status & EWarn)
    {
      	CAN_ERROR = 1;
	  	LEDY = 1;
    }
	SFRPAGE  = ACTIVE_PAGE;
}	


//--------------------Timer 0 delay------------------------------
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

void delay_ms(unsigned int ms)
{
    unsigned long start = timeout_counter;
    while ((timeout_counter - start) < ms);
}
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

