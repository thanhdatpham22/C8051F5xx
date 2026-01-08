#ifndef SPI0_H
#define SPI0_H
	
	#define  F_SCK_MAX         2000000     // Max SCK freq (Hz)
	#define  T_NSS_DISABLE_MIN 500         // Min NSS disable time (ns)
	#define  EEPROM_CAPACITY   4096        // EEPROM capacity (bytes)

	// EEPROM Instruction Set
	#define  EEPROM_CMD_READ   0x03        // Read Command
	#define  EEPROM_CMD_WRITE  0x02        // Write Command
	#define  EEPROM_CMD_WRDI   0x04        // Reset Write Enable Latch Command
	#define  EEPROM_CMD_WREN   0x06        // Set Write Enable Latch Command
	#define  EEPROM_CMD_RDSR   0x05        // Read Status Register Command
	#define  EEPROM_CMD_WRSR   0x01        // Write Status Register Command

	void SPI0_Init(void);
	void EEPROM_Write (U16 address, U8 value);
	U8 EEPROM_Read (U16 address);
	
#endif SPI0_H