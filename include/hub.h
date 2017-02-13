#ifndef _UDP_HUB_H_
#define _UDP_HUB_H_

#include "udp_wizard.h"
#include "message_structs.h"
#include <map>

class Hub : public UdpWizard {

public:

	Hub(int);
	~Hub();

	virtual void ListenForRequest();


protected:

	virtual void RespondToPeerSendRequest(UdpWizardMessageTypes::UDPHubRegistrationRequest*, UdpWizardMessageTypes::UDPHubRegistrationResponse*);
	virtual void RespondToPeerRecvRequest(UdpWizardMessageTypes::UDPHubRegistrationRequest*, UdpWizardMessageTypes::UDPHubRegistrationResponse*);

	struct hubMapValueStruct {
		sockaddr_in address;
		char senderName[UdpWizardMessageTypes::MAXIMUM_NAME_LENGTH];
	};

	std::map<char*, hubMapValueStruct> mapOfReceivers;

};


#endif