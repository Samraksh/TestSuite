/*
 * RadioTest.h
 */

#ifndef RadioTest_H_
#define RadioTest_H_

#include <tinyhal.h>
//#include <Samraksh/Radio.h>
#include <Targets/Native/Radios/SX1276/SamrakshSX1276hal.h>

namespace NSRadioSendReceiveTest{

#define LED_RED (GPIO_PIN)20
#define LED_GREEN (GPIO_PIN)21
#define LED_BLUE (GPIO_PIN)22
#define LED_ON_STATE false
#define LED_OFF_STATE true


	#define MSG_EMBED 5
	#define BYTELENGTHOFNESSAGE 32
	#define MSGSIZE BYTELENGTHOFNESSAGE
	typedef uint8_t RepeatedBasicMsgType ;
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
		//Check if the packet content is set by SetMsg
		bool CheckMsgConsistency(){
			for(auto i = 2; i < MSGSIZE+1 ; ++i){
				if(this->array[i] != this->array[i-1] + 1) return false;
			}
			return true;
		}
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



		LongMessage msg_written;
		LongMessage msg_read;

		bool m_RxDone;
		bool m_PacketDetected;

		bool m_PacketSentAccepted;
		bool m_PacketSentTxDone;

		bool m_RxPacketCorrect;
		bool m_EndOfTest;

	public:



		bool Initialize();

		void SendPacket();
		void ReceivePacket(uint8_t *payload, uint16_t size );
		bool CheckEndOfTest();
		void StartListenning();

	};



	void Test_InitializeAndRun();

} // End of namespace

#endif /* RadioTest_H_ */



