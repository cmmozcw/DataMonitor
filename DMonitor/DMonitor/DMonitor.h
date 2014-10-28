
// DMonitor.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "CommLayer.h"

// CDMoniterApp:
// 有关此类的实现，请参阅 DMonitor.cpp
//

class CDMoniterApp : public CWinApp
{
public:
	CDMoniterApp();
	CString GetResString(UINT uStringID);
	char *FromUNICODEToANSI(CString str);
	short FromUnicodeToUTF8 (LPSTR utf8Str, short utf8StrLen, WORD * unStr, unsigned short unMaxLen);
	void GetModulePath();
	//int CreateDmFile(CString strFile);
	int CreateIniFile(CString strFile);

	//void LinkElementeFuns(TiXmlElement * element, CString str);

	CCommLayer commLayer;//串口通信管理
	HINSTANCE exe_hInstance;
	HANDLE hSem;//判断软件市立是否已存在

	// MFC临界区类对象
	CCriticalSection g_cs;

	CString strSemaphore;
    CString strSemaphoreExist;
    CString strSemaphoreCreatFail;
	CString strUserSetCommPort;//用户选择的串口号
	BYTE m_DeviceType;//设备类型
	int m_CommResault;//记录串口连接结果
//存储路径信息
	CString szExePath;//可执行文件所在位置绝对路径
	CString ModuleFilePath;
	CString TempPath;//临时文件夹绝对路径
	CString LogPath;//log日志所在位置绝对路径
	CString IniFilePath;//ini文件夹绝对路径
	CString strIniFilePath;//ini文件所在位置绝对路径
	CString DataPath;//临时文件夹绝对路径
	CString JobPath;//作业文件夹的绝对路径
	CString ToolPath;//仪器文件夹绝对路径
	CString strUnitsFile;//记录度量单位的配置文件
	CString strCurveFile;//记录绘图模板的配置文件
	CString strSettingFile;//记录用户设置的常用数值配置文件
	//发送给串口设备的数据buffer
	bool bCurveFileExist;
	UINT8 sendCmd[COMM_BUFFER_BASESIZE];//通信层发送数据 buffer
	WORD sendDataSize;
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// 实现

	DECLARE_MESSAGE_MAP()
};

extern CDMoniterApp theApp;