#ifndef _UDP_BCASTER_EXCEPTIONS_H_
#define _UDP_BCASTER_EXCEPTIONS_H_

#include <exception>
#include <string>

class UdpBcasterError : public std::exception {

public:
	
	UdpBcasterError(std::string m): message(m) {}
	const char * what() { return message.c_str(); }

protected:
	
	std::string message;

};

class SendError : public UdpBcasterError {
public:
	SendError(): UdpBcasterError("Error occured while sending datagram") {}
};

class ReceiveError : public UdpBcasterError {
public:
	ReceiveError(): UdpBcasterError("Error occured while receiving datagram") {}
};

class SocketPermissionError : public UdpBcasterError {
public:
	SocketPermissionError(): UdpBcasterError("Error occured setting socket broadcast permissions") {}
};

class SocketInitError : public UdpBcasterError {
public:
	SocketInitError(): UdpBcasterError("Error encountered while initializing socket") {}
};

class SocketBindError : public UdpBcasterError {
public:
	SocketBindError(): UdpBcasterError("Error encountered while binding socket") {}
};

#endif
