#pragma once
#pragma warning(disable : 4995)
#include "afxcmn.h"
#include "jobeditdlgcable.h"
#include "jobeditdlgtool.h"
#include "jobeditdlgcurve.h"
#include "jobeditdlgorigin.h"
#include "afxwin.h"


// CJobEditDlg �Ի���

class CJobEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CJobEditDlg)

public:
	CJobEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CJobEditDlg();

// �Ի�������
	enum { IDD = IDD_JOBEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_jobName;
	bool m_Open;
	CTreeCtrl m_jeTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedJobeditTree(NMHDR *pNMHDR, LRESULT *pResult);
public:	
    CJobEditDlgCable m_Cable;//��о��ʾ�Ի�������Ӧ����
	CJobEditDlgTool m_Tool;//������Ϣ�Ի����Ӧ����
	CJobEditDlgCurve m_Curve;//������Ϣ�Ի����Ӧ����
	CJobEditDlgOrigin m_Origin;//ԭʼ��Ϣ�Ի����Ӧ����
	afx_msg void OnBnClickedOk();
private:
	void CableInit();	
	void CableAdd();
	void CableUpdate();
	void ToolAdd();
	void ToolInit(CString Label);
	void ToolInit();
	void ToolDel(CString Label);
	void ToolUpdate(CString Label);
	void CurveAdd();
	void CurveInit(CString Label);
	void CurveInit();
	void CurveDel(CString Label);
	void CurveUpdate(CString Label);
	void OriginAdd();
	void OriginInit(CString Label);
	void OriginInit();
	void OriginDel(CString Label);
	void OriginUpdate(CString Label);
	void DisplayTree();
	void JobInit();//��ʼ����ҵ���ݿ�

public:
	afx_msg void OnBnClickedAdd();
	int m_editTable;
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedUpdate();
};