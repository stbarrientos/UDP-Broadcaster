#ifndef _UDP_BCASTER_H_
#define _UDP_BCASTER_H_

#include <iostream>
#include <string>
#include <arpa/inet.h>

class UdpBcaster {

public:
	
	UdpBcaster();
	UdpBcaster(UdpBcaster&);
	UdpBcaster& operator=(UdpBcaster&);
	~UdpBcaster();

	// Data Sender
	void Send(std::string, int, const char*, int);
	void SendFile(std::string, int, std::string);
	void Broadcast(std::string, int, const char*, int);

	// Data Receiver
	void Receive(int, char*, int);
	void ReceiveFile(int, std::string);

	// Close socket
	void CloseSocket();

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
