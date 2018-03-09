#include <ArduinoPTP.h>
//#include <time.h>
//#include <stdio.h>
//#include <stdlib.h>

/*typedef struct _PTPMsg {
		uint16_t sourcePortID;
		uint8_t version;
		uint8_t domain;
		//long long clockID;
		uint8_t clockID[8];
		unsigned int transportSpecific; 
		unsigned int type; 
		uint16_t length;
		uint16_t flags;
		long long correction;
	//	char correction1[8];
		uint16_t sequenceID;
		uint8_t control;
uint8_t log_period; 
} PTPMsg_t;*/


typedef enum state {
	PTP_SYNC_WAIT,
	
	PTP_DELAY_RESPONSE_WAIT,
	
	PTP_ANNOUNCE_WAIT,
	
	PTP_FOLLOW_UP_WAIT
	
} ptpState_t;
char buffer[100];
int packetSize;
int packetByte;

//PTPMsg_t PTPMsg;
ptpState_t pstate = PTP_ANNOUNCE_WAIT;

uint8_t ArduinoPTP::begin() {
	if (!_general.begin(320)) return 1;
	if (!_event.begin(319)) return 1;
	pstate = PTP_ANNOUNCE_WAIT;
	return 0;
}
void ArduinoPTP::stop() {
	_general.stop();
	_event.stop();
}

void ArduinoPTP::handle() {
	checkState();
	//static int npackets = 0;
	
//	changeState(PTP_ANNOUNCE_WAIT);
   // pstate=PTP_ANNOUNCE_WAIT;
	//packetSize = _general.parsePacket();
	//if (packetSize) {
		// Do something to handle general packets.
	//}
	
//	changeState(PTP_SYNC_WAIT);
  //  pstate=PTP_SYNC_WAIT;
	//packetSize = _event.parsePacket();
	//if  (packetSize) {
		//npackets++;
		//if (npackets == 10) 
		//{
			//npackets = 0;
			//memset(buffer,0,sizeof(buffer));
		//	packetByte = _event_packet.read(buffer,44);
			
			//PTPMsg.control =buffer[32];
			
			//checkState();
			
			
		//Serial.print("PTP event: ");
	//Serial.print(_event.getSeconds());
		//Serial.print(".");
		//Serial.println(_event.getNanoseconds());
		
		//uint16_t remotePort=_event.remotePort();
        //IPAddress remoteIP=IPAddress(128,146,33,30);
		
		//Serial.print("packets: ");
		//Serial.println(tivaPacketCount);
		
		//int b=_event.beginPacket(remoteIP,remotePort);	
		//Serial.print("begin packet:");
		//Serial.println(b);
		//uint8_t byte1=0xab;
		//_event.write(byte1);
		//_event.setTxTimestamp(true);
		//Serial.println(_event.getTxTimestamp());
		//_event.endPacket();
		
		//Serial.print("packets: ");
		//Serial.println(tivaPacketCount);
		
		//Serial.print("timestamp info: ");
		//Serial.print(tivaTxTimestampDone);
	//	Serial.print(" ");
		//Serial.print(tivaSetTimestamp);
		//Serial.print(" ");
		//Serial.println(tivaNumChained);

		//Serial.print("PTP delay event:");
		//Serial.print(_event.getTxTimestampHi());
		//Serial.print(".");
		//Serial.println(_event.getTxTimestampLo());
		
	//	}
//	}
}

void ArduinoPTP::checkState()
{
	
	 switch(pstate)
	 {
		 case PTP_ANNOUNCE_WAIT:
		 packetSize = _event.parsePacket();
		 if (_general.parsePacket())
		 {
			 packetByte = _general.read(buffer,2);
			 //Serial.println(packetByte);
			unsigned int transportSpecific= (buffer[0]>>4)& 0xF;
            unsigned int type=(buffer[0])& 0xF;
			 //Serial.println(transportSpecific,HEX);
			 //Serial.println(type,HEX);
			// Serial.print("Announce message");
			// buffer[0]=(buffer[0]>>4)& 0xF;
			// Serial.println(buffer[0]);
			 if((type)==11)
			 {
				 Serial.println("Announce message");
				 
				 pstate=PTP_SYNC_WAIT;
				 checkState();
			 }
		 }
		 break;
		 
		 case PTP_SYNC_WAIT:
        // if (PTPMsg.control==0)
		 //{
			// uint64_t t0_sec=(buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
			// uint32_t t0_nanosec=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
		 //}			 
		 break; 
		 
		 case PTP_FOLLOW_UP_WAIT:
		 break;
		 
		 case PTP_DELAY_RESPONSE_WAIT:
		 break;
	 }
}

//void ArduinoPTP::changeState(state new_state)
//{
	
	//pstate = new_state;
//}
