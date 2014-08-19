// CurvePage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlotEditor.h"
#include "CurvePage.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CCurvePage �Ի���

IMPLEMENT_DYNAMIC(CCurvePage, CPropertyPage)

CCurvePage::CCurvePage()
	: CPropertyPage(CCurvePage::IDD)
{
	curveSelectColor = RGB(0xFF,0x00,0x00);
}

CCurvePage::~CCurvePage()
{
}

void CCurvePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BIAOSHI, combBiaoShi);
	DDX_Control(pDX, IDC_COMBO_SHUJU, combShuJu);
	DDX_Control(pDX, IDC_COMBO_GUIDAO, combGuiDao);
	DDX_Control(pDX, IDC_EDIT_ZUOBIANJIE, editZuoBianJie);
	DDX_Control(pDX, IDC_EDIT_YOUBIANJIE, editYouBianJie);
	DDX_Control(pDX, IDC_BUTTON_COLOR, btColor);
	DDX_Control(pDX, IDC_COMBO_KEDU, combKeDu);
	DDX_Control(pDX, IDC_COMBO_XIANXING, combXianXing);
	DDX_Control(pDX, IDC_COMBO_XIANKUAN, combXianKuan);
	DDX_Control(pDX, IDC_COMBO_HUIRAOMOSHI, combHuiRaoMoShi);
	DDX_Control(pDX, IDC_EDIT_HUIRAOCISHU, combHuiRaoCiShu);
	DDX_Control(pDX, IDC_SPIN, spinEdit);
	DDX_Control(pDX, IDC_STATIC_COLOR, mStaticColor);
}


BEGIN_MESSAGE_MAP(CCurvePage, CPropertyPage)
	ON_BN_CLICKED(IDOK, &CCurvePage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCurvePage::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCurvePage::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CCurvePage::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CCurvePage ��Ϣ�������


BOOL CCurvePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CWnd::SetWindowText(theApp.GetResString(IDS_CURVE_TAB1));

	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(0,_T("Track 1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(1,_T("Depth Track"));
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->InsertString(2,_T("Track 23"));

	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->InsertString(0,_T("����"));
	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->InsertString(1,_T("����"));

	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(0,_T("ʵ��"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(1,_T("����1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(2,_T("����2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(3,_T("����3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(4,_T("����4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->InsertString(5,_T("����5"));
	
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(0,_T("0"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(1,_T("0.5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(2,_T("1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(3,_T("1.5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(4,_T("2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->InsertString(5,_T("2.5"));

	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(0,_T("�����"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(1,_T("�һ���"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(2,_T("���Ҷ�����"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(3,_T("������"));
	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->InsertString(4,_T("No Wrap"));

	if(theApp.curveList.IsEmpty())
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->SetCurSel(0);
		editZuoBianJie.SetWindowText(_T("1"));
		editYouBianJie.SetWindowText(_T("100"));
		mStaticColor.SetStaiticColor(curveSelectColor);
		combHuiRaoCiShu.SetWindowText(_T("1"));
	}
	else
	{
		//����theApp.curveList
		int i = 0;
		POSITION pos3 = theApp.curveList.GetHeadPosition();
		while(pos3)
		{
			CCurveInfo* plist = theApp.curveList.GetNext(pos3);
			AddCurveInfo(plist,i);
			i++;
		}
		if(!SetCurveInfo(0))
		{
			SendMessage(WM_CLOSE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CCurvePage::AddCurveInfo(CCurveInfo* pCurveInfo, int item)
{
	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->InsertString(item,pCurveInfo->strCurveName);
	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->InsertString(item,pCurveInfo->strCurveName);
}

bool CCurvePage::SetCurveInfo(int item)
{
	if(item >= theApp.curveList.GetCount() || item <0)
	{
		return false;
	}
	CCurveInfo* plist = theApp.curveList.GetAt(theApp.curveList.FindIndex(item));
	CString str;

	((CComboBox*)GetDlgItem(IDC_COMBO_BIAOSHI))->SetCurSel(plist->curveName);

	((CComboBox*)GetDlgItem(IDC_COMBO_SHUJU))->SetCurSel(plist->curveName);
	
	((CComboBox*)GetDlgItem(IDC_COMBO_GUIDAO))->SetCurSel(plist->curveTrack);

	str.Format(_T("%d"),plist->leftLimit);
	editZuoBianJie.SetWindowText(str);

	str.Format(_T("%d"),plist->rightLimit);
	editYouBianJie.SetWindowText(str);

	mStaticColor.SetStaiticColor(plist->curveColor);
	curveSelectColor = plist->curveColor;

	((CComboBox*)GetDlgItem(IDC_COMBO_KEDU))->SetCurSel(plist->graduation);

	((CComboBox*)GetDlgItem(IDC_COMBO_XIANXING))->SetCurSel(plist->lineType);
	
	str.Format(_T("%d"),plist->lineWidth);
	((CComboBox*)GetDlgItem(IDC_COMBO_XIANKUAN))->SetCurSel(plist->lineWidth);

	((CComboBox*)GetDlgItem(IDC_COMBO_HUIRAOMOSHI))->SetCurSel(plist->wrapMode);

	str.Format(_T("%d"),plist->wrapCount);
	combHuiRaoCiShu.SetWindowText(str);
	
	return true;
}

void CCurvePage::InseertCurveInfo( )
{
	CString str;
	POSITION pos,pos2;
	bool bLast = true;
	CString strTemp;
	combShuJu.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
	{
		MessageBox(_T("��������������"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
		return;
	}
	if(theApp.curveList.IsEmpty())
	{
	}
	else
	{
		int i = 0;
		pos = theApp.curveList.GetHeadPosition();
		while(pos)
		{
			pos2 = pos;
			CCurveInfo* plist = theApp.curveList.GetNext(pos);
			if(plist->strCurveName.Compare(strTemp) == 0)
			{
				MessageBox(_T("�����Ѵ��ڣ����������룡"),_T("��ʾ"),MB_ICONQUESTION|MB_OK);
				return;
			}
			else if(plist->strCurveName.Compare(strTemp) > 0)
			{
				bLast = false;
				break;
			}
			else
			{
				continue;
			}
			i++;
		}
		CCurveInfo* pCurveInfo = new CCurveInfo();
		pCurveInfo->strCurveName = strTemp;
		pCurveInfo->curveTrack = combGuiDao.GetCurSel();

		editZuoBianJie.GetWindowText(str);
		pCurveInfo->leftLimit = _wtoi(str);
		editYouBianJie.GetWindowText(str);
		pCurveInfo->rightLimit = _wtoi(str);

		pCurveInfo->curveColor = curveSelectColor;
		pCurveInfo->graduation = combKeDu.GetCurSel();
		pCurveInfo->lineType = combXianXing.GetCurSel();
		pCurveInfo->lineWidth = combXianKuan.GetCurSel();
		pCurveInfo->wrapMode = combHuiRaoMoShi.GetCurSel();

		combHuiRaoCiShu.GetWindowText(str);
		pCurveInfo->wrapCount = _wtoi(str);

		if(bLast)
		{
			theApp.curveList.AddTail(pCurveInfo);
		}
		else
		{
			theApp.curveList.InsertBefore(pos2,pCurveInfo);
		}
	}
}

void CCurvePage::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CCurvePage::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CCurvePage::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	InseertCurveInfo();
	
}


void CCurvePage::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
