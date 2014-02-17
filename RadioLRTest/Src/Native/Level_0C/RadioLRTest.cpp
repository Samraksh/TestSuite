////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* This program verifies if receive on the LR radio extension board works.
 * Comment out the send portion and deploy on eMote where receive has to be tested.
 * Uncomment send portion on eMote which will do the sending.	
 */

#include "RadioLRTest.h"
#include <Samraksh\HAL_util.h>
#include <radio\RF231\RF231.h>

// Reflects #defines above
//#include "rf231.c"

//#define USE_LCD
//#define USE_UART
#define USE_PA_BRD
//#define USE_CLOCKS

#define OUTPUT_TEST_POWER 0x0 // 0x0-highest 0xF-lowest
// One or the other
#define DO_TX_TEST 0
#define DO_RX_TEST 1



//---//
RadioLRTest testObject;

GPIO_InitTypeDef GPIO_InitStructure;
SPI_InitTypeDef SPI_InitStruct;
uint8_t fb_read_buf[128];
uint8_t fb_write_buf[128];
int lq; // Link quality


extern "C"
{

void* myReceiveHandler (void *msg, UINT16 Size){

	UINT8 * data = (UINT8 *) msg;

	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);

	if(testObject.currentpackedId == data[0])
		testObject.successfulTestCount++;

	RadioLRTest::RadioReceivedPending = FALSE;

	Message_15_4_t** temp = &recv_mesg_carrier_ptr;
	recv_mesg_carrier_ptr = (Message_15_4_t *) msg;
	return (void *) *temp;

}

void mySendAckHandler(void *msg, UINT16 size, NetOpStatus state)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 24, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 24, FALSE);

	RadioLRTest::RadioAckPending = FALSE;
}

}



BOOL RadioLRTest::Initialize(int seedValue, int numberOfEvents)
{
	hal_printf("Check Point 1.a\n");
		//CPU_GPIO_Initialize();
	CPU_SPI_Initialize();

	hal_printf("Check Point 1.b\n");
	RadioAckPending = FALSE;
	radioID = RF231RADIOLR;
	numberOfRadios = 1;
	mac_id = 1;
	DeviceStatus result;

	radioEventHandler.SetRecieveHandler(&myReceiveHandler);
	radioEventHandler.SetSendAckHandler(&mySendAckHandler);


	result = CPU_Radio_Initialize(&radioEventHandler , radioID, numberOfRadios, mac_id );


	if(result != DS_Success)
	{
		DisplayStats(FALSE,"Radio Initialization failed", NULL, NULL);
		return FALSE;
	}

	this->numberOfEvents = numberOfEvents;

	msg_carrier_ptr = & msg_carrier;


	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24 , FALSE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25 , FALSE);

	successfulTestCount = 0;


//Taken from D:\AnanthAtSamraksh\APPS-FOR-TESTING-MOTE\ExtRad_test\src\main.c
	
#if 0
#ifdef USE_PA_BRD
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOF | RCC_APB2Periph_AFIO, ENABLE);

  // SPI clock and MOSI pins
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // SPI MISO pin
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  SPI_StructInit(&SPI_InitStruct);
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_Init(SPI2, &SPI_InitStruct);
  SPI_SSOutputCmd(SPI2, DISABLE);
  SPI_Cmd(SPI2, ENABLE);

  // RadUp, RadOff
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  // RadInt
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // RadNow, CPS
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
#else
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // PE1 radio Interrupt
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  SPI_StructInit(&SPI_InitStruct);
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_Init(SPI1, &SPI_InitStruct);
  SPI_SSOutputCmd(SPI1, DISABLE);
  SPI_Cmd(SPI1, ENABLE);
#endif
#endif


  rf231_reset();



/*
  printf("RF231 reset complete\r\n");
  rf231_print_state();
  rf231_print_mn();
  rf231_print_pn();
  rf231_print_vn();
  rf231_print_irq();
  rf231_print_vreg();

  printf("Disabling CLKM and lower drive strength\r\n");
  // Disable CLKM and lower drive strength
  rf231_reset_clkm();

  printf("Configuring Antenna Diversity\r\n");
  // Enable Antenna diversity.
  rf231_enable_ant_div();
*/


  // Enable external PA and control from RF231
  printf("Enable External PA controls\r\n");
  rf231_enable_pa_rxtx();
  RF231_AMP(1);


/*
  // Set output power to lowest
  printf("Setting output power to setting 0x%X\r\n", OUTPUT_TEST_POWER);
  rf231_set_output_power(OUTPUT_TEST_POWER);
*/


  // Enable all IRQs
  printf("Enable all IRQs\r\n");
  rf231_set_irq_mask(0xFF);


  printf("Setting to channel 26\r\n");
  rf231_set_channel(26);


#if 0
  // Go to TRX_OFF
  rf231_goto_trx_off();
  rf231_print_state();

  // Sleep test
  //rf231_goto_sleep();
  //while(1) ;

  if(DO_TX_TEST) {
	  int j=0;
	  int g=0;
	  // Go to PLL_ON
	  rf231_goto_pll_on();
	  rf231_print_state();


	  while(1) {
		  for(int i=0; i<1000000; i++) { ; }
		  write_fb(fb_write_buf,128);
		  for(int i=0; i<100000; i++) { ; }
		  rf231_goto_tx_start();
		  for(int i=0; i<100000; i++) { ; }
		  if (rf231_print_irq() & 0x8) {
			  printf("Packet sent\r\n");
			  j++;
		  }
		  else {
			  printf("Packet tx fail!\r\n");
			  g++;
		  }
	  }
  }
#endif

#if 0
  if (DO_RX_TEST) {
	  rf231_goto_rx_on();
	  for(int i=0; i<1000000; i++) { ; }
	  rf231_print_state();

	  while (1) {
		  if (rf231_print_irq() & 0x8) {

			  lq = read_fb(fb_read_buf);
			  printf("Got packet\r\n");

		  }
	  }
  }
#endif



	return TRUE;

}

BOOL RadioLRTest::DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, int accuracy)
{
	hal_printf("\r\nresult=%s\r\n", (result) ? "PASS":"FAIL");
	hal_printf("\r\naccuracy=%d\r\n", accuracy);
	hal_printf("\r\nresultParameter1=%s\r\n", resultParameter1);
	hal_printf("\r\nresultParameter2=%s\r\n", resultParameter2);
	hal_printf("\r\nresultParameter3=%s\r\n", "null");
	hal_printf("\r\nresultParameter4=%s\r\n", "null");
	hal_printf("\r\nresultParameter5=%s\r\n", "null");

	return TRUE;
}

BOOL RadioLRTest::SendPacketSync(UINT16 dest, UINT8 dataType, void* msg, int Size)
{

	IEEE802_15_4_Header_t *header = msg_carrier_ptr->GetHeader();

	if(RadioAckPending == TRUE)
		return FALSE;

	header->length = Size + sizeof(IEEE802_15_4_Header_t);
	header->fcf = (65 << 8);
	header->fcf |= 136;
	header->dsn = 97;
	header->destpan = (34 << 8);
	header->destpan |= 0;
	header->dest =dest;
	header->src = MF_NODE_ID;
	header->network = 138;
	header->mac_id = 1;
	header->type = dataType;

	UINT8* lmsg = (UINT8 *) msg;
	UINT8* payload =  msg_carrier_ptr->GetPayload();

	for(UINT8 i = 0 ; i < Size; i++)
		payload[i] = lmsg[i];

	Message_15_4_t* mymsg = msg_carrier_ptr;

	if(mymsg != NULL){
		RadioAckPending = TRUE;
		RadioReceivedPending = TRUE;
		msg_carrier_ptr = (Message_15_4_t *) CPU_Radio_Send(RF231RADIOLR, (mymsg), header->length);
	}

	return TRUE;
}

// This test only checks if the manufacture id can be read
BOOL RadioLRTest::Level_0A()
{

	DeviceStatus result;
	BOOL SendResult;
	UINT32 i = 0;
	UINT8 regValue;

	result = CPU_Radio_TurnOn(radioID);

	if(result == DS_Success)
	{
		regValue = grf231RadioLR.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
		if(regValue != RF230_RX_ON)
		{
			DisplayStats(FALSE, "Read register failed after turn on",NULL, NULL);
			return FALSE;
		}
	}
	else
	{
		DisplayStats(FALSE,"Turn On Radio failed", NULL, NULL);
		return FALSE;
	}

	/*while(i++ < this->numberOfEvents)
	{
			// Try sending a packet now
		{
			msg.MSGID= i;
			msg.data[0] = 0;
			msg.data[1] = 1;
			msg.data[2] = 2;
			msg.data[3] = 3;
			msg.data[4] = 4;
				// Try sending a packet after turning it on
			SendResult = SendPacketSync(MAC_BROADCAST_ADDRESS, MFM_DATA, (void *) &msg.data, sizeof(Payload_t));

			while(RadioAckPending == TRUE);

			if(SendResult != TRUE)
			{
				DisplayStats(FALSE, "Send failed", NULL, NULL);
				return FALSE;
			}

		}
	}*/

	DisplayStats(TRUE,"Send Test Successful", NULL, NULL);
	return TRUE;
}

// This test writes data and then verifies the write has been successful
BOOL RadioLRTest::Level_0B()
{

	DeviceStatus result;
	BOOL SendResult;
	UINT32 i = 0;
	UINT8 regValue;
	INIT_STATE_CHECK();


	result = CPU_Radio_TurnOn(radioID);

	if(result == DS_Success)
	{
		regValue = grf231RadioLR.ReadRegister(RF230_TRX_STATUS) & RF230_TRX_STATUS_MASK;
		if(regValue != RF230_RX_ON)
		{
			DisplayStats(FALSE, "Read register failed after turn on",NULL, NULL);
			return FALSE;
		}
	}
	else
	{
		DisplayStats(FALSE,"Turn On Radio failed", NULL, NULL);
		return FALSE;
	}

	while(i++ < this->numberOfEvents)
	{

		// Try sending a packet now
		{
			msg.MSGID= i;
			msg.data[0] = i;
			msg.data[1] = 1;
			msg.data[2] = 2;
			msg.data[3] = 3;
			msg.data[4] = 4;

			currentpackedId = i;

			hal_printf("Transmitting Packet %d", i);
					// Try sending a packet after turning it on
			SendResult = SendPacketSync(MAC_BROADCAST_ADDRESS, MFM_DATA, (void *) &msg.data, sizeof(Payload_t));

					//while(RadioAckPending == TRUE);
			DID_STATE_CHANGE(RadioAckPending,"Timed out waiting for send done ack");

			DID_STATE_CHANGE(RadioReceivedPending,"Timed out waiting for receive ack from slave");

			for(volatile UINT32 i = 0; i < 50000; i++);

			if(SendResult != TRUE)
			{
				DisplayStats(FALSE, "Send failed", NULL, NULL);
				return FALSE;
			}

		}

	}

	DisplayStats(TRUE,"Send Test Successful", NULL, NULL);
	return TRUE;

}


BOOL RadioLRTest::Execute( int testLevel )
{
	BOOL result;

	switch(testLevel)
	{
	case 0:
		result = Level_0A();
		break;
	case 1:
		result = Level_0B();
		break;
	default:
		break;
	}


	return result;



} //Execute

