/*
 * RadioTest.h
 */

#ifndef RadioTest_H_
#define RadioTest_H_

#include <tinyhal.h>
#include <Targets/Native/STM32F10x/DeviceCode/drivers/radio/SX1276/SX1276wrapper.h.h>

#define PAYLOAD_SIZE	5

struct Payload_t {
	UINT16 MSGID;
	UINT8 data[PAYLOAD_SIZE];
};



class RadioTestSend	{
public:
	static void TxDone( bool success);
	static void PacketDetected();
	static void RxDone (uint8_t *payload, uint16_t size );
	static void CadDone ( bool channelActivityDetected );
	static void DataStatusCallback( bool success, UINT16 number_of_bytes_in_buffer );
	static const RadioEvents_t radio_events = {
			TxDone, PacketDetected, RxDone, CadDone, DataStatusCallback
	};

	static void Test_0A_Timer1_Handler(void * arg){
			g_RadioTestSend.SendPacket();
	}

	/*
	 * Alternates between sending packets and sleeping
	 */
	static void Test_0A_Timer2_Handler(void * arg){
		static bool toggle = false;
		if(!toggle){
			toggle = true;
			for(int i = 0; i < 200; i++){
				g_RadioTestSend.SendPacket();
				//HAL_Time_Sleep_MicroSeconds(500);
			}
		}
		else{
			toggle = false;
			HAL_Time_Sleep_MicroSeconds(400000);
		}
	}
public:
	Payload_t msg;
	SamrakshRadio_I* radio;
	RadioEvents_t radio_events;

	BOOL Initialize();
	BOOL StartTest();
	void ScheduleSendPacket();

};

//extern RadioTest g_RadioTest;

void RadioTest_Initialize();

#endif /* RadioTest_H_ */



