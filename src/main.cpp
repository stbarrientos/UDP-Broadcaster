#include <iostream>
#include <cstring>
#include "../include/udp_bcaster.h"

int main(int argc, char* argv[])
{
	if (argc > 1 && strcmp(argv[1], "send") == 0){
		UdpBcaster ub;
		std::string data = "Hello World";
		// std::string ip = "10.0.0.255";
		std::string ip = "10.1.10.25";
		std::cout << "Sending..." << std::endl;
		ub.Send(ip, 4444, data.c_str(), data.size());
		std::cout << "Sent Data To " << ip << ":4444" << std::endl;
		ub.CloseSocket();
	}

	if (argc > 1 && strcmp(argv[1], "recv") == 0){
		UdpBcaster ub;
		char buffer[512];
		std::cout << "Listening..." << std::endl;
		ub.Receive(4444, buffer, 512);
		ub.CloseSocket();
		std::cout << "Received: " << buffer << std::endl;
	}

	return 0;
}
