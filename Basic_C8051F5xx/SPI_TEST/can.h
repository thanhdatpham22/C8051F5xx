#ifndef CAN0_H
#define CAN0_H

/*----------------------------------------------------------*/

#define MESSAGE_SIZE 			8

//--Message Objects to use (range 0-31)------------------------------------
#define MO_TX_BL_CMD    0x10
#define MO_TX_BL_WRITE8 0x11
#define MO_RX_BL_RSP    0x12
#define MO_FUNCTION_REQ 0x13
#define MO_PGS_PHYS_ID  0x14
#define MO_RECEIVE_ID   0x15
#define MO_TX_CGW1_ID  	0x1A // IGN SW


// ----11-bit Arbitration Message IDs----------------------------------------------
#define MSG_ID_TX_BL_CMD      0x0B1 // Transmitted by Master MCU for all BL commands, except:
#define MSG_ID_TX_BL_WRITE8   0x1B1 // Transmitted by Master MCU for the 8-byte Write command
#define MSG_ID_RX_BL_RSP      0x2B1 // Received by Master MCU (responses from target MCU)
#define MSG_ID_FUNCTION_REQ   0x7DF // Transmitted by Master MCU for the 8-byte Write command
#define MSG_ID_PGS_PHYS_ID    0x796 // Received by Master MCU (responses from target MCU)
#define MSG_ID_RECEIVE_ID	  0x79E // Received by Master MCU (responses from target MCU)
#define MSG_ID_TX_IGNSW       0x541 // Transmitted by Master MCU for the 8-byte Write command


//------------diagnostic-------------
#define DEFAULT_MODE 		 	0x00
#define STANDARD_MODE 		 	0x01
#define SWVERSION	  		 	0x02
#define EXTENDED_MODE 		 	0x03
#define EXTENDED_SESSION 	 	0x04
#define CONTROLDTC_OFF 		 	0x05
#define COMM_CONTROL_DISABLE 	0x06
#define PROGRAMMING_SESSION  	0x07
#define REQUEST_SEED		 	0x08
#define SEND_KEY			 	0x09
#define ROUTINE_BINARY_UPDATE	0x0A
#define REQUEST_DOWNLOAD_1		0x0B
#define REQUEST_DOWNLOAD_2		0x0C
#define REQUEST_TRANSFER_EXIT	0x0D
#define COMM_CONTROL_ENABLE		0x0E
#define ECU_RESET				0x0F
#define READ_PARTNUMBER	        0x10
#define CONTROL_FLOW            0x11
#define FIRST_FARME             0x12
#define CONSECUTIVE             0x13
#define COMM_CONTROL_DISABLE2	0x16

//------------------------
// Bit Definition Masks
//------------------------

// CAN0STAT
#define BOff  0x80                     // Busoff Status
#define EWarn 0x40                     // Warning Status
#define EPass 0x20                     // Error Passive
#define RxOk  0x10                     // Receive Message Successfully
#define TxOk  0x08                     // Transmitted Message Successfully
#define LEC   0x07                     // Last Error Code

// CAN0INTERRUPT
#define  STATUS_CHANGE  0x8000U
#define  END            0

// CAN0MESSAGECONTROL MASK
#define  NEWDATA        0x8000U
#define  EOB            0x0080U
/*-------------------VARIABLE-------------------------------*/

/*----------------------------------------------------------*/
	void Can_Init(void);
	void CAN0_Send_Message (U8 *buf, U8 msg_obj);
	void SEND_541(U8 IGN_Sw);
	U8 SEND_7DF(U8 mode);
	void SEND_796(U8 mode);
	void Mode_Complete(void);
	U8 TGT_Write_Flash(U8 *buf, U16 index, U8 numbytes, U8 Page_Num);



#endif CAN0_H