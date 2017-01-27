#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/udp_wizard.h"
#include "../include/exceptions.h"

UdpWizard::UdpWizard() {}
UdpWizard::~UdpWizard() {}

void UdpWizard::Send(std::string destIP, int destPort, const char* sendString, int sendStringLen)
{
	InitSocket();
	BuildSendAddress(destIP, destPort);
	if (sendto(mSocket, sendString, sendStringLen, 0, (struct sockaddr*) &mAddress, sizeof(mAddress)) != sendStringLen){
		throw SendError();
	}
}

void UdpWizard::SendFile(std::string filePath, std::string destIP, int destPort)
{
	// First, prepare a datagram with the header and the first chunk of the file
	// Wait for recipient response before sending the next file
	// Repeat until all of the file has been sent
}

void UdpWizard::Broadcast(std::string destIP, int destPort, const char* sendString, int sendStringLen)
{
	InitSocket();
	SetBroadcastPermission();
	BuildSendAddress(destIP, destPort);
	if (sendto(mSocket, sendString, sendStringLen, 0, (struct sockaddr*) &mAddress, sizeof(mAddress)) != sendStringLen){
		throw SendError();
	}
}


void UdpWizard::Receive(int port, char* buffer, int bufferLen)
{
	struct sockaddr_in senderAddress;
	socklen_t senderLen = sizeof(senderAddress);
	InitSocket();
	BuildReceiveAddress(port);
	BindSocket();
	if (recvfrom(mSocket, buffer, bufferLen, 0, (struct sockaddr*) &senderAddress, &senderLen) == -1){
		throw ReceiveError();
	}
}

void UdpWizard::ReceiveFile(std::string destFilePath, int port)
{
	// Listen for incoming packet
	// Read the header to see how many packets to prepare for, and store the first chunk in the file specified
	// Send message telling that packet has been received and listen for more messages
	// Repeat until full file has been read
}

void UdpWizard::CloseSocket()
{
	close(mSocket);
}

void UdpWizard::BuildSendAddress(std::string destIP, int destPort)
{
	memset(&mAddress, 0, sizeof(mAddress));
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr(destIP.c_str());
	mAddress.sin_port = htons(destPort);
}

void UdpWizard::BuildReceiveAddress(int port)
{
	memset(&mAddress, 0, sizeof(mAddress));
	mAddress.sin_family = AF_INET;
	mAddress.sin_port = htons(port);
	mAddress.sin_addr.s_addr = htonl(INADDR_ANY);
}

void UdpWizard::SetBroadcastPermission()
{
	if (setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (void*) &mBroadcastPermission, sizeof(mBroadcastPermission)) == -1){
		throw SocketPermissionError();
	}
}

void UdpWizard::InitSocket()
{
	mSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == -1) throw SocketInitError();
}

void UdpWizard::BindSocket()
{
	if (bind(mSocket, (struct sockaddr*) &mAddress, sizeof(mAddress)) == -1){
		throw SocketBindError();
	}
}

