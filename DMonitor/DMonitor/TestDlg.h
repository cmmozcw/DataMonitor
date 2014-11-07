#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "tinyxml.h"
#include "ColorStatic.h"
// CTestDlg �Ի���

class CTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestDlg();
	void AddWorkInfo(CWorkInfo* pWorkInfo, int item);
	bool SetCurveInfo(int item);
	void InitCtrl();
	void InitComboBox(int id,CString signal,CString title, CString uint);
	void InitWorkInfoList(CString signal,CString title, CString uint);
// �Ի�������
	enum { IDD = IDD_TEST_DLG };
public:
	CWorkInfo* pCurList;//��¼��ǰ����
	CColorStatic mStaticColor;
	COLORREF curveSelectColor; //��ɫ
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonColor();
	CListCtrl mTestList;
	CComboBox mCbSignal;
	CComboBox nAbbr;
	CComboBox mTestUnit;
	CEdit nTestEditMin;
	CEdit nTestEditMax;
	CButton mTestAdd;
	CButton mTestDel;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
