#include <ArduinoPTP.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//using namespace std;

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
	
	PTP_SEND_DELAY_REQ,
	
	PTP_TIMESTAMPS,
	
	PTP_TIVA_TX_TIMESTAMP_DONE
	
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
 uint8_t clockID[8];
 uint8_t clockID1[8];
 uint32_t t0_prime_seconds=0;
 uint32_t t0_prime_nanoseconds=0;
 uint64_t t0_seconds;
 uint32_t t0_nanoseconds;
 //char timestamp[6];
 //char timestamp1[6];
 uint8_t control;
 uint16_t sequenceID_sync;
 uint16_t sequenceID_follow_up;
 uint16_t sequenceID_Delay=0;
 uint32_t t1_prime_seconds;
 uint32_t t1_prime_nanoseconds;
 uint64_t t1_seconds;
 uint32_t t1_nanoseconds;
 uint16_t sequenceID_Delay_response;
 uint16_t t0_secondsHi;
 uint32_t t0_secondsLo;
 uint16_t t1_secondsLo;
 uint32_t t1_secondsHi;
 
 //int i=0;
 //int k=0;
 //int check_clock=0
void ArduinoPTP::checkState()
{
	uint16_t remotePort=_event.remotePort();
	IPAddress remoteIP=/*IPAddress(128,146,33,30)*/ _event.remoteIP();
	 switch(pstate)
	 {
		 case PTP_ANNOUNCE_WAIT:
		 packetSize = _event.parsePacket();
		 if (_general.parsePacket())
		 {
			 packetByte = _general.read(buffer,30);
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
				 // We have an announce message.
				 Serial.println("Got an announce message");
				// Serial.println("Announce message");
				 // memcpy
				// for(k=20;k<28;k++)
				// {
					// clockID[i]=buffer[k];
					 //Serial.print(clockID[i],HEX);
                   //  i++;
				 //}
				 memcpy(clockID,buffer + 20,8);
				 memcpy(clockID1,buffer + 20,8);
				 
				 pstate=PTP_SYNC_WAIT;
				 //Serial.println("Switching to sync mode");
				 //checkState();
				 return;
			 } else {
				// This packet wasn't an announce message.
				// Do nothing.
				return;
			 }
		 } else {
			 // No packet. Do nothing.
			 return;
		 } 
		 
		case PTP_SYNC_WAIT:
		if (_event.parsePacket()) {
			// i=0;
			Serial.println("got a sync message");
			t0_prime_seconds = _event.getSeconds();
			t0_prime_nanoseconds=_event.getNanoseconds();
			//Serial.println(t0_prime_seconds);
			memset(buffer,0,sizeof(buffer));
			//Serial.println(sizeof(buffer));
			//Serial.println("Working");
			packetByte = _event.read(buffer,44);
			//Serial.println(packetByte);
			//Serial.println(sizeof(buffer));
			// memcmp
			//for(k=20;k<28;k++)
			//{
				//if (buffer[k]==clockID[i]) 
				//{
					if(!(memcmp(clockID,buffer + 20,8))) //use a different clockID array each time.
					{
					//check_clock++; 
					//Serial.print(buffer[k],HEX);
					//} 
					//i++;
			//}
			//Serial.println("Working");
			//if (check_clock==8)
			//{
				//Serial.println("Sync clock verified.");
				//Serial.println("Getting timestamps");
				//t0_prime_seconds = _event.getSeconds(); //Serial.println(_event.getSeconds()); Serial.println(t0_prime_seconds);
		        //t0_prime_nanoseconds=_event.getNanoseconds();
				
				//t0_seconds=(buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
				//   t0_nanoseconds=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
				  // itoa(t0_seconds,timestamp,10);
				  // Serial.println(timestamp);
				  // Serial.println(t0_nanoseconds);
				  sequenceID_sync=(buffer[30] << 8) | buffer[31];
				  //Serial.print("Sync ID ");
				  //Serial.println(sequenceID_sync);
				
			}
				
			//}
		//	pstate=PTP_DELAY_RESPONSE_WAIT;
			//Serial.println("Changing state to follow up");
			//return;
			// if (PTPMsg.control==0)
			 //{
				// uint64_t t0_sec=(buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
				// uint32_t t0_nanosec=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
			 //}
		}
		
		else {
			return;
		}
		if(_general.parsePacket())
		{
			Serial.println("general packet received");
			  memset(buffer,0,sizeof(buffer));
			  packetByte = _general.read(buffer,44);
			  control=buffer[32] &0xF;
			  //Serial.println(control);
			 
			  
			  if ((!(memcmp(clockID1,buffer + 20,8))) && (control==2)) 
			  {
				   Serial.println("The general packet is a follow-up");
				   //Serial.println("Getting timestamps");
				   t0_seconds=(buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
				   t0_nanoseconds=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
				   
				   t0_secondsHi=(t0_seconds >>32) & 0xFFFFFFFF;
				   t0_secondsLo=(t0_seconds >>0) & 0xFFFFFFFF;
				  //itoa(t0_seconds,timestamp,10);
				  // Serial.println(timestamp);
				 //  Serial.println(t0_nanoseconds);
				   sequenceID_follow_up=(buffer[30] << 8) | buffer[31];
				  //Serial.print("Follow-up ID ");
				   //Serial.println(sequenceID_follow_up);
				   //pstate=PTP_DELAY_RESPONSE_WAIT;
				   //return;
				   pstate=PTP_SEND_DELAY_REQ;
				   //Serial.println("Switching to delay request mode");
			       
			  }
			  return;
		}
		else return;
		 
		 /*case PTP_FOLLOW_UP_WAIT:
		 _event.parsePacket();
		  if (_general.parsePacket())
		  {
			  Serial.println("general packet received");
			  memset(buffer,0,sizeof(buffer));
			  packetByte = _general.read(buffer,44);
			  control=buffer[32] &0xF;
			  Serial.println(control);
			 
			  
			  if (control==2)
			  {
				   Serial.println("The general packet is a follow-up");
				   t0_seconds=(buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
				   t0_nanoseconds=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
				   itoa(t0_seconds,timestamp,10);
				   Serial.println(timestamp);
				   Serial.println(t0_nanoseconds);
				   pstate=PTP_DELAY_RESPONSE_WAIT;
				   return;
			  }
			  
			  else
			  {
				  return;
			  }
			  
			  
			  
			 
		  }
		 else {
			 return;
		 }*/
		 
		 case PTP_SEND_DELAY_REQ:
		// Serial.println(remoteIP);
	//	for(int i=0;i<3;i++)
		//{
		 _event.beginPacket(remoteIP,remotePort); 
		 //_event.setTxTimestamp(true);
		 //t1_prime_seconds=_event.getTxTimestampHi();
		// t1_prime_nanoseconds=_event.getTxTimestampLo();
		 sequenceID_Delay++;
		 Serial.println(sequenceID_Delay);
		 buffer_out[0]=(((0x0 & 0xF)<<4)|((0x1 & 0xF) <<0));
		 buffer_out[1]=(((0x0 & 0xF)<<4)|((0x2 & 0xF) <<0));
		 buffer_out[2]=0x00;
		 buffer_out[3]=0x2c;
		 buffer_out[4]=0x00;
		 buffer_out[5]=255;
		 buffer_out[6]=0x00;
		 buffer_out[7]=0x00;		
         buffer_out[8]=	0x00;
		 buffer_out[9]=0x00;
		 buffer_out[10]=0x00;
		 buffer_out[11]=0x00;
		 buffer_out[12]=0x00;
		 buffer_out[13]=0x00;
		 buffer_out[14]=0x00;
		 buffer_out[15]=0x00;
		 buffer_out[16]=0x01;
		 buffer_out[17]=0x23;
		 buffer_out[18]=0x45;
		 buffer_out[19]=0x67;
		 buffer_out[20]=0x01;
		 buffer_out[21]=0x00;
		 buffer_out[22]=0xff;
		 buffer_out[23]=0xfe;
		 buffer_out[24]=0x5e;
		 buffer_out[25]=0x00;
		 buffer_out[26]=0x01;
		 buffer_out[27]=0x81;
		 buffer_out[28]=0x00;
		 buffer_out[29]=0x02;
		 buffer_out[30]=((sequenceID_Delay >>8) & 0xFF);
		 buffer_out[31]=((sequenceID_Delay >>0) & 0xFF);
		 buffer_out[32]=0x01;
		 buffer_out[33]=127;
		 _event.setTxTimestamp(true);
		 //Serial.println("Getting timestamps");
		 t1_prime_seconds=_event.getTxTimestampHi();
		 t1_prime_nanoseconds=_event.getTxTimestampLo();
		 //Serial.println(t1_prime_seconds);
		// Serial.println(t1_prime_nanoseconds);
		 buffer_out[34]==((t1_prime_seconds >> 40) & 0xFF);
		 buffer_out[35]=((t1_prime_seconds >> 32) & 0xFF);
		 buffer_out[36]=((t1_prime_seconds >> 24) & 0xFF);
		 buffer_out[37]=((t1_prime_seconds >> 16) & 0xFF);
		 buffer_out[38]=((t1_prime_seconds >> 8) & 0xFF);
		 buffer_out[39]=((t1_prime_seconds >> 0) & 0xFF);
		 buffer_out[40]=((t1_prime_nanoseconds >> 24) & 0xFF);
		 buffer_out[41]=((t1_prime_nanoseconds >> 16) & 0xFF);
		 buffer_out[42]=((t1_prime_nanoseconds >> 8) & 0xFF);
		 buffer_out[43]=((t1_prime_nanoseconds >> 0) & 0xFF);
		 _event.write(buffer_out,44);
		 
		 _event.endPacket();
		 t1_prime_seconds=_event.getTxTimestampHi();
		 Serial.println(t1_prime_seconds);
		t1_prime_nanoseconds=_event.getTxTimestampLo();
		//Serial.println(t1_prime_nanoseconds);
		//Serial.println(t0_prime_seconds);
		//Serial.println(t0_prime_nanoseconds);
		 pstate = PTP_TIVA_TX_TIMESTAMP_DONE;
		 return;
		 //return;
		 //Serial.println(tivaTxTimestampDone);
		// Serial.print("Delay request ID ");
		//Serial.println(sequenceID_Delay);
		
		//Do nothing before this!!
		// if (tivaTxTimestampDone)
		 //{
			 //Serial.println("tivaTxTimestampDone is true");
			 //Serial.println(t1_prime_seconds);
		     //Serial.println(t1_prime_nanoseconds);
			 
			 
		 //pstate=PTP_DELAY_RESPONSE_WAIT;
		// return;
		 //}
		 //else{
			 //Serial.println("tivaTxTimestampDone is false");
			// pstate=PTP_SYNC_WAIT;
			// pstate=PTP_DELAY_RESPONSE_WAIT;
			// return;
		// }
	//	}
		 //pstate=PTP_DELAY_RESPONSE_WAIT;
		 //return;
		 
		 case PTP_TIVA_TX_TIMESTAMP_DONE:
		 if (!tivaTxTimestampDone)
		 {
			 ///if(t1_prime_seconds==0)
			// {
				 pstate=PTP_SEND_DELAY_REQ;
				 return;
			 //}
			
			 //Serial.println(t1_prime_seconds);
			// pstate=PTP_DELAY_RESPONSE_WAIT;
		 }
		 else 
		 {
			 
			 pstate=PTP_DELAY_RESPONSE_WAIT;
			 return;
		 }
		 
		 case PTP_DELAY_RESPONSE_WAIT:
		// ptpState_t PTP_END;
		 _event.parsePacket();
		 if(_general.parsePacket())
		 {
			 Serial.println("general packet received");
			  memset(buffer,0,sizeof(buffer));
			  packetByte = _general.read(buffer,44);
			  control=buffer[32] &0xF;
			  sequenceID_Delay_response=(buffer[30] << 8) | buffer[31];
			  
			 // t1_seconds=(buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
			  //t1_nanoseconds=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
			 // itoa(t1_seconds,timestamp1,10);
			  
			 // Serial.println(control);
			  
			 // Serial.println(sequenceID_Delay);
			
			  
			  if (control==3)
			  {
				  //Serial.print("Delay response ID ");
			      //Serial.println(sequenceID_Delay_response);
				  Serial.println("the message is a delay response");
				  Serial.println(sequenceID_Delay_response);
				  if(sequenceID_Delay==sequenceID_Delay_response)
				  {
					  Serial.println("the delay IDs match up");
					  
					  //Serial.println(t1_seconds);
					  //Serial.println(t1_nanoseconds);
				  if(sequenceID_follow_up==sequenceID_sync)
				  {
					  if(sequenceID_Delay==(sequenceID_sync))
					  {
					  Serial.println("the sync and follow-up IDs match up");
					  Serial.println("Get all the timestamps here!");
					  pstate=PTP_TIMESTAMPS;
					  return;
					  }
					  //FILE *fp;
					 // fp = fopen("C:/Users/ara/Documents/PTP Timestamps.txt","w");
					  //int fclose(FILE *fp);
					  //return ;
					  
					 //t1_seconds=(buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
			  // t1_nanoseconds=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
			  // itoa(t1_seconds,timestamp1,10);
					/* Serial.print(timestamp);
					  Serial.print("   ");
					  Serial.print(t0_nanoseconds);
					  Serial.print("   ");
					  Serial.print(t0_prime_seconds);
					  Serial.print("   ");
					  Serial.print(t0_prime_nanoseconds);
					  Serial.print("   ");
					  Serial.print(t1_prime_seconds);
					  Serial.print("   ");
					  Serial.println(t1_prime_nanoseconds);
					  /*Serial.print("   ");
					  Serial.print(timestamp1);
					  Serial.print("   ");
					  Serial.println(t1_nanoseconds);
					  
				  }
				  
					
				  
				  
				  //Serial.print("Delay response ID ");
			      //Serial.println(sequenceID_Delay_response);
				 // Serial.println("delay response packet received"); 
				  //Serial.println((buffer[30] << 8) | buffer[31]);
				 /* if(sequenceID_follow_up==sequenceID_sync)
				  {
				  Serial.println(sequenceID_Delay);
				  Serial.println(sequenceID_Delay_response);
				  Serial.println(sequenceID_follow_up);
				  Serial.println(sequenceID_sync);
				  }*/
				// pstate=PTP_SYNC_WAIT;
				  }
			  }
		 }
			  return;
		 }
		 else 
		 
	     return;
		 
		 case PTP_TIMESTAMPS:
		 t1_seconds=(buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
	     t1_nanoseconds=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
		 t1_secondsLo=(t1_seconds >>32) & 0xFFFFFFFF;
		 t1_secondsHi=(t1_seconds >>0) & 0xFFFFFFFF;
		 //itoa(t1_seconds,timestamp1,10);
		//Serial.println("PTP TIMESTAMPS");
		 //Serial.println(timestamp);
		 //Serial.print("   ");
		 Serial.print(t0_secondsLo);
		 //Serial.println(t0_secondsHi);
		 Serial.print("   ");
		Serial.print(t0_nanoseconds);
		Serial.print("   ");
		 Serial.print(t0_prime_seconds);
		 Serial.print("   ");
		Serial.print(t0_prime_nanoseconds);
		Serial.print("   ");
		 Serial.print(t1_prime_seconds);
		 Serial.print("   ");
		 Serial.print(t1_prime_nanoseconds);
		 Serial.print("   ");
		 //Serial.println(timestamp1);
		 Serial.print(t1_secondsHi);
		 Serial.print("   ");
		 //Serial.println(t1_secondsLo);
		 Serial.println(t1_nanoseconds);
		 //memset(timestamp,0,sizeof(timestamp));
		 //memset(timestamp1,0,sizeof(timestamp1));
		 pstate=PTP_SYNC_WAIT;
		 return;
		 default:
		 return;
	 }
}

//void ArduinoPTP::changeState(state new_state)
//{
	
	//pstate = new_state;
//}
