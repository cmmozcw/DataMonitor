#pragma once



// CPanelView ������ͼ

class CPanelView : public CFormView
{
	DECLARE_DYNCREATE(CPanelView)

protected:
	CPanelView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPanelView();

public:
	enum { IDD = IDD_PANEL_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


