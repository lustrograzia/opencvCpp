#include "SerialComm.h"

CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}

int CSerialComm::connect(CString port_name)
{
	if (!serial.OpenPort(port_name)) // ��Ʈ�� �����ϰ� ���¿� �����Ͽ����� fail�� ��ȯ�Ѵ�.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_9600, 8, false, NOPARITY, ONESTOPBIT); // ��Ʈ �⺻���� �����Ѵ�.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); // Timeout�� ����

	return RETURN_SUCCESS;
}

int CSerialComm::sendCommand(CString pos) // �����͸� �����ϴ� �Լ�
{
	if (serial.WriteByte(pos)) // WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
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

void CSerialComm::disconnect() // ��Ʈ�� �� ���� �� �� �ݴ� �Լ�
{
	serial.ClosePort();
}