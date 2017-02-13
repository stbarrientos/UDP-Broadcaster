#ifndef _MESSAGE_STRUCTS_H_
#define _MESSAGE_STRUCTS_H_

#include <cstdint>

namespace UdpWizardMessageTypes {

	const static int FT_DATA_PAYLOAD_SIZE = 512;
	static const int MAXIMUM_NAME_LENGTH = 20;

	// Packet format data that will be used to ensure file transfer integrity
	#pragma pack(1)
	struct UDPFTSendData {
		uint32_t totalFileSize = 0;
		uint16_t packetNum = 0;
		char payload[FT_DATA_PAYLOAD_SIZE];
	};

	// Packet format data that will be used to respond to file senders
	#pragma pack(1)
	struct UDPFTResponseData {
		uint8_t resendPacket = 0; // 0 for false, anything else for true
		uint16_t lastPacketReceived = 0;
	};

	// Packet format data with Hub requests for establishing connections with peers
	#pragma pack(1)
	struct UDPHubRegistrationRequest {
		char myName[MAXIMUM_NAME_LENGTH] = {0};
		char peerName[MAXIMUM_NAME_LENGTH] = {0};
		bool sending = true;
	};

	#pragma pack(1)
	struct UDPHubRegistrationResponse {
		bool accepted = true;
		int errorCode = 0;
		sockaddr_in peerAddress = {0};
	};

	static const char* const HUB_RROR_CODES[] = {
		"Success",
		"myName not listed as valid sender to peerName",
		"No User Found With peerName",
		"Receiver name already taken",
		"Misc Connection Error"
	};

}


#endif