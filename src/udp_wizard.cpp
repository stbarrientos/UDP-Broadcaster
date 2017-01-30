#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fstream>
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
	if (!mSocketInitizialized) InitSocket();
	BuildOtherAddress(destIP, destPort);
	if (sendto(mSocket, sendString, sendStringLen, 0, (struct sockaddr*) &mOtherAddress, sizeof(mOtherAddress)) != sendStringLen){
		throw SendError();
	}
}

void UdpWizard::SendFile(std::string filePath, std::string destIP, int destPort)
{
	using namespace std;
	UDPFTSendData sendData;
	char responseBuffer[sizeof(UDPFTResponseData)];

	// Read the first chunk of data from the file.
	ifstream file;
	long fileSize;
	file.open(filePath.c_str(), ios::binary);

	// Get File Size
	file.seekg(0, ios::end);
	fileSize = file.tellg();
	file.seekg(0, ios::beg);

	// Determine how many packets need to be sent
	long totalNumPackets = (fileSize % FT_DATA_PAYLOAD_SIZE == 0) ? fileSize / FT_DATA_PAYLOAD_SIZE : fileSize / FT_DATA_PAYLOAD_SIZE + 1;
	long i = 0;
	while (i < totalNumPackets){

		// Prepare data
		sendData.totalFileSize = fileSize;
		sendData.packetNum = i + 1;
		memset(sendData.payload, 0, FT_DATA_PAYLOAD_SIZE);

		// Read the next chunk into the payload
		file.read(sendData.payload, FT_DATA_PAYLOAD_SIZE);

		// Send data
		Send(destIP, destPort, reinterpret_cast<char*>(&sendData), sizeof(sendData));

		// Wait for recipient response before sending the next file
		try {
			Receive(responseBuffer, sizeof(UDPFTResponseData));
		} catch (SendError& e) {
			cout << "Recipient did not recieve packet, resending.." << endl;
			continue;
		}

		// Read recipient response
		UDPFTResponseData* response = reinterpret_cast<UDPFTResponseData*>(responseBuffer);
		cout << "Recipient received packet #" << response->lastPacketReceived << endl;
		memset(responseBuffer, 0, sizeof(UDPFTResponseData));
		i++;
	}
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
	socklen_t senderLen = sizeof(mOtherAddress);
	if (!mSocketInitizialized) InitSocket();
	if (recvfrom(mSocket, buffer, bufferLen, 0, (struct sockaddr*) &mOtherAddress, &senderLen) == -1){
		throw ReceiveError();
	}
}

void UdpWizard::ReceiveFile(std::string destFilePath)
{
	using namespace std;
	ofstream file;
	file.open(destFilePath.c_str(), ios::binary);
	int bufferLen = sizeof(UDPFTSendData);
	char buffer[bufferLen];
	long totalNumOfPackets = -1;
	long i = 0;
	UDPFTResponseData responseData;
	do {
		Receive(buffer, bufferLen);
		UDPFTSendData* sentData = reinterpret_cast<UDPFTSendData*>(buffer);
		cout << "Received Packet #" << sentData->packetNum << endl;

		// Find totoal num of packets expected if we haven't already
		if (totalNumOfPackets == -1) totalNumOfPackets = (sentData->totalFileSize % FT_DATA_PAYLOAD_SIZE == 0) ? sentData->totalFileSize / FT_DATA_PAYLOAD_SIZE : sentData->totalFileSize / FT_DATA_PAYLOAD_SIZE + 1;

		// Write to file, but make sure not to include garbage included in last packet
		if (i == totalNumOfPackets - 1){
			file.write(sentData->payload, sentData->totalFileSize % FT_DATA_PAYLOAD_SIZE);
		} else {
			file.write(sentData->payload, FT_DATA_PAYLOAD_SIZE);
		}

		// Prepare the response
		responseData.lastPacketReceived = sentData->packetNum;

		// Respond
		RespondToSender(reinterpret_cast<char*>(&responseData), sizeof(responseData));
		i = responseData.lastPacketReceived;
	} while (i < totalNumOfPackets);
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

