#include <iostream>
#include "../include/udp_bcaster.h"

int main()
{
	/*
	UdpBcaster ub;
	std::string data = "Hello World";
	std::string ip = "10.0.0.255";
	ub.Send(ip, 4444, data.c_str(), data.size());
	std::cout << "Sent Data To " << ip << ":4444" << std::endl;
	ub.CloseSocket();
	*/

	UdpBcaster ub;
	char buffer[512];
	std::cout << "Listening..." << std::endl;
	ub.Receive(4444, buffer, 512);
	ub.CloseSocket();
	std::cout << "Received: " << buffer << std::endl;

	return 0;
}
