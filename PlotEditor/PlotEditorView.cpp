
// PlotEditorView.cpp : CPlotEditorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "PlotEditor.h"
#endif

#include "PlotEditorDoc.h"
#include "PlotEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlotEditorView

IMPLEMENT_DYNCREATE(CPlotEditorView, CView)

BEGIN_MESSAGE_MAP(CPlotEditorView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CPlotEditorView ����/����

CPlotEditorView::CPlotEditorView()
{
	// TODO: �ڴ˴���ӹ������

}

CPlotEditorView::~CPlotEditorView()
{
}

BOOL CPlotEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPlotEditorView ����

void CPlotEditorView::OnDraw(CDC* /*pDC*/)
{
	CPlotEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPlotEditorView ��ӡ

BOOL CPlotEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPlotEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPlotEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CPlotEditorView ���

#ifdef _DEBUG
void CPlotEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CPlotEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlotEditorDoc* CPlotEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlotEditorDoc)));
	return (CPlotEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlotEditorView ��Ϣ�������
