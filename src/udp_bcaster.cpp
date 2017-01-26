#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/udp_bcaster.h"
#include "../include/exceptions.h"

UdpBcaster::UdpBcaster() {}
UdpBcaster::~UdpBcaster() {}

void UdpBcaster::Send(std::string destIP, int destPort, const char* sendString, int sendStringLen)
{
	InitSocket();
	BuildSendAddress(destIP, destPort);
	if (sendto(mSocket, sendString, sendStringLen, 0, (struct sockaddr*) &mAddress, sizeof(mAddress)) != sendStringLen){
		throw SendError();
	}
}

void UdpBcaster::SendFile(std::string destIP, int destPort, std::string filePath)
{

}

void UdpBcaster::Broadcast(std::string destIP, int destPort, const char* sendString, int sendStringLen)
{
	InitSocket();
	SetBroadcastPermission();
	BuildSendAddress(destIP, destPort);
	if (sendto(mSocket, sendString, sendStringLen, 0, (struct sockaddr*) &mAddress, sizeof(mAddress)) != sendStringLen){
		throw SendError();
	}
}

void UdpBcaster::Receive(int port, char* buffer, int bufferLen)
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

void UdpBcaster::ReceiveFile(int port, std::string destFilePath)
{

}

void UdpBcaster::CloseSocket()
{
	close(mSocket);
}

void UdpBcaster::BuildSendAddress(std::string destIP, int destPort)
{
	memset(&mAddress, 0, sizeof(mAddress));
	mAddress.sin_family = AF_INET;
	mAddress.sin_addr.s_addr = inet_addr(destIP.c_str());
	mAddress.sin_port = htons(destPort);
}

void UdpBcaster::BuildReceiveAddress(int port)
{
	memset(&mAddress, 0, sizeof(mAddress));
	mAddress.sin_family = AF_INET;
	mAddress.sin_port = htons(port);
	mAddress.sin_addr.s_addr = htonl(INADDR_ANY);
}

void UdpBcaster::SetBroadcastPermission()
{
	if (setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (void*) &mBroadcastPermission, sizeof(mBroadcastPermission)) == -1){
		throw SocketPermissionError();
	}
}

void UdpBcaster::InitSocket()
{
	mSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == -1) throw SocketInitError();
}

void UdpBcaster::BindSocket()
{
	if (bind(mSocket, (struct sockaddr*) &mAddress, sizeof(mAddress)) == -1){
		throw SocketBindError();
	}
}

