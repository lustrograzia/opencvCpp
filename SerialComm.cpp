#include "SerialComm.h"

CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}

int CSerialComm::connect(CString port_name)
{
	if (!serial.OpenPort(port_name)) // 포트를 오픈하고 오픈에 실패하였으면 fail을 반환한다.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_9600, 8, false, NOPARITY, ONESTOPBIT); // 포트 기본값을 설정한다.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); // Timeout값 설정

	return RETURN_SUCCESS;
}

int CSerialComm::sendCommand(CString pos) // 데이터를 전송하는 함수
{
	if (serial.WriteByte(pos)) // WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}

int CSerialComm::recvCommand(BYTE& serial_data)
{
	serial_data = 0;
	if (serial.ReadByte(serial_data))
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}

void CSerialComm::disconnect() // 포트를 다 쓰고 난 뒤 닫는 함수
{
	serial.ClosePort();
}