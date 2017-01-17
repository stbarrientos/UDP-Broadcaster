#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUFFLEN = 512;

int main()
{

	struct sockaddr_in si_me, si_other;
	int sock;
	int port = 4444;
	int recv_len;
	socklen_t slen = sizeof(si_other);
	char buffer[BUFFLEN];

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0){
		std::cout << "Bad Socket" << std::endl;
		return 1;
	}

	memset(&si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(port);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (struct sockaddr*) &si_me, sizeof(si_me)) == -1){
		std::cout << "Bind Error" << std::endl;
		return 1;
	}
	
	std::cout << "Waiting..." << std::endl;
	while(true){
		std::cout << "." << std::flush;
		recv_len = recvfrom(sock, buffer, BUFFLEN, 0, (struct sockaddr *) &si_other, &slen);
		if (recv_len == -1){
			std::cout << "Error receiving" << std::endl;
			return 1;
		}
		std::cout << inet_ntoa(si_other.sin_addr) << " - " << ntohs(si_other.sin_port) << std::endl;
		std::cout << buffer << std::endl;
	}

	close(sock);

	return 0;
}
