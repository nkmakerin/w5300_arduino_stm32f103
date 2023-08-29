// Edited By NK MAKER to use STM32 BLUEPILL
#include "w5300_anymcu.h"
#include <Arduino.h>

// Initlize W5300 in use 8 bit Indirect addressing mode
void w5300_init(void)
{  
	// init all w5300 connection pins as output and set all pins high
	DATA_DIR = DATA_OUT_SET;
	ADDR_DIR = (ADDR_DIR & ADDR_OUT_MASK) | ADDR_OUT_SET ;
	CTRL_DIR = (CTRL_DIR & CTRL_OUT_MASK) | CTRL_OUT_SET ;
	DATA_OUT |= ~DATA_MASK; 
	ADDR_OUT |= ~ADDR_MASK;     
	CTRL_OUT |= ~CTRL_MASK; 
	delay(1000);
	Serial.println("\r\n--> Modified W5300 Arduino ethernet library for any mcu with Indirect Addressing  (need 14 GPIO Only) \r\n--> Developed by - M.Vaghasia (rcele_85@yahoo.com) \r\n--> edited by NK MAKER to use STM32 BLUEPILL \r\n--> Current MCU Selected - STM32 BLUEPILL");
	Serial.println("--> Soft Reset W5300");
	w5300_reset();
	delay(1000);
	// configure W5300 to work in indirect addressing mode
	w5300_setup_indirect_MR();
	//Serial.println(w5300_read_indirect(0),HEX);
	delay(1000);
	if(w5300_read_indirect(0) == 0x3801)	  Serial.println("--> w5300 indirect addressing Init - Success ;)");
	else 
	  {
		while(1)
		 {
			Serial.println("--> w5300 indirect addressing Init - Fail :(\r\n--> Check w5300 hardware connections and reset mcu");
			delay(5000);
		  }
	  }
	delay(1000);
}

// Setup W5300 to use 8 bit Indirect addressing mode
void w5300_setup_indirect_MR(void)
{
	DATA_DIR = DATA_OUT_SET; // Set Databus as output
	// Write 0x01 to MR_LOW (Addr - 0x01) for configure w5300 indirect addressing
	DATA_OUT |= ~DATA_MASK; DATA_OUT &= DATA_MASK | 0x0001;
	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_1;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WRST;  
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WREX; 
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE;  
	delayMicroseconds(1);
}

// Soft Reset W5300
void w5300_reset(void)
{
	DATA_DIR = DATA_OUT_SET; // Set Databus as output
	// Write 0x80 to MR_LOW (Addr - 0x01) for soft reset w5300
	DATA_OUT |= ~DATA_MASK; DATA_OUT &= DATA_MASK | 0x0080;
	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_1;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WRST;  
	  
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WREX; 
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE;  
	delayMicroseconds(1);
}

// Write 16 Bit register of W5300 using 8 bit indirect addressing
void w5300_write_indirect(uint16_t addr, uint16_t data1)
{
	uint16_t data = data1;
	DATA_DIR = DATA_OUT_SET; // Set Databus as output
	// set desired addr in indirect address reg 0x02, 0x03 address
	DATA_OUT |= ~DATA_MASK; DATA_OUT &= DATA_MASK | ((addr>>8) & 0xFF);
	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_2;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WRST;  
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WREX; 
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE;  
	delayMicroseconds(1);

	DATA_OUT |= ~DATA_MASK; DATA_OUT &= DATA_MASK | (addr & 0xFF);
	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_3;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WRST;  
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WREX; 
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE;  
	delayMicroseconds(1);
	  
	// set desired data in indirect data reg 0x04, 0x05 address
	DATA_OUT |= ~DATA_MASK; DATA_OUT &= DATA_MASK | ((data1>>8) & 0xFF);
	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_4;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WRST;  
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WREX; 
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE;  
	delayMicroseconds(1);

	DATA_OUT |= ~DATA_MASK; DATA_OUT &= DATA_MASK | (data1 & 0xFF);
	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_5;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WRST;  
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WREX; 
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE;  
	delayMicroseconds(1);
}

// Read 16 Bit register of W5300 using 8 bit indirect addressing
uint16_t w5300_read_indirect(uint16_t addr)
{
	uint16_t data =0;
	uint32_t data1 = 0;
	uint32_t data2 = 0;
	DATA_DIR = DATA_OUT_SET; // Set Databus as output
	  
	// set desired addr in indirect address reg 0x02, 0x03 address
	DATA_OUT |= ~DATA_MASK; DATA_OUT &= DATA_MASK | ((addr>>8) & 0xFF);
	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_2;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WRST;  
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WREX; 
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE;  
	delayMicroseconds(1);

	DATA_OUT |= ~DATA_MASK; DATA_OUT &= DATA_MASK | (addr & 0xFF);
	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_3;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WRST;  
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_WREX; 
	delayMicroseconds(1);
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE;  
	delayMicroseconds(1);
	   
	DATA_DIR = DATA_IN_SET; // Set Data Bus input
	  
	// read data from indirect data register
	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_4;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_RDEX; 
	delayMicroseconds(2);
	data1 = DATA_IN & 0xFF;
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE; 
	delayMicroseconds(1);

	ADDR_OUT |= ~ADDR_MASK; ADDR_OUT &= ADDR_MASK | ADDR_5;     
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_RDEX;  
	delayMicroseconds(2);
	data2 = DATA_IN & 0xFF;
	data = (data1<<8)|data2;
	CTRL_OUT |= ~CTRL_MASK; CTRL_OUT &= CTRL_MASK | CTRL_IDLE; 
	delayMicroseconds(1);
	return data;
}



