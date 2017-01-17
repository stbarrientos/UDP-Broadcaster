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
	char* broadcastIP; // Broadcast IP
	unsigned short broadcastPort; // Broadcast Port
	char* sendString; // String to broadcast
	int broadcastPermission; // Socket option to set permission to broadcast
	unsigned int sendStringLength; // Length of send string

	// Set Up Vars
	broadcastIP = "127.0.0.1";
	broadcastPort = 4444;
	sendString = "Hello World";
	sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0){
		std::cout << "Sock < 0. Fatal" << std::endl;
		return 1;
	}

	broadcastPermission = 1;
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
