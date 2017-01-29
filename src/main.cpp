#include <iostream>
#include <cstring>
#include <cstdlib>
#include "../include/udp_wizard.h"

void validateSendArgs(int argc, char** argv);
void validateRecvArgs(int argc, char** argv);
void printUsage();

int main(int argc, char* argv[])
{
	if (argc > 1 && strcmp(argv[1], "send") == 0){
		validateSendArgs(argc,argv);
		UdpWizard ub(5555);
		char buffer[512];
		memset(&buffer, 0, 512);
		std::string data = "Ping";
		std::string ip = argv[2];
		int port = std::stoi(argv[3]);

		// Send ping
		std::cout << "Sending To " << ip << ":" << ntohs(port) << std::endl;
		ub.Send(ip, port, data.c_str(), data.size());

		// Listen for pong
		std::cout << "Listening On Port " << ub.GetSelfPort() << std::endl;
		ub.Receive(buffer, 512);
		std::cout << "Sent From " << ub.GetOtherIP() << ":" << ub.GetOtherPort() << std::endl;
		std::cout << "Received: " << buffer << std::endl;

		// Close
		ub.CloseSocket();

	} else if (argc > 1 && strcmp(argv[1], "recv") == 0){
		validateRecvArgs(argc, argv);
		UdpWizard ub(std::stoi(argv[2]));
		char buffer[512];
		std::string data = "Pong";
		memset(&buffer, 0, 512);

		// Listen for ping
		std::cout << "Listening On Port " << ub.GetSelfPort() << std::endl;
		ub.Receive(buffer, 512);
		std::cout << "Received: " << buffer << ". Sent Using Socket " << ub.GetSocket() << std::endl;

		// Send pong
		std::string ip = ub.GetOtherIP();
		int port = ub.GetOtherPort();
		std::cout << "Sending To " << ip << ":" << ntohs(port) << std::endl;
		ub.RespondToSender(data.c_str(), data.size());

		// Close
		ub.CloseSocket();
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
