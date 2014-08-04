#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCurvePage �Ի���

class CCurvePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCurvePage)

public:
	CCurvePage();
	virtual ~CCurvePage();

// �Ի�������
	enum { IDD = IDD_CURVEPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	CComboBox combBiaoShi;
	CComboBox combShuJu;
	CComboBox combGuiDao;
	CEdit editZuoBianJie;
	CEdit editYouBianJie;
	CButton btColor;
	CComboBox combKeDu;
	CComboBox combXianXing;
	CComboBox combXianKuan;
	CComboBox combHuiRaoMoShi;
	CEdit combHuiRaoCiShu;
	CSpinButtonCtrl spinEdit;
};
