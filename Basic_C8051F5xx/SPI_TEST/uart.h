#ifndef UART0_H
#define UART0_H

	#include <compiler_defs.h>


	#define UART1_BUFFERSIZE 253


	void UART1_Init (void);
	U16 CRC16_CCITT_FALSE(U8 *datac, U16 length);
	void SEND_String(U8 *str, U8 lenght);



#endif UART0_H