#include "LoRa_test.h"

// HINT: YOU PROBABLY WANT TO CHANGE THESE
// RX_NODE doesn't actually matter, logic is !TX --> RX
#define TX_NODE 0x333E6EFC
#define RX_NODE 0xD3E02E71

#define POLL_INTERVAL_MS 25
#define RADIO_SLEEP_AFTER_TX

// Seems to be a bug in the hardware (???). FIFO doesn't get cleared after CRC error in LoRa mode.
// Docs are unclear on how to do this via register command except to frob the global state
// e.g., Rx --> Stby --> Rx
#define RADIO_RESET_FIFO_AFTER_CRC_ERROR

// LoRa "wrapper" (lowest-level) driver layer
extern SX1276M1BxASWrapper g_SX1276M1BxASWrapper;

static void my_wait(void) { Events_WaitForEvents(0, POLL_INTERVAL_MS); }

static void valid_header(void) {
	debug_printf("%s\r\n", __func__);
}

static void rx_timeout(void) {
	debug_printf("%s\r\n", __func__);
}

static void tx_timeout(void) {
	debug_printf("%s\r\n", __func__);
}

static void radio_tx_done(void) {
	SX1276M1BxASWrapper *radio = &g_SX1276M1BxASWrapper;
	//debug_printf("%s\r\n", __func__);
#ifdef RADIO_SLEEP_AFTER_TX
	radio->Sleep();
#endif
}

static void rx_done(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
	static unsigned local_count = 0;
	static unsigned spur = 0;
	static unsigned crc_error = 0;
	my_pkt_t *pkt = (my_pkt_t *) payload;

	if (payload == NULL) { // our signal for CRC error
		crc_error++;
		debug_printf("%s(): CRC error\r\n", __func__);
		return;
	}

	if ( strncmp("Samraksh", pkt->name, sizeof(pkt->name)) != 0) {
		spur++;
		debug_printf("%s(): Spurious packet size:%d rssi:%d spur:%u\r\n", __func__, size, rssi, spur);
		return;
	}

	debug_printf("%s(): size: %d rssi:%d spur:%u crc:%u local:%u remote:%u\r\n", __func__, size, rssi, spur, crc_error, local_count, pkt->count);
	local_count++;
}

static void rx_error(void) {
#ifdef RADIO_RESET_FIFO_AFTER_CRC_ERROR
	SX1276M1BxASWrapper *radio = &g_SX1276M1BxASWrapper;
	radio->Standby(); // Goes back to RX in main loop
#endif
	rx_done(NULL, 0, 0, 0);
	//debug_printf("%s\r\n", __func__);
}

static void radio_change_channel( uint8_t currentChannel ) {
	debug_printf("%s\r\n", __func__);
}

static void radio_cad_done(bool channelActivityDetected) {
	debug_printf("%s\r\n", __func__);
}

static uint32_t get_cpu_id_hash(void) {
	// 96-bit (3 word) global unique ID
	uint32_t *id = (uint32_t *) 0x1FFFF7E8;
	uint32_t ret;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	CRC_ResetDR();
	ret = CRC_CalcBlockCRC(id, 3);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE);
	return ret;
}

static void native_link_test(void) {
	const unsigned RTC_TIMEBASE = 16384;
	my_pkt_t pkt;
	SX1276RadioEvents_t events;
	SX1276M1BxASWrapper *radio = &g_SX1276M1BxASWrapper;
	uint32_t id = get_cpu_id_hash();

	debug_printf("Native Link Test: %s\r\n", __DATE__);
	debug_printf("I am 0x%.8X\r\n", id);

	pkt.count = 0;
	strncpy(pkt.name, "Samraksh", sizeof(pkt.name));

	events.ValidHeaderDetected 	= valid_header;
	events.TxDone 				= radio_tx_done;
	events.TxTimeout 			= tx_timeout;
	events.RxDone 				= rx_done;
	events.RxTimeout 			= rx_timeout;
	events.RxError 				= rx_error;
	events.FhssChangeChannel 	= radio_change_channel;
	events.CadDone 				= radio_cad_done;

	radio->Initialize(&events);

	radio->SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    radio->SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

	radio->SetChannel(RF_FREQUENCY);
	radio->Sleep();
	WakeLock(1);
	my_wait(); // yield for a moment just in case

	// TX
	if (id == TX_NODE) {
		UINT32 interval = RTC_TIMEBASE * 10; // 0.1 Hz
		UINT32 next = CPU_Timer_GetCounter(RTC_32BIT);
		UINT32 now = next;
		UINT32 last_now;
		bool overflow = false;
		while(1) {
			// wait for now to zero-cross if overflow detected
			while (overflow && now >= last_now) { my_wait(); now = CPU_Timer_GetCounter(RTC_32BIT); }
			overflow = false;
			while (now < next) { my_wait(); now = CPU_Timer_GetCounter(RTC_32BIT); }
			radio->Send( (uint8_t *)&pkt, sizeof(pkt) );
			debug_printf("Sent: %u\r\n", pkt.count);
			pkt.count++;
			next = now+interval;
			if (next < now) { // overflow
				overflow = true;
				last_now = now;
			}
		}
	}

	//RX
	else {
		debug_printf("I am RX only\r\n");
		while(1) {
			if (radio->settings.State == RF_IDLE) { radio->Rx(0); }
			my_wait();
		}
	}
}

void ApplicationEntryPoint()
{
	native_link_test(); // does not return

    while(TRUE){
		::Events_WaitForEvents( 0, 100 );
    }
}
