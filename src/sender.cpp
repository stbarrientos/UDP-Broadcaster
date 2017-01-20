#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{

	int sock; // Socket
	struct sockaddr_in broadcastAddr; // Broadcast Address
	const char* broadcastIP = "10.0.0.255" ; // Broadcast IP
	const unsigned short broadcastPort = 4444; // Broadcast Port
	const char* sendString = "Hello World"; // String to broadcast
	int broadcastPermission; // Socket option to set permission to broadcast
	int sendStringLength; // Length of send string

	// Set Up Vars
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0){
		std::cout << "Sock < 0. Fatal" << std::endl;
		return 1;
	}

	// Set Sock Permission to allow Broadcast
	broadcastPermission = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, sizeof(broadcastPermission)) < 0){
		std::cout << "Socket Permission Failed" << std::endl;
		return 1;
	}

	// Build address struct
	memset(&broadcastAddr, 0, sizeof(broadcastAddr));
	broadcastAddr.sin_family = AF_INET;
	broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);
	broadcastAddr.sin_port = htons(broadcastPort);

	sendStringLength = strlen(sendString);

	if (sendto(sock, sendString, sendStringLength, 0, (struct sockaddr *) &broadcastAddr, sizeof(broadcastAddr)) != sendStringLength){
		std::cout << "Sent A Different Number of Bytes Than Expected" << std::endl;
		return 1;
	}

	std::cout << "Done" << std::endl;

	return 0;
}
