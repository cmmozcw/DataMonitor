#pragma once


// CJobEditDlgTool �Ի���

class CJobEditDlgTool : public CDialog
{
	DECLARE_DYNAMIC(CJobEditDlgTool)

public:
	CJobEditDlgTool(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CJobEditDlgTool();

// �Ի�������
	enum { IDD = IDD_JOBEDIT_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
