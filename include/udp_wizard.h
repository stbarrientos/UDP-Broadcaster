#ifndef _UDP_WIZARD_H_
#define _UDP_WIZARD_H_

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <cstdint>

class UdpWizard {

public:
	
	UdpWizard(int);
	UdpWizard(UdpWizard&);
	UdpWizard& operator=(UdpWizard&);
	~UdpWizard();

	// Getters
	int GetSocket() { return mSocket; }

	// Data Sender
	void Send(std::string, int, const char*, int);
	void SendFile(std::string filePath, std::string destIP, int destPort);
	void Broadcast(std::string, int, const char*, int);
	const char* const GetSelfIP() const { return  inet_ntoa(mSelfAddress.sin_addr); }
	int GetSelfPort() { return mSelfAddress.sin_port; }

	// Data Receiver
	void Receive(char*, int);
	void ReceiveFile(std::string filePath);
	const char* const GetOtherIP() const { return inet_ntoa(mOtherAddress.sin_addr); }
	int GetOtherPort() { return mOtherAddress.sin_port; }
	void RespondToSender(const char* data, int dataLen);

	// Close socket
	void CloseSocket();

protected:

	// Build usable address struct
	void BuildSelfAddress(int);
	void BuildOtherAddress(std::string, int);

	// Set permission for broadcasting
	void SetBroadcastPermission();

	// Initialize Socket
	void InitSocket();
	bool SetSocketSendTimeout(struct timeval);
	bool SetSocketReceiveTimeout(struct timeval);
	void BindSocket();

	const static int FT_DATA_PAYLOAD_SIZE = 512;

	// Packet format data that will be used to ensure file transfer integrity
	#pragma pack(1)
	struct UDPFTSendData {
		uint32_t totalFileSize = 0;
		uint16_t packetNum = 0;
		char payload[FT_DATA_PAYLOAD_SIZE];
	};

	#pragma pack(1)
	struct UDPFTResponseData {
		uint8_t resendPacket = 0; // 0 for false, anything else for true
		uint16_t lastPacketReceived = 0;
	};

	// Members
	struct sockaddr_in mSelfAddress;
	struct sockaddr_in mOtherAddress;
	int mSocket = 0;
	bool mSocketBound = false;
	bool mSocketInitizialized = false;
	bool mSelfAddressBuilt = false;
	bool mOtherAddressBuilt = false;
	int mBroadcastPermission = 1;
};

#endif
