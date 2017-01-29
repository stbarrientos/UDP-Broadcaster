#ifndef _UDP_BCASTER_EXCEPTIONS_H_
#define _UDP_BCASTER_EXCEPTIONS_H_

#include <exception>
#include <string>

class UdpWizardError : public std::exception {

public:
	
	UdpWizardError(std::string m): message(m) {}
	const char* what() { return message.c_str(); }

protected:
	
	std::string message;

};

class SendError : public UdpWizardError {
public:
	SendError(): UdpWizardError("Error occured while sending datagram") {}
};

class ReceiveError : public UdpWizardError {
public:
	ReceiveError(): UdpWizardError("Error occured while receiving datagram") {}
};

class SocketPermissionError : public UdpWizardError {
public:
	SocketPermissionError(): UdpWizardError("Error occured setting socket broadcast permissions") {}
};

class SocketInitError : public UdpWizardError {
public:
	SocketInitError(): UdpWizardError("Error encountered while initializing socket") {}
};

class SocketBindError : public UdpWizardError {
public:
	SocketBindError(): UdpWizardError("Error encountered while binding socket") {}
};

#endif
