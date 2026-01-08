#ifndef APPLICATION_H
#define APPLICATION_H

	void mcu_config(void);
	void application (void);
	void get_key_update(void);
	void service_can_uds(void);
	void handle_uart_data(void);
	static void CAN_Tx_IGNSw_Process(void);

#endif APPLICATION_H