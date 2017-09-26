/*
 * RadioTest.h
 */

#ifndef RadioTest_H_
#define RadioTest_H_

#include <tinyhal.h>
//#include <Samraksh/Radio.h>
#include <Targets/Native/Radios/SX1276/SamrakshSX1276hal.h>

namespace NSRadioFIFOTest{


#define LED_RED (GPIO_PIN)20
#define LED_GREEN (GPIO_PIN)21
#define LED_BLUE (GPIO_PIN)22
#define LED_ON_STATE false
#define LED_OFF_STATE true

#define MSG_EMBED 5
#define BYTELENGTHOFNESSAGE 255

typedef uint8_t RepeatedBasicMsgType ;
#define MSGSIZE BYTELENGTHOFNESSAGE

typedef uint16_t MsgIteratorT;

struct LongMessage{
	RepeatedBasicMsgType array[MSGSIZE+1];
	LongMessage(){
		SetMsg(1);
	};
	void SetMsg(RepeatedBasicMsgType x){
		for(auto i = 1; i < MSGSIZE+1 ; ++i){
			this->array[i] = x + i;
		}
	};

	bool operator ==(const LongMessage& rhs) const{
		for(uint16_t i = 1; i < MSGSIZE+1; ++i){
			if(array[i] != rhs.array[i]){
				return false;
			}
		}
		return true;
	};
	void operator=(const LongMessage& rhs){
		for(uint16_t i = 1; i < MSGSIZE+1 ; ++i){
			this->array[i] = rhs.array[i];
		}
	};
	bool operator<(const LongMessage& rhs) const{
		for(uint16_t i = 1; i < MSGSIZE+1; ++i){
			if(array[i] >= rhs.array[i]){
				return false;
			}
			else if(array[i] < rhs.array[i]){
				return true;
			}
		}
		return false;
	};

};



class TestObject_t {
public:
	static void TxDone( bool success);
	static void PacketDetected();
	static void RxDone (uint8_t *payload, uint16_t size );
	static void CadDone ( bool channelActivityDetected );
	static void DataStatusCallback( bool success, UINT16 number_of_bytes_in_buffer );
	SamrakshRadio_I::RadioEvents_t radio_events;

public:
	LongMessage msg_written;
	LongMessage msg_read;

	bool Initialize();

	void IncrementBuffers();
	bool WriteToBuffer();
	bool ReadFromBuffer();
	bool CompareBuffers();

};

void Test_InitializeAndRun();


}

#endif /* RadioTest_H_ */



