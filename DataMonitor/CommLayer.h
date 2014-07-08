
#pragma once

#include "SerialPort.h"

#define WM_USER_RECEIVEDATA (WM_USER+7)
#define WM_CONNECT_ERR      (WM_USER+8)
#define WM_CONNECT_OK      (WM_USER+9)
const int array_size =100;
const BYTE diag_maincmd =0x16;

typedef struct
{
    DWORD  SubKeyIndex;
    TCHAR   SubKeyName[80];
    DWORD  SubKeyNameLength;
    TCHAR   SubKeyValue[80];
    DWORD  SubKeyValueLength;
}SubKeyInfo_type;

typedef struct _DIAG_PIM_HEADCMD
{
    BYTE maincmd;
    BYTE subcmd;
    WORD childcmd;
    WORD state;
}DIAG_PIM_HEADCMD;

typedef enum
{
    UNSIGNALED,
    SIGNALED,
}Event_type;

typedef enum
{
    TYPE_NONE,
    TYPE_PIM,
    TYPE_DLL,
    TYPE_COMM
}Calling_type;
// CCommLayer ����Ŀ��

class CCommLayer : public CCmdTarget
{
	DECLARE_DYNAMIC(CCommLayer)
public:
	HWND fatherHwnd;  //����dll��Ӧ�ó��򴰿�ָ�롣
	CSerialPort m_SerialPort;
    SubKeyInfo_type SubKey[array_size];//sub keys
    DWORD m_PhoneUsedNum;//�����Ѵ��¼����
    DWORD m_UimUsedNum;//UIM���Ѵ��¼����
    DWORD m_SimUsedNum;//SIM���Ѵ��¼����
    DWORD m_TDUsedNum;//TD����,�Ѵ��¼����
    int m_FuncReturnValue;
    DWORD comnumber;//��Ч��COM������
    DWORD num;//��Ч��COM������
    WORD m_RetryTimes;
    BYTE m_WriteBuffer[4*1024];//send buffer
    WORD m_WriteBufferSize;
    BYTE m_ReceiveBuff[4*1024];//receive buffer
    WORD m_ReceiveBufferSize;
    BOOL ReceivewholePacket;     //���յ�������packet
    BOOL m_bTopFlagByte;             //��һ��0x7E��־
    BOOL m_bEscapeByte;              //��������0x7E\0x7D���û���־
    BOOL m_bComPortEffective; //�˿���Ч
    BOOL m_bConnectEffective; //������Ч,�Ͽ����Ӻ��ֵΪFALSE
    BOOL bTaskCanceled;//ִ��ȡ�������ӿ�
    WORD m_wlocalReceivePtr;         //����bytes����λ��
    BYTE m_Mingroup;//�����С���
	BYTE m_Maxgroup;//���������

    DIAG_PIM_HEADCMD m_req;

    CEvent g_event;
    CEvent g_eventcom;
    Event_type m_EventStatus;
    Calling_type m_CallType;

	BOOL m_bISAddField;  
    CString m_SFamilyname;
    CString m_SGivenname;
	CString m_SCountry;
	CString m_SProvince;
	CString m_SCity;
	CString m_SStreet;
	CString m_SPostbox;
	CString m_SPostcode;
	CString m_SLabel;
	CString m_SExtension;
	CString m_SRegion;
public:
	CCommLayer();
	virtual ~CCommLayer();

	wchar_t * FromGBToUNICODE(char *pData,int *nDataLen,int *pwDataLen);
    char * FromUNICODEToGB(wchar_t *pData, int *nwDataLen, int *pDataLen);
    int GetRegisterdComPort(SubKeyInfo_type* SubKey, DWORD* number);
    int SelectComPort(void);
    int SendVerification(void);
    WORD TransData(BYTE* outbuff, DWORD dwSize);
    WORD MakePacket(BYTE* pDest, BYTE* pSource, WORD wSize);
    WORD CalcCRC(BYTE* buf_ptr, WORD len);
	void RecvDataForPCSuite(BYTE* inbuff, DWORD* dwSize);
    WORD RecvData(BYTE* inbuff, WORD dwSize);
    BOOL DecodeReceivePacket(BYTE* pDest, WORD *pwSize2, BYTE *pSource, WORD *pwSize);
    BOOL CheckReceivePacketStatus(BYTE *pbBuff, WORD *wSize);
    WORD RecvDataAnalysis(BYTE* m_ReceiveBuff, WORD m_ReceiveBufferSize);
    WORD ChangeCStringToWORD (CString strChange);
	DWORD ChangeCStringToDWORD (CString strChange);

protected:
	DECLARE_MESSAGE_MAP()
};


