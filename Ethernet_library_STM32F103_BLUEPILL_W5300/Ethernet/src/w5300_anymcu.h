// Edited By NK MAKER to use STM32 BLUEPILL
#ifndef	_W5300_ANYMCU_H_
#define	_W5300_ANYMCU_H_
#include <Arduino.h>
// D7...D0 -> PA7...PA0
#define DATA_DIR  GPIOA->CRL
#define DATA_OUT  GPIOA->ODR
#define DATA_IN   GPIOA->IDR
#define DATA_OUT_SET 0x33333333
#define DATA_IN_SET  0x88888888

//PA10 -> ADDR2, PA09 -> ADDR1, PA08 -> ADDR0 
#define ADDR_DIR  GPIOA->CRH
#define ADDR_OUT  GPIOA->ODR
#define ADDR_OUT_MASK 0xFFFFF000
#define ADDR_OUT_SET  0x00000333

//PC15 -> CS, PC14 -> WR, PC13 -> RD 
#define CTRL_DIR  GPIOC->CRH
#define CTRL_OUT  GPIOC->ODR
#define CTRL_OUT_MASK 0x000FFFFF
#define CTRL_OUT_SET  0x33300000

#define DATA_MASK ~(0x00FF<<0)
#define ADDR_MASK ~(0x0007<<8)
#define CTRL_MASK ~(0x0007<<13)

#define ADDR_0  0x0000<<8  //bit seq A2 A1 A0
#define ADDR_1  0x0001<<8
#define ADDR_2  0x0002<<8
#define ADDR_3  0x0003<<8
#define ADDR_4  0x0004<<8
#define ADDR_5  0x0005<<8
#define ADDR_6  0x0006<<8
#define ADDR_7  0x0007<<8

#define CTRL_IDLE 0x0007<<13  //b111  //bit seq CS WR RD
#define CTRL_RDST 0x0006<<13  //b110
#define CTRL_RDEX 0x0002<<13  //b010
#define CTRL_WRST 0x0005<<13  //b101
#define CTRL_WREX 0x0001<<13  //b001

uint16_t w5300_read_indirect(uint16_t addr);
void w5300_write_indirect(uint16_t addr, uint16_t data1);
void w5300_setup_indirect_MR(void);
void w5300_init(void);
void w5300_reset(void);

#endif   
