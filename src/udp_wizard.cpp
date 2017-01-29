#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "../include/udp_wizard.h"
#include "../include/exceptions.h"

UdpWizard::UdpWizard(int p){
	InitSocket();
	BuildSelfAddress(p);
	BindSocket();
}

UdpWizard::~UdpWizard() {}

void UdpWizard::Send(std::string destIP, int destPort, const char* sendString, int sendStringLen)
{
	InitSocket();
	BuildOtherAddress(destIP, destPort);
	if (sendto(mSocket, sendString, sendStringLen, 0, (struct sockaddr*) &mOtherAddress, sizeof(mOtherAddress)) != sendStringLen){
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
	if (!mSocketInitizialized) InitSocket();
	SetBroadcastPermission();
	BuildOtherAddress(destIP, destPort);
	if (sendto(mSocket, sendString, sendStringLen, 0, (struct sockaddr*) &mOtherAddress, sizeof(mOtherAddress)) != sendStringLen){
		throw SendError();
	}
}


void UdpWizard::Receive(char* buffer, int bufferLen)
{
	memset(&mSelfAddress, 0, sizeof(mSelfAddress));
	socklen_t senderLen = sizeof(mOtherAddress);
	if (!mSocketInitizialized) InitSocket();
	if (recvfrom(mSocket, buffer, bufferLen, 0, (struct sockaddr*) &mOtherAddress, &senderLen) == -1){
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

void UdpWizard::RespondToSender(const char* data, int dataLen)
{
	if (sendto(mSocket, data, dataLen, 0, (struct sockaddr*) &mOtherAddress, sizeof(mOtherAddress)) == -1){
		throw SendError();
	}
}

void UdpWizard::CloseSocket()
{
	mSocketBound = false;
	close(mSocket);
}

void UdpWizard::BuildOtherAddress(std::string destIP, int destPort)
{
	memset(&mOtherAddress, 0, sizeof(mOtherAddress));
	mOtherAddress.sin_family = AF_INET;
	mOtherAddress.sin_addr.s_addr = inet_addr(destIP.c_str());
	mOtherAddress.sin_port = htons(destPort);
	mOtherAddressBuilt = true;
}

void UdpWizard::BuildSelfAddress(int port)
{
	memset(&mSelfAddress, 0, sizeof(mSelfAddress));
	mSelfAddress.sin_family = AF_INET;
	mSelfAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	mSelfAddress.sin_port = htons(port);
	mSelfAddressBuilt = true;
}

void UdpWizard::SetBroadcastPermission()
{
	if (setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (void*) &mBroadcastPermission, sizeof(mBroadcastPermission)) == -1){
		throw SocketPermissionError();
	}
}

void UdpWizard::InitSocket()
{
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == -1){
		mSocketInitizialized = false;
		throw SocketInitError();
	}
	mSocketInitizialized = true;
}

void UdpWizard::BindSocket()
{
	if (bind(mSocket, (struct sockaddr*) &mSelfAddress, sizeof(mSelfAddress)) == -1){
		std::cout << "Error: " << errno << std::endl;
		mSocketBound = false;
		throw SocketBindError();
	}
	mSocketBound = true;
}

