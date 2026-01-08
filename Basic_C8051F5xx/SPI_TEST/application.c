#include <compiler_defs.h>
#include <C8051F580_defs.h>
#include "config.h"
#include "timer.h"
#include "uart.h"
#include "spi.h"
#include <stdio.h>
/*-------------------------------------------------*/
#define IGN_SW_ON  0x03


/*---------------------------------------------------*/
extern SEGMENT_VARIABLE(UART1_Buffer[UART1_BUFFERSIZE], U8, SEG_XDATA);

extern U8 UART1_Buffer_Size;
/*-------------------------------------------------*/

void mcu_config(void);
void application (void);


/*-------------------------------------------------*/
void mcu_config(void)
{
	SFRPAGE = ACTIVE_PAGE;

   	PCA0_Init();                   
   	PORT_Init();                        
   	SYSCLK_Init ();                     
   	UART1_Init();
   	Timer0_Init();
	TIMER2_Init();
	SPI0_Init();
	Can_Init();
   	EA = 1;

   	SFRPAGE = ACTIVE2_PAGE;
}



void application (void)
{
	U8 i;
	U16 address;                        // EEPROM address
   	U8 test_byte; 
    //----------------Funtion------------------------------
    mcu_config();
	LEDY = 1;  
	printf("Filling with 0xFF's...\n");
   	for (address = 0; address < EEPROM_CAPACITY; address++)
    {
	       test_byte = 0xFF;
	       EEPROM_Write (address, test_byte);

	       // Print status to UART0
	       if ((address % 16) == 0)
	       {
	          printf ("\nWriting 0x%04x: %02x ", address, (U16)test_byte);
	          LEDY = !LEDY;
	       }
	       else 
	       {
	          printf ("%02x ", (U16)test_byte); 
	       }
     }

	   // Verify EEPROM with 0xFF's
	   printf("\n\nVerifying 0xFF's...\n");
	   for (address = 0; address < EEPROM_CAPACITY; address++)
	   {
	      test_byte = EEPROM_Read (address);

	      // Print status to UART0
	      if ((address % 16) == 0)
	      {
	         printf ("\nVerifying 0x%04x: %02x ", address, (U16)test_byte);
	         LEDY = !LEDY;
	      }
	      else
	      {
	         printf ("%02x ", (U16)test_byte);
	      }

	      if (test_byte != 0xFF)
	      {
	         LEDY = 0;
	         printf ("Error at %u\n", address);
	         while (1);                    // Stop here on error (for debugging)
	      }
	   }
 
	printf ("\n\nVerification success!\n");

    while (1)                           // Loop forever
    {
      LEDY = !LEDY;    
	  //for(i = 0; i < UART1_Buffer_Size; i++)
	  //{
	  	//printf("%c",UART1_Buffer[i]);
	  //}      
	  printf("\r\n");            // Flash LED when done (all verified)
      delay_ms (1000);
    }
}

