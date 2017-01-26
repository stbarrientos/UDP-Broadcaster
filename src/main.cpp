#include <iostream>
#include <cstring>
#include <cstdlib>
#include "../include/udp_bcaster.h"

void validateSendArgs(int argc, char** argv);
void validateRecvArgs(int argc, char** argv);
void printUsage();

int main(int argc, char* argv[])
{
	if (argc > 1 && strcmp(argv[1], "send") == 0){
		validateSendArgs(argc,argv);
		UdpBcaster ub;
		std::string data = "Hello World";
		std::string ip = argv[2];
		int port = std::stoi(argv[3]);
		std::cout << "Sending To " << ip << ":" << port << std::endl;
		ub.Send(ip, port, data.c_str(), data.size());
		std::cout << "Sent" << std::endl;
		ub.CloseSocket();
	} else if (argc > 1 && strcmp(argv[1], "recv") == 0){
		validateRecvArgs(argc, argv);
		UdpBcaster ub;
		char buffer[512];
		memset(&buffer, 0, 512);
		int port = std::stoi(argv[2]);
		std::cout << "Listening On Port " << port << std::endl;
		ub.Receive(port, buffer, 512);
		ub.CloseSocket();
		std::cout << "Received: " << buffer << std::endl;
	} else {
		printUsage();
		return 1;
	}

	return 0;
}

void validateSendArgs(int argc, char** argv)
{
	if (argc != 4){
		printUsage();
		exit(1);
	}
}

void validateRecvArgs(int argc, char** argv)
{
	if (argc != 3){
		printUsage();
		exit(1);
	}
}

void printUsage()
{
	using namespace std;
	cout << "Usage:" << endl;
	cout << "udp_bcaster send <destination ip> <destination port>" << endl;
	cout << "udp_bcaster recv <incoming port>" << endl;
	cout << "Examples:" << endl;
	cout << "udp_bcaster send 10.0.0.5 4444 // send datagram to 10.0.0.5:4444" << endl;
	cout << "udp_bcaster recv 4444 // listen for incoming datagram on port 4444" << endl;
}
