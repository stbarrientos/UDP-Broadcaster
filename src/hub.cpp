#include "../include/hub.h"
#include "../include/udp_wizard.h"

Hub::Hub() {}

Hub::~Hub() {}

void Hub::ListenForRequest() {
	int bufferLen = sizeof(UDPHubForwardRequestData);
	char buffer[bufferLen];

	// Reset mClients array so we start fresh
	memset(&mClients, 0, sizeof(mClients));
	mClientsReadyForSwap = false;

	// Listen for requests until we have enough addresses to swap
	int requests = 0;
	while (requests <= CLIENTS_REQUIRED_FOR_SWAP){
		try {
			Receive(&buffer, bufferlen);
		} catch (ReceiveError& e) {
			continue // on failure, do nothing and retry
		}
		memcpy(mClientAddresses[requests], &mOtherAddress, sizeof(mOtherAddress));
		requests++;
	}

	RespondWithOtherAddress();
}

void Hub::RespondWithOtherAddress () {
	//Send(sockaddr_in* address, const char* data, int dataLen)
	UDPHubForwardResponseData data1, data2;
	data1.address = mClientAddresses[0];
	data2.address = mClientAddresses[1];

	Send(&data1, (const char *) &data2, sizeof(data2));
	Send(&data2, (const char *) &data1, sizeof(data1));
}