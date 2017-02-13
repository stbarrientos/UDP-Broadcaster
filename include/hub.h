#ifndef _UDP_HUB_H_
#define _UDP_HUB_H_

#include "udp_wizard.h"

class Hub : public UdpWizard {

public:

	Hub();
	~Hub();

	void ListenForRequest();
	void RespondWithOtherAddress();


protected:

	static const int CLIENTS_REQUIRED_FOR_SWAP = 2;
	sockaddr_in mClientAddresses[CLIENTS_REQUIRED_FOR_SWAP];
	bool mClientsReadyForSwap = false;

};


#endif