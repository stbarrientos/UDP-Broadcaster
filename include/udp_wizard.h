#ifndef _UDP_WIZARD_H_
#define _UDP_WIZARD_H_

#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <cstdint>

class UdpWizard {

public:
	
	UdpWizard();
	UdpWizard(UdpWizard&);
	UdpWizard& operator=(UdpWizard&);
	~UdpWizard();

	// Data Sender
	void Send(std::string, int, const char*, int);
	void SendFile(std::string filePath, std::string destIP, int destPort);
	void Broadcast(std::string, int, const char*, int);

	// Data Receiver
	void Receive(int, char*, int);
	void ReceiveFile(std::string filePath, int port);

	// Close socket
	void CloseSocket();

	// Headers that will be used to ensure file transfer integrity
	struct UDPFTSendHeader {
		uint16_t totalFileSize = 0;
		uint16_t packetNum = 0;
		uint16_t bytesInPacket;
	};

	struct UDPFTReceiveHeader {
		uint8_t resendPacket = 0; // 0 for false, anything else for true
		uint16_t lastPacketReceived = 0;
	};

protected:

	// Build usable address struct
	void BuildSendAddress(std::string, int);
	void BuildReceiveAddress(int);

	// Set permission for broadcasting
	void SetBroadcastPermission();

	// Initialize Socket
	void InitSocket();
	void BindSocket();

	struct sockaddr_in mAddress;
	int mSocket = 0;
	int mBroadcastPermission = 1;
};

#endif
