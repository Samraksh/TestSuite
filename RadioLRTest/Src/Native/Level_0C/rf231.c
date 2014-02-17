#include "rf231.h"

static uint8_t rf231_read_reg(uint8_t reg);
static uint8_t rf231_write_reg(uint8_t reg, uint8_t data);

void rf231_reset(void) {
	volatile int i;
	RF231_SEL(1); // set SEL high (deselect)
	RF231_SLP(0); // disable sleep
	for (i=0; i<1000; i++) { ; }
	RF231_RST(0); // put chip in reset
	for (i=0; i<1000; i++) { ; }
	RF231_RST(1); // Out of reset
	for (i=0; i<1000; i++) { ; }
}

void rf231_goto_pll_on(void) {
	rf231_write_reg(RF231_REG_TRX_STATE, GOTO_STATE_PLL_ON);
}

void rf231_goto_trx_off(void) {
	rf231_write_reg(RF231_REG_TRX_STATE, GOTO_STATE_TRX_OFF);
}

void rf231_goto_rx_on(void) {
	rf231_write_reg(RF231_REG_TRX_STATE, GOTO_STATE_RX_ON);
}

// Go to state TX_START
void rf231_goto_tx_start(void) {
	rf231_write_reg(RF231_REG_TRX_STATE, GOTO_STATE_TX_START);
}

void rf231_goto_sleep(void) {
	RF231_SLP(1);
}

// Enable external PA control
void rf231_enable_pa_rxtx(void) {
#ifndef USE_PA_BRD
	return;
#endif
	uint8_t data = rf231_read_reg(RF231_REG_TX_CTRL_1);
	data |= 0x80;
	rf231_write_reg(RF231_REG_TX_CTRL_1, data);
}

// Enable antenna diversity. Must enable both the hardware and automatic selection.
void rf231_enable_ant_div(void) {
#ifndef USE_PA_BRD
	return;
#endif
	uint8_t data = rf231_read_reg(RF231_REG_ANT_DIV);
	data |= 0x0C; // enable bits 3 and 2. ANT_EXT_SW_EN and ANT_DIV_EN
	rf231_write_reg(RF231_REG_ANT_DIV, data);
}

// Set TX power from the RF231. From +3 to -17 dBm
// Setting is 0 (highest power) to 0xF (lowest power)
// See RF231 datasheet section 9.2.5
void rf231_set_output_power(uint8_t p) {
	uint8_t reg, pp;
	if (p>0xF) pp = 0xF;
	else pp = p;

	reg = rf231_read_reg(RF231_REG_PHY_TX_PWR) & 0xF0;
	reg &= 0xF0;
	reg |=  pp & 0x0F;
	rf231_write_reg(RF231_REG_PHY_TX_PWR, reg);
}

// Return random byte from RSSI reg. See RF231 datasheet section 11.2.1
// random bits updated every 1us.
// Requires RX_ON
uint8_t rf231_get_random(void) {
	uint8_t data, ret=0;

	// two bits each time
	data = rf231_read_reg(RF231_REG_PHY_RSSI);
	ret |= (data >> 5) & 0x03;
	ret = ret << 2;

	data = rf231_read_reg(RF231_REG_PHY_RSSI);
	ret |= (data >> 5) & 0x03;
	ret = ret << 2;

	data = rf231_read_reg(RF231_REG_PHY_RSSI);
	ret |= (data >> 5) & 0x03;
	ret = ret << 2;

	data = rf231_read_reg(RF231_REG_PHY_RSSI);
	ret |= (data >> 5) & 0x03;

	return ret;
}

// Returns 5-bit RSSI value
uint8_t rf231_get_rssi(void) {
	return rf231_read_reg(RF231_REG_PHY_RSSI) & 0x1F;
}

uint8_t rf231_get_ed(void) {
	return rf231_read_reg(RF231_REG_PHY_ED_LEVEL);
}

void rf231_reset_clkm() {
	rf231_write_reg(REG_TRX_CTRL_0, 0x00);
}

void rf231_set_irq_mask(uint8_t m) {
	rf231_write_reg(REG_IRQ_MASK, m);
}

void rf231_set_channel(uint8_t c) {
	uint8_t reg;
	reg = rf231_read_reg(RF231_REG_PHY_CC_CCA);
	reg &= 0xE0;
	rf231_write_reg(RF231_REG_PHY_CC_CCA, reg | c);
}

uint8_t write_fb(uint8_t *fb, uint8_t sz) {
	uint8_t PHR = sz;
	uint8_t ret;
	int i;
	uint8_t myfb[127];
	int idx = 0;

	if (PHR > 127)
		PHR = 127;

	memcpy( myfb, fb, PHR);

	RF231_SEL(0);

	// Send Command
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, RF231_FB_WRITE);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	ret = SPI_I2S_ReceiveData(RF231_SPI);

	// Write PHR (length)
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, PHR);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	SPI_I2S_ReceiveData(RF231_SPI);

	while(idx < PHR) {
		//printf("sending byte %d [%d]\r\n",myfb[idx], idx);
		while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
		SPI_I2S_SendData(RF231_SPI, myfb[idx++]);
		while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
		SPI_I2S_ReceiveData(RF231_SPI);
	}
	for (i=0; i<1000; i++) { ; } // wait
	RF231_SEL(1);
	return ret;
}

uint8_t read_fb(uint8_t *fb) {
	uint8_t PHR;
	int idx = 0;

	RF231_SEL(0);

	// Send Command
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, RF231_FB_READ);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	SPI_I2S_ReceiveData(RF231_SPI);

	// Read PHR
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, 0x00);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	PHR = SPI_I2S_ReceiveData(RF231_SPI);

	printf("PHR 0x%X\r\n", PHR);

	while(idx < PHR) {
		while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
		SPI_I2S_SendData(RF231_SPI, 0x00);
		while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
		fb[idx++] = SPI_I2S_ReceiveData(RF231_SPI);
	}

	// Read LQI
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, 0x00);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	RF231_SEL(1);
	return SPI_I2S_ReceiveData(RF231_SPI);
}

uint8_t rf231_print_irq() {
	uint8_t reg = rf231_read_reg(REG_IRQ_STATUS);
	if (reg == 0xFF) { return 0; } // assume radio not present if stuck high
	printf("IRQ reg: 0x%X", reg);
	if (reg & 1<<0)
		printf(" PLL_LOCK ");
	if (reg & 1<<1)
		printf(" PLL_UNLOCK ");
	if (reg & 1<<2)
		printf(" RX_START ");
	if (reg & 1<<3)
		printf(" TRX_END ");
	if (reg & 1<<4)
		printf(" CCA_ED_DONE ");
	if (reg & 1<<5)
		printf(" AMI ");
	if (reg & 1<<6)
		printf(" TRX_UR ");
	if (reg & 1<<7)
		printf(" BAT_LOW ");
	printf("\r\n");
	return reg;
}

uint8_t  rf231_read_reg(uint8_t reg) {
	uint8_t ret;
	RF231_SEL(0);
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, 0x80 | reg);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	ret = SPI_I2S_ReceiveData(RF231_SPI);
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, 0x00);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	ret = SPI_I2S_ReceiveData(RF231_SPI);
	RF231_SEL(1);
	return ret;
}

uint8_t  rf231_write_reg(uint8_t reg, uint8_t data) {
	uint8_t ret;
	volatile int i;
	RF231_SEL(0);
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, 0xC0 | reg);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	ret = SPI_I2S_ReceiveData(RF231_SPI);
	while (SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_TXE) == RESET) { ; }
	SPI_I2S_SendData(RF231_SPI, data);
	while ( SPI_I2S_GetFlagStatus(RF231_SPI, SPI_I2S_FLAG_RXNE) == RESET) {;}
	ret = SPI_I2S_ReceiveData(RF231_SPI);
	RF231_SEL(1);
	for (i=0; i<10000; i++) { ; } // wait
	return ret;
}



uint8_t rf231_print_pn() {
	uint8_t reg = rf231_read_reg(0x1C);
	printf("PN \t= %d (should be 3)\r\n", reg);
	return reg;
}

uint8_t rf231_print_vn() {
	uint8_t reg = rf231_read_reg(0x1D);
	printf("VN \t= %d (should be 2)\r\n", reg);
	return reg;
}

uint16_t rf231_print_mn() {
	uint8_t reg1 = rf231_read_reg(0x1E);
	uint8_t reg2 = rf231_read_reg(0x1F);
	uint16_t reg3 = (reg2<<8) | reg1;
	printf("MN \t= %d\r\n", reg3);
	return reg3;
}

uint8_t rf231_print_state() {
	uint8_t reg = rf231_read_reg(RF231_REG_TRX_STATUS);
	printf("RF231 State: 0x%X ",reg & 0x1F);
	switch(reg & 0x1F) {
		case 0x00: printf("P_ON"); break;
		case 0x01: printf("BUSY_RX"); break;
		case 0x02: printf("BUSY_TX"); break;
		case 0x06: printf("RX_ON"); break;
		case 0x08: printf("TRX_OFF"); break;
		case 0x09: printf("PLL_ON"); break;
		case 0x0F: printf("SLEEP"); break;
		case 0x11: printf("BUSY_RX_AACK"); break;
		case 0x12: printf("BUSY_TX_ARET"); break;
		case 0x16: printf("RX_AACK_ON"); break;
		case 0x19: printf("TX_ARET_ON"); break;
		case 0x1C: printf("RX_ON_NOCLK"); break;
		case 0x1D: printf("RX_AACK_ON_NOCLK"); break;
		case 0x1E: printf("BUSY_RX_AACK_NOCLK"); break;
		case 0x1F: printf("STATE TRANSISTION IN PROGRESS"); break;
		default: printf("RESERVED, FAIL"); break;
	}
	printf("\r\n");
	return reg;
}

void rf231_print_vreg() {
	uint8_t data;
	data = rf231_read_reg(RF231_REG_VREG);
	printf("VREG Register Contents 0x%X\r\n", data);
	if (data & 0x80)
		printf("\tAVREG external\r\n");
	else
		printf("\tAVREG internal\r\n");

	if (data & 0x40)
		printf("\tAVDD_OK\r\n");
	else
		printf("\tAVDD NOT OK\r\n");

	if (data & 0x08)
		printf("\tDVREG external\r\n");
	else
		printf("\tDVREG internal\r\n");

	if (data & 0x04)
		printf("\tDVDD_OK\r\n");
	else
		printf("\tDVDD NOT OK\r\n");
}
