#include <ArduinoPTP.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

extern bool feedWatchdog;

char buffer[100];
//char buffer_event[100];
int v0 = 0; //0 if debugging mode is off.
int v1=0;//0 if debugging is off.
int v2=0;//0 if debugging is off.
int v3=0;//0 if debugging is off.
int v4=1;//0 if debugging is off.
#define var 0;
int globalCountgeneral = 0; //keep track of number of general packets.
int globalCountevent = 0; //keep track of number of event packets.
int k=0; 
int i=0;
char timestamp[6];
//IPAddress ip(192,168,1,177);
IPAddress remoteIP;
typedef struct _PTPMsg {
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
		char correction1[8];
		uint16_t sequenceID;
		uint8_t control;
		uint8_t log_period; 
} PTPMsg_t;

typedef struct _TimeStamp {
	long long seconds; 
	uint32_t nanoseconds; 
	
} TimeStamp_t;

typedef struct _ClockQuality {
	uint8_t clockClass;
	uint8_t clockAccuracy;
	uint16_t clockVariance;
} ClockQuality_t;

typedef struct _AnnounceMsg{
	struct _TimeStamp originTimestamp;
	uint8_t clockID[8];
	uint16_t currentUtcOffset;
	uint8_t grandmasterPriority1;
	uint8_t grandmasterPriority2;
	uint16_t stepsRemoved;
	struct _ClockQuality masterClockQuality;
	uint8_t TimeSource;
	
} AnnounceMsg_t;

typedef struct _SyncMsg {
	struct _TimeStamp originTimestamp;
} SyncMsg_t;

typedef struct _FollowupMsg {
	struct _TimeStamp preciseOriginTimestamp;
} FollowupMsg_t;

uint8_t ArduinoPTP::begin() {
	if (!_general.begin(320)) return 1;
	if (!_event.begin(319)) return 1;
	return 0;
}
void ArduinoPTP::stop() {
	_general.stop();
	_event.stop();
}

PTPMsg_t PTPMsg;
FollowupMsg_t FollowupMsg;
AnnounceMsg_t AnnounceMsg;
SyncMsg_t SyncMsg;

void ArduinoPTP::handle() {
	feedWatchdog=true;
	int packetSize;
	int packetByte;
	packetSize = _general.parsePacket();
	
	//Serial.println("packet size:%d", packetSize);
	if (packetSize) {
		// Do something to handle general packets.
		globalCountgeneral++;
		//if(globalCountgeneral%10==0)
		
			memset(buffer,0,sizeof(buffer));
			if (v4)
			{
				Serial.println("");
			}
			/*if (v3)
			{
			Serial.print(globalCountgeneral);
			}*/
			if (packetSize==44)
			packetByte = _general.read(buffer,44);
		    else if (packetSize==64)
		    packetByte = _general.read(buffer,64);
			/*if (v1)
			{
			Serial.println(packetByte);
			}*/
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
			/*i=0;
            for (k=8;k<16;k++)
			{
				PTPMsg.correction[i]=buffer[k];
				i++;
			}*/
			i=0;
			for(k=20;k<28;k++)
			{
				PTPMsg.clockID[i]=buffer[k];
				i++;
			}
			PTPMsg.correction=(buffer[8] << 56) | (buffer[9] << 48) | (buffer[10] << 40) | (buffer[11] << 32) | (buffer[12] << 24) | (buffer[13] << 16) | (buffer[14] << 8) | (buffer[15]);
			//PTPMsg.clockID =(buffer[20] << 56) | (buffer[21] << 48) | (buffer[22] << 40) | (buffer[23] << 32) | (buffer[24] << 24) | (buffer[25] << 16) | (buffer[26] << 8) | (buffer[27]);
			itoa(PTPMsg.correction,PTPMsg.correction1,10);
			//itoa(PTPMsg.clockID,PTPMsg.clockID1,10);
			
			
			/*if (v1)
			{
			for (int i=0;i<44;i++)
			{
				
				
				Serial.print(buffer[i],HEX);
				Serial.print(" ");
			}
			}
			if (v2)
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
				for(k=0;k<8;k++)
				{
					Serial.print(PTPMsg.clockID[k],HEX);
				}
				Serial.println("");
				//Serial.println("%02x02x02x02x02x02x02x02x",PTPMsg.clockID1);
				//Serial.print("Size of clockid:");
				//Serial.println(sizeof(PTPMsg.clockID));
				Serial.print("Printing correction info general:");
				Serial.println(PTPMsg.correction1);
				/*for(k=0;k<8;k++)
				{
					Serial.print(PTPMsg.correction[k]);
				}
				Serial.println("");
				Serial.print("Size of correction:");
				
				Serial.println(sizeof(PTPMsg.correction));
				
				
			}*/
			if (packetSize==44 && PTPMsg.control == 2)
			{
				//long bigNumber = (buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
				//FollowupMsg.preciseOriginTimestamp.seconds = bigNumber.toString();
			    FollowupMsg.preciseOriginTimestamp.seconds = (buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
				FollowupMsg.preciseOriginTimestamp.nanoseconds=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
				itoa(FollowupMsg.preciseOriginTimestamp.seconds,timestamp,10);
				
				/*if(v2)
			{
				//Serial.print(packetSize);
				Serial.print("Printing follow-up timestamp (seconds):");
				Serial.println(timestamp);
				Serial.print("Size of seconds timestamp:");
				Serial.println(sizeof(FollowupMsg.preciseOriginTimestamp.seconds));
				Serial.print("Printing follow-up timestamp (nanoseconds)");
				Serial.println(FollowupMsg.preciseOriginTimestamp.nanoseconds);
			}*/
			}
			else if (packetSize==64 && PTPMsg.control == 5)
			{
				AnnounceMsg.originTimestamp.seconds = (buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
				AnnounceMsg.originTimestamp.nanoseconds = (buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
				itoa(AnnounceMsg.originTimestamp.seconds,timestamp,10);
				AnnounceMsg.currentUtcOffset = (buffer[44] << 8)|(buffer[45]);
				AnnounceMsg.grandmasterPriority1=buffer[47];
				AnnounceMsg.grandmasterPriority2=buffer[52];
				AnnounceMsg.stepsRemoved=(buffer[61] << 8)|buffer[62];
				AnnounceMsg.TimeSource=buffer[63];
				AnnounceMsg.masterClockQuality.clockClass=buffer[48];
				AnnounceMsg.masterClockQuality.clockAccuracy=buffer[49];
				AnnounceMsg.masterClockQuality.clockVariance=(buffer[50] << 8)|buffer[51];
				k=0;
				for(i=53;i<61;i++)
				{
					AnnounceMsg.clockID[k]=buffer[i];
					k++;
				}
				/*if(v1)
				{
					Serial.print("Announce timestamp seconds:");
					Serial.println(timestamp);
					Serial.print("Announce timestamp nanoseconds:");
					Serial.println(AnnounceMsg.originTimestamp.nanoseconds);
					Serial.print("Announce timestamp utc offset:");
					Serial.println(AnnounceMsg.currentUtcOffset);
					Serial.print("Announce timestamp priority 1:");
					Serial.println(AnnounceMsg.grandmasterPriority1);
					Serial.print("Announce timestamp priority 2:");
					Serial.println(AnnounceMsg.grandmasterPriority2);
					Serial.print("Announce timestamp steps removed:");
					Serial.println(AnnounceMsg.stepsRemoved);
					Serial.print("Announce timestamp time source:");
					Serial.println(AnnounceMsg.TimeSource,HEX);
					Serial.print("Announce timestamp clock class:");
					Serial.println(AnnounceMsg.masterClockQuality.clockClass);
					Serial.print("Announce timestamp clock accuracy:");
					Serial.println(AnnounceMsg.masterClockQuality.clockAccuracy,HEX);
					Serial.print("Announce clock id");
					for(i=0;i<8;i++)
					{
						Serial.print(AnnounceMsg.clockID[i],HEX);
					}
				Serial.println("");
				}*/
				
			}
			
			
			
			/*if (v0)
		{
		Serial.println("It is working!");
		}*/
			
			
			
		
		


	}

	packetSize = _event.parsePacket();
	
	if  (packetSize) {
		// Do something to handle event packets
		//remoteIP=_event.remoteIP();
	    
		/*if (v0)
		{
		Serial.println("It is working");
		}*/
		globalCountevent++;
		//if(globalCountevent%10==0)
		
			memset(buffer,0,sizeof(buffer));
	
			/*if (v3)
			{
				Serial.println("");
			}
			if (v1)
			{
			Serial.println(globalCountevent);
			}*/
			packetByte = _event.read(buffer,44);
			/*if (v1)
			{
			Serial.println(packetByte);
			}*/
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
			//i=0;
           /* for (k=8;k<16;k++)
			{
				PTPMsg.correction[i]=buffer[k];
				i++;
			}*/
			i=0;
			for(k=20;k<28;k++)
			{
				PTPMsg.clockID[i]=buffer[k];
				i++;
			}
			PTPMsg.correction=(buffer[8] << 56) | (buffer[9] << 48) | (buffer[10] << 40) | (buffer[11] << 32) | (buffer[12] << 24) | (buffer[13] << 16) | (buffer[14] << 8) | (buffer[15]);
			//PTPMsg.clockID =(buffer[20] << 56) | (buffer[21] << 48) | (buffer[22] << 40) | (buffer[23] << 32) | (buffer[24] << 24) | (buffer[25] << 16) | (buffer[26] << 8) | (buffer[27]);
			itoa(PTPMsg.correction,PTPMsg.correction1,10);
			/*if (v1)
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
				for(k=0;k<8;k++)
				{
					Serial.print(PTPMsg.clockID[k],HEX);
				}
				Serial.println("");
				//Serial.print("Size of clockid:");
				//Serial.println(sizeof(PTPMsg.clockID));
				Serial.print("Printing correction info event:");
				Serial.println(PTPMsg.correction1);
				for(k=0;k<8;k++)
				{
					Serial.print(PTPMsg.correction[k]);
				}
				Serial.println("");
				//Serial.print("Size of correction:");
				//Serial.println(sizeof(PTPMsg.correction));
				
				
			} */
			if (PTPMsg.control ==0)
			{
			SyncMsg.originTimestamp.seconds=(buffer[34] << 40) | (buffer[35] << 32) | (buffer[36] << 24) | (buffer[37] << 16) | (buffer[38] << 8) | (buffer[39]);
			SyncMsg.originTimestamp.nanoseconds=(buffer[40] << 24) | (buffer[41] << 16) |(buffer[42] << 8) | buffer[43];
			itoa(SyncMsg.originTimestamp.seconds,timestamp,10);
			
			/*if(v2)
			{
				//Serial.print(packetSize);
				Serial.print("Printing sync timestamp (seconds):");
				Serial.println(timestamp);
				//Serial.print("Size of seconds timestamp:");
				//Serial.println(sizeof(SyncMsg.originTimestamp.seconds));
				Serial.print("Printing sync timestamp (nanoseconds)");
				Serial.println(SyncMsg.originTimestamp.nanoseconds);
			}*/
			}
			/*if (v1)
			{
			for (int i=0;i<44;i++)
			{
				
				
				Serial.print(buffer[i],HEX);
				Serial.print(" ");
			}
			}*/
			/*if (v2)
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
			}*/
			
			
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
	packMsg();	


	

}
}

void ArduinoPTP::packMsg() {
	remoteIP=_event.remoteIP();
	Serial.print("Remote IP:");
	Serial.println(remoteIP);
}



