
// PlotEditorView.h : CPlotEditorView 类的接口
//

#pragma once

#include "PlotEditorDoc.h"
class CPlotEditorView : public CView
{
protected: // 仅从序列化创建
	CPlotEditorView();
	DECLARE_DYNCREATE(CPlotEditorView)

// 特性
public:
	CPlotEditorDoc* GetDocument() const;
	void DrawCoordinateSystem(CDC* pDC);
// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CPlotEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

#ifndef _DEBUG  // PlotEditorView.cpp 中的调试版本
inline CPlotEditorDoc* CPlotEditorView::GetDocument() const
   { return reinterpret_cast<CPlotEditorDoc*>(m_pDocument); }
#endif

