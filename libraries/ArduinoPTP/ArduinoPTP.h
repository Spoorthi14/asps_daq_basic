/*
 *
 * PTP daemon class.
 * Note that to operate correctly, the microcontroller needs to receive
 * PTP packets, which requires joining IGMP group 224.0.1.129.
 */
 
 #include <EthernetUdpTimestamped.h>
 #include "lwip/pbuf.h"

 
class ArduinoPTP {
public:
    ArduinoPTP() {}
	uint8_t begin();
	void stop();
	void handle();
	void checkState();
//	typedef enum state{} ptpState_t;
  //  void changeState(state new_state);
	
	
private:
   // EthernetUDP _general_packet;
 //   EthernetUDP _event_packet;
	EthernetUDPTimestamped _general;
	EthernetUDPTimestamped _event;
};
