
// MonitorDlg.h : ͷ�ļ�
//

#pragma once
#include "jobdlg.h"
#include "jobeditdlg.h"
#include "ChildDraw.h"

extern bool bRunning;
extern CTypedPtrList < CPtrList, SEND_COMMAND_LIST * >senCommandList;

// CMonitorDlg �Ի���
class CMonitorDlg : public CDialogEx
{
public:
	CToolBar m_ToolBar;
	CChildDraw* childDrawWnd;
private:
	
	CWinThread*   m_Thread;
	CTypedPtrList < CPtrList, CDialog * >wndList;//�Ӵ�������б�
// ����
public:
	CMonitorDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMonitorDlg();
	DWORD CreatConnect( );//��������
	DWORD DestroyConnect( );//�Ͽ�����
	void DataReceive(BYTE* inbuff, DWORD* dwSize);
	WORD DataSend(BYTE* outbuff, DWORD dwSize);
	bool ChildWndInitialize();
	bool ShowActiveDlg(CWnd* activeWnd,int id);
// �Ի�������
	enum { IDD = IDD_DATAMONITOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	static UINT MainThread(LPVOID pParam);
	bool StartThread();
	void initCmdList();
	void initWndList();
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMenuAbout();
	afx_msg void OnUpdateMenuAbout(CCmdUI *pCmdUI);
	afx_msg void OnMenuExit();
	afx_msg LRESULT OnCommReceive(WPARAM wParam, LPARAM lParam);//���ն˿���Ϣ
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	afx_msg void OnMenuNew();
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuWork();
	afx_msg void OnMenuInstrument();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
