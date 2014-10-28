
// DMoniterDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "PublicInterface.h"
#include "PetroData.h"
#include <string>
// CDMoniterDlg �Ի���
class CDMoniterDlg : public CDialogEx
{
// ����
public:
	CDMoniterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DMONITER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	HMENU m_hMenu;//�˵�
	
	CRect rectMain;//����������
	CRect rectView;//��ͼ��������
	CRect rectPanel;//���ݲ�������
	CRect rectScale;//����ͼ������
	CRect rectScrollV;//��ֱ����������
	int minMainDialogWidth, minMainDialogHeight;//��������С���ȡ����
	int m_SM_CXFULLSCREEN,m_SM_CYFULLSCREEN;//��ȡ��Ļ�ֱ���

	int processType;//�����������ͣ�0 - �Ӵ��ڻ�õ����ݣ�1 - ���ļ���ȡ������
	BYTE* pData;//�洢�ļ�����
	CTypedPtrList < CPtrList, CPetroData * >petroList;//���ݴ洢����

	
	CDC   MemDC;          //˫���� ����һ����ʾ�豸����  
	CBitmap   MemBitmap;  //˫���� ����һ��λͼ����  

	//why add ��ͼ����
	unsigned long base;//������
	unsigned long bias;//���ƫ����
	long counter;//������

	POSITION pos;//��ǰ��¼λ��
	CPetroData* pPData;//�õ���������
	CPetroData* pOldPData;//����ǰһ�����ݶ���
	CRect rectTotal;

	int			unitPixel;//ÿ�׶�Ӧ����ֵ
	int			unitRatio;//ÿ�׶�Ӧ����ֵ��ϵ��
	int			m_iterator;//��ͼ������
	CRect		m_screenRect;// m_screenRect - view�Ŀɼ���Χ�����Ļ���Ͻ�����
	CRect		m_clientRect;// m_clientRect - view�Ŀɼ���Χ�ͻ���
	CRect		m_totalRect;// m_totalRect - ����Ҫ��������ķ�Χ���������ɼ�

	CRect		m_plot1Rect;// m_plot1Rect - borderspace1
	CRect		m_plot2Rect;// m_plot2Rect - borderspace2
	CRect		m_plot3Rect;// m_plot3Rect - borderspace3

	SIZE sizeTotal;//����Ҫ��������ķ�Χ���������ɼ�

	double		minDepth;//�ɼ���Χ��С���
	double		maxDepth;//�ɼ���Χ������
	double		maxPreDepth;//�ɼ���Χ�ƶ�֮ǰ��������
	double		minDepthLimit;//��ҵ����С���
	double		maxDepthLimit;//��ҵ��������
	bool		m_bAutoScrollY;		// automatic y range scrolling
	bool		m_bDirectDown;		// ���»���
	bool		bScroll;//���Ƶ��ײ���ʼ����ʱ����Ϊtrue
	int			m_drawCount;//ÿ�λ��ƶ��ٸ�����
	int			m_step;//ÿ���ƶ����룬����Ϊ��λ
	bool		bTimer;
	//��ɫ
	COLORREF	m_gridColor;		// grid line color
	//��������
	CFont		m_font;
	LOGFONT		m_zoomFont;

	//��������ɲ�����������Сֵ
	long tempLimitArray[2];
	long gmLimitArray[2];
	long rmLimitArray[2];
	long cclLimitArray[2];
	long magLimitArray[3][2];

	DATA_TEMP olddeptArray;
	DATA_TEMP oldtempArray;
	DATA_TEMP oldgmArray;
	DATA_TEMP oldrmArray;
	DATA_TEMP oldcclArray;
	DATA_TEMP oldmagArray[3];

public:
	void GetRectParam(CRect rectMain);
	void ClearPetroData();
	void InitArrayData();
	void InitOldArrayData();
	void CDMoniterDlg::ParseData(BYTE* tmp, WPARAM wParam);//������������
	int CheckString( std::string str );
	void DrawData(CDC* pDC);
	
	void DrawBasic(CDC * pDC);//��ͼ������ʼ
	void DrawGrid(CDC * pDC);//���ƺ�����������
	void DrawPlot(CDC* pDC);//������������

	void PrepareDraw();
	void GetMaxMinData();
	unsigned long GetMinData(DATA_PART tmp,unsigned long m);

	void StartTimer();
	void StopTimer();
	void CalculateParam();

	void DrawCurve(CDC* pDC , double upDepth, double DownDepth);//���Ʋ������
	void DrawDeptData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//���Ƹ߶�����
	void DrawTempData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//�����¶�����
	void DrawRmData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//����Rm����
	void DrawMagxData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//���ƴ�����������
	void DrawGmData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//����Gm����
	void DrawCclData(CDC* pDC ,CPetroData* pPData,CPen* pPpen);//����Ccl����
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
	// ��ͼ����ֱ������
	CScrollBar mScrollV;
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdateFileOpen(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
