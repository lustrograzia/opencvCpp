#pragma once

#include "SerialPort.h"

#define RETURN_SUCCESS 1
#define RETURN_FAIL 0

class CSerialComm
{
public:
	CSerialComm();
	~CSerialComm();

	CSerialPort serial;
	int connect(CString port_name);
	int sendCommand(CString pos);
	int recvCommand(BYTE& serial_data);
	void disconnect();
};