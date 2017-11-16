#include <ArduinoPTP.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

char buffer[100];
//char buffer_event[100];
int v0 = 0; //0 if debugging mode is off.
int v1=0;//0 if debugging is off.
int v2=0;//0 if debugging is off.
int v3=0;//0 if debugging is off.
int v4=1;//0 if debugging is off.
int globalCountgeneral = 0; //keep track of number of general packets.
int globalCountevent = 0; //keep track of number of event packets.


struct _PTPMsg {
		uint16_t sourcePortID;
		uint8_t version;
		uint8_t domain;
		long clockID;
		unsigned int transportSpecific; 
		unsigned int type; 
		uint16_t length;
		uint16_t flags;
		long correction;
		uint16_t sequenceID;
		uint8_t control;
		uint8_t log_period; 
} PTPMsg;

struct _TimeStamp {
	long seconds; 
	uint32_t nanoseconds; 
	
} TimeStamp;

struct _ClockQuality {
	uint8_t clockClass;
	uint8_t clockAccuracy;
	uint16_t clockVariance;
} ClockQuality;

struct _AnnounceMsg{
	struct _TimeStamp originTimestamp;
	long clockID;
	uint16_t currentUtcOffset;
	uint8_t grandmasterPriority1;
	uint8_t grandmasterPriority2;
	uint16_t stepsRemoved;
	struct _ClockQuality masterClockQuality;
	uint8_t TimeSource;
	
} AnnounceMsg;

struct _SyncMsg {
	struct _TimeStamp originTimestamp;
} SyncMsg;

struct _FollowupMsg {
	struct _TimeStamp preciseOriginTimestamp;
} FollowupMsg;

uint8_t ArduinoPTP::begin() {
	if (!_general.begin(320)) return 1;
	if (!_event.begin(319)) return 1;
	return 0;
}
void ArduinoPTP::stop() {
	_general.stop();
	_event.stop();
}
void ArduinoPTP::handle() {
	int packetSize;
	int packetByte;
	struct _PTPMsg PTPMsg;
	struct _AnnounceMsg AnnounceMsg;
	struct _FollowupMsg FollowupMsg;
	struct _SyncMsg SyncMsg;
	packetSize = _general.parsePacket();
	//Serial.println("packet size:%d", packetSize);
	if (packetSize) {
		// Do something to handle general packets.
		globalCountgeneral++;
		if(globalCountgeneral%10==0)
		{
			memset(buffer,0,sizeof(buffer));
			if (v4)
			{
				Serial.println("");
			}
			if (v3)
			{
			Serial.print(globalCountgeneral);
			}
			if (packetSize==44)
			packetByte = _general.read(buffer,44);
		    else
		    packetByte = _general.read(buffer,64);
			if (v1)
			{
			Serial.println(packetByte);
			}
			PTPMsg.version=buffer[1];
			PTPMsg.domain=buffer[4];
			PTPMsg.sourcePortID=(buffer[28] << 8) | buffer[29];
			PTPMsg.transportSpecific= (buffer[0]>>4)& 0xF;
			PTPMsg.type=(buffer[0])& 0xF;
			PTPMsg.length=(buffer[2] << 8) | buffer[3];
			PTPMsg.flags=((buffer[6] << 8) | buffer[7]);
			PTPMsg.sequenceID=(buffer[30] << 8) | buffer[31];
			PTPMsg.control=buffer[32];
			PTPMsg.log_period=buffer[33];
			PTPMsg.correction=(buffer[8] << 56) | (buffer[9] << 48) | (buffer[10] << 40) | (buffer[11] << 32) | (buffer[12] << 24) | (buffer[13] << 16) | (buffer[14] << 8) | (buffer[15]);
			PTPMsg.clockID =(buffer[20] << 56) | (buffer[21] << 48) | (buffer[22] << 40) | (buffer[23] << 32) | (buffer[24] << 24) | (buffer[25] << 16) | (buffer[26] << 8) | (buffer[27]);
				
			
			
			if (v1)
			{
			for (int i=0;i<44;i++)
			{
				
				
				Serial.print(buffer[i],HEX);
				Serial.print(" ");
			}
			}
			if (v4)
			{
				Serial.print("Printing Version info general:");
				Serial.print(PTPMsg.version);
				Serial.println("");
				Serial.print("Printing Domain info general:");
				Serial.print(PTPMsg.domain);
				Serial.println("");
				Serial.print("Printing source port ID general:");
				Serial.print(PTPMsg.sourcePortID);
				Serial.println("");
				Serial.print("Printing transport specific general:");
				Serial.print(PTPMsg.transportSpecific,HEX);
				Serial.println("");
				Serial.print("Printing message type general:");
				Serial.print(PTPMsg.type,HEX);
				Serial.println("");
				Serial.print("Printing length info general:");
				Serial.print(PTPMsg.length);
				Serial.println("");
				Serial.print("Printing flags info general:");
				Serial.print(PTPMsg.flags,HEX);
				Serial.println("");
				Serial.print("Printing sequence ID info general:");
				Serial.print(PTPMsg.sequenceID);
				Serial.println("");
				Serial.print("Printing control info general:");
				Serial.print(PTPMsg.control);
				Serial.println("");
				Serial.print("Printing log period info general:");
				Serial.print(PTPMsg.log_period);
				Serial.println("");
				Serial.print("Printing clock info general:");
				Serial.println(PTPMsg.clockID,HEX);
				Serial.print("Printing correction info general:");
				Serial.println(PTPMsg.correction);
				
				
			}
			if (packetSize==44)
			{
			    FollowupMsg.preciseOriginTimestamp.seconds = (buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
				FollowupMsg.preciseOriginTimestamp.nanoseconds=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
				
				if(v4)
			{
				//Serial.print(packetSize);
				Serial.print("Printing follow-up timestamp:");
				Serial.println(FollowupMsg.preciseOriginTimestamp.seconds);
				Serial.println(FollowupMsg.preciseOriginTimestamp.nanoseconds);
			}
			}
			
			
			
			if (v0)
		{
		Serial.println("It is working!");
		}
			
			
			
		
		
}

	}

	packetSize = _event.parsePacket();
	if  (packetSize) {
		// Do something to handle event packets
		if (v0)
		{
		Serial.println("It is working");
		}
		globalCountevent++;
		if(globalCountevent%10==0)
		{
			memset(buffer,0,sizeof(buffer));
	
			if (v3)
			{
				Serial.println("");
			}
			if (v1)
			{
			Serial.println(globalCountevent);
			}
			packetByte = _event.read(buffer,44);
			if (v1)
			{
			Serial.println(packetByte);
			}
			PTPMsg.version=buffer[1];
			PTPMsg.domain=buffer[4];
			PTPMsg.sourcePortID=(buffer[28] << 8) | buffer[29];
			PTPMsg.transportSpecific= (buffer[0]>>4)& 0xF;
			PTPMsg.type=(buffer[0])& 0xF;
			PTPMsg.length=(buffer[2] << 8) | buffer[3];
			PTPMsg.flags=((buffer[6] << 8) | buffer[7]);
			PTPMsg.sequenceID=(buffer[30] << 8) | buffer[31];
			PTPMsg.control=buffer[32];
			PTPMsg.log_period=buffer[33];
			PTPMsg.correction=(buffer[8] << 56) | (buffer[9] << 48) | (buffer[10] << 40) | (buffer[11] << 32) | (buffer[12] << 24) | (buffer[13] << 16) | (buffer[14] << 8) | (buffer[15]);
			PTPMsg.clockID =(buffer[20] << 56) | (buffer[21] << 48) | (buffer[22] << 40) | (buffer[23] << 32) | (buffer[24] << 24) | (buffer[25] << 16) | (buffer[26] << 8) | (buffer[27]);
			
			if (v4)
			{
				Serial.print("Printing Version info event:");
				Serial.print(PTPMsg.version);
				Serial.println("");
				Serial.print("Printing Domain info event:");
				Serial.print(PTPMsg.domain);
				Serial.println("");
				Serial.print("Printing source port ID event:");
				Serial.print(PTPMsg.sourcePortID);
				Serial.println("");
				Serial.print("Printing transport specific event:");
				Serial.print(PTPMsg.transportSpecific,HEX);
				Serial.println("");
				Serial.print("Printing message type event:");
				Serial.print(PTPMsg.type,HEX);
				Serial.println("");
				Serial.print("Printing length info event:");
				Serial.print(PTPMsg.length);
				Serial.println("");
				Serial.print("Printing flags info event:");
				Serial.print(PTPMsg.flags,HEX);
				Serial.println("");
				Serial.print("Printing sequence ID info event:");
				Serial.print(PTPMsg.sequenceID);
				Serial.println("");
				Serial.print("Printing control info event:");
				Serial.print(PTPMsg.control);
				Serial.println("");
				Serial.print("Printing log period info event:");
				Serial.print(PTPMsg.log_period);
				Serial.println("");
				Serial.print("Printing clock info event:");
				Serial.println(PTPMsg.clockID,HEX);
				Serial.print("Printing correction info event:");
				Serial.println(PTPMsg.correction);
				
			}
			
			if (v1)
			{
			for (int i=0;i<44;i++)
			{
				
				
				Serial.print(buffer[i],HEX);
				Serial.print(" ");
			}
			}
			if (v2)
			{
				Serial.print("Printing Version info event:");
				Serial.print(PTPMsg.version);
				Serial.println("");
				Serial.print("Printing Domain info event:");
				Serial.print(PTPMsg.domain);
				Serial.println("");
				Serial.print("Printing source port ID event:");
				Serial.print(PTPMsg.sourcePortID);
				Serial.println("");
				Serial.print("Printing clock info event:");
				Serial.println("");
			}
			
			
				/*for(int j=34;j<40;j++)
				{
				SyncMsg.originTimestamp.secondsField[i]=buffer[j];
				i++;
				}
				i=0;
			    for(int j=40;j<44;j++)
				{
				SyncMsg.originTimestamp.nanosecondsField[i]=buffer[j];
				i++;
				}
				if(v4)
			{
				Serial.print(packetSize);
				Serial.print("Printing sync timestamp:");
				for(int j=0;j<6;j++)
					Serial.print(SyncMsg.originTimestamp.secondsField[j]);
				    
			}*/
		
}
}
}

