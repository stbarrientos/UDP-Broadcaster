#include <cstring>
#include "../include/hub.h"
#include "../include/udp_wizard.h"
#include "../include/exceptions.h"
#include "../include/message_structs.h"

Hub::Hub(int port): UdpWizard(port) {}

Hub::~Hub() {}

void Hub::ListenForRequest() {
	using namespace UdpWizardMessageTypes;
	int bufferLen = sizeof(UDPHubRegistrationRequest);
	char buffer[bufferLen];
	UDPHubRegistrationResponse hubResponse;

	while (true){
		try {
			Receive(buffer, bufferLen);
		} catch (ReceiveError& e) {
			continue; // on failure, do nothing and retry
		}
		UDPHubRegistrationRequest* request = reinterpret_cast<UDPHubRegistrationRequest*>(buffer);
		
		// If the request is for a file sender, look up recipients in the map
		// Else we need to add a new recipient entry to the map
		if (request->sending){
			RespondToPeerSendRequest(request, &hubResponse);
		} else {
			RespondToPeerRecvRequest(request, &hubResponse);
		}
		RespondToSender(reinterpret_cast<char*>(&hubResponse), sizeof(hubResponse));
	}
}

void Hub::RespondToPeerSendRequest(UdpWizardMessageTypes::UDPHubRegistrationRequest* request, UdpWizardMessageTypes::UDPHubRegistrationResponse* response)
{
	std::map<char*, hubMapValueStruct>::iterator it = mapOfReceivers.find(request->peerName);

	// If we don't have a receiver with the requested name
	if (it == mapOfReceivers.end()){
		response->accepted = false;
		response->errorCode = 2;

	// If the receiver exists, but has a different sender listed
	} else if (strcmp(it->second.senderName, request->myName) != 0) {
		response->accepted = false;
		response->errorCode = 1;

	// Otherwise, success
	} else {
		response->accepted = true;
		response->errorCode = 0;
		response->peerAddress = mOtherAddress;
	}
}

void Hub::RespondToPeerRecvRequest(UdpWizardMessageTypes::UDPHubRegistrationRequest* request, UdpWizardMessageTypes::UDPHubRegistrationResponse* response)
{
	// hubMapValueStruct mapValue = mapOfReceivers.find(request.peerName);
	std::map<char*, hubMapValueStruct>::iterator it = mapOfReceivers.find(request->myName);

	// create a new entry in the register
	if (it == mapOfReceivers.end()){
		response->accepted = true;
		response->errorCode = 0;
		hubMapValueStruct v;
		// v.senderName = request->peerName;
		memcpy(v.senderName, request->peerName, UdpWizardMessageTypes::MAXIMUM_NAME_LENGTH);
		memcpy(&(v.address), &mOtherAddress, sizeof(mOtherAddress));
		mapOfReceivers.insert(std::make_pair(request->myName, v));

	// otherwise a receiver with that name is already registered
	} else {
		response->accepted = false;
		response->errorCode = 3;
	}
}