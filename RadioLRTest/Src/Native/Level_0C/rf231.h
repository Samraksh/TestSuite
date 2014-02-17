#include <stddef.h>
#include "stm32f10x.h"
#include <string.h>
#include <stdint.h>
#include <..\Targets\Native\STM32F10x\DeviceCode\drivers\gpio\stm32f10x_gpio.h>
#include <..\Targets\Native\STM32F10x\DeviceCode\drivers\spi\stm32f10x_spi.h>

#define USE_LCD
//#define USE_UART
#define USE_PA_BRD
//#define USE_CLOCKS

#ifdef USE_UART
#include <stdio.h>
#endif

#ifndef USE_UART // disable printf
#define printf(fmt, ...) ((void)0)
#endif

#define RF231_REG_VREG 			0x10
#define RF231_REG_TRX_STATUS 	0x01
#define RF231_REG_TRX_STATE 	0x02
#define REG_IRQ_STATUS 			0x0F
#define REG_IRQ_MASK			0x0E
#define REG_TRX_CTRL_0			0x03
#define RF231_REG_TX_CTRL_1		0x04
#define RF231_REG_PHY_TX_PWR	0x05
#define RF231_REG_PHY_RSSI		0x06
#define RF231_REG_PHY_ED_LEVEL	0x07
#define RF231_REG_PHY_CC_CCA	0x08
#define RF231_REG_ANT_DIV		0x0D

#define STATE_P_ON 					0x00
#define STATE_BUSY_RX 				0x01
#define STATE_BUSY_TX 				0x02
#define STATE_RX_ON					0x06
#define STATE_TRX_OFF				0x08
#define STATE_PLL_ON				0x09
#define STATE_SLEEP					0x0F
#define STATE_BUSY_RX_AACK			0x11
#define STATE_BUSY_TX_ARET			0x12
#define STATE_RX_AACK_ON			0x16
#define STATE_TX_ARET_ON			0x19
#define STATE_RX_ON_NOCLK			0x1C
#define STATE_RX_AACK_ON_NOCLK		0x1D
#define STATE_BUSY_RX_AACK_NOCLK	0x1E

#define GOTO_STATE_NOP				0x00
#define GOTO_STATE_TX_START			0x02
#define GOTO_STATE_FORCE_TRX_OFF	0x03
#define GOTO_STATE_FORCE_PLL_ON		0x04
#define GOTO_STATE_RX_ON			0x06
#define GOTO_STATE_TRX_OFF			0x08
#define GOTO_STATE_PLL_ON			0x09
#define GOTO_STATE_RX_AACK_ON		0x16
#define GOTO_STATE_TX_ARET_ON		0x19

#define RF231_FB_WRITE				0x60
#define RF231_FB_READ				0x20

#define IRQ_BAT_LOW 	1<<7
#define IRQ_TRX_UR	 	1<<6
#define IRQ_AMI			1<<5
#define IRQ_CCA_ED_DONE 1<<4
#define IRQ_TRX_END		1<<3
#define IRQ_RX_START	1<<2
#define IRQ_PLL_UNLOCK	1<<1
#define IRQ_PLL_LOCK	1<<0

#ifdef USE_PA_BRD
#define RF231_SPI SPI2
#define RF231_SEL(x) { if(x) GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_SET); else GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_RESET); }
#define RF231_RST(x) { if(x) GPIO_WriteBit(GPIOF, GPIO_Pin_10, Bit_SET); else GPIO_WriteBit(GPIOF, GPIO_Pin_10, Bit_RESET); }
#define RF231_SLP(x) { if(x) GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET); else GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_RESET); }
#define RF231_AMP(x) { if(x) GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET); else GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET); }
#else
#define RF231_SPI SPI1
#define RF231_SEL(x) { if(x) GPIO_WriteBit(GPIOG, GPIO_Pin_6, Bit_SET); else GPIO_WriteBit(GPIOG, GPIO_Pin_6, Bit_RESET); }
#define RF231_RST(x) { if(x) GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_SET); else GPIO_WriteBit(GPIOE, GPIO_Pin_2, Bit_RESET); }
#define RF231_SLP(x) { if(x) GPIO_WriteBit(GPIOG, GPIO_Pin_8, Bit_SET); else GPIO_WriteBit(GPIOG, GPIO_Pin_8, Bit_RESET); }
#define RF231_AMP(x) ((void)0)
#endif


void rf231_print_vreg();
uint8_t rf231_print_state();
uint8_t rf231_print_pn();
uint8_t rf231_print_vn();
uint16_t rf231_print_mn();
uint8_t rf231_print_irq();

uint8_t read_fb(uint8_t *fb);
uint8_t write_fb(uint8_t *fb, uint8_t sz);
uint8_t rf231_get_random(void);
uint8_t rf231_get_rssi(void);
uint8_t rf231_get_ed(void);

void rf231_enable_ant_div(void);
void rf231_enable_pa_rxtx(void);

void rf231_reset_clkm(void);
void rf231_set_irq_mask(uint8_t);

void rf231_set_channel(uint8_t c);
void rf231_set_output_power(uint8_t p);

void rf231_goto_tx_start(void);
void rf231_goto_pll_on(void);
void rf231_goto_rx_on(void);
void rf231_goto_trx_off(void);
void rf231_goto_sleep(void);

void rf231_reset(void);


