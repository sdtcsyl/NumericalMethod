#pragma once
#include <afxwin.h>

//interface of the CCode3AView class

#define N 3

class CCode3AView : public CView
{
protected:	// create from serialization only
	CCode3AView();
	DECLARE_DYNCREATE(CCode3AVIEW);

//Attributes
public:
	CCode3ADoc * GetDocument() const;

//Operations
public:

	//overrides
public:
	virtual void OnDraw(CDC* pDC); //overrideen to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo * pInfo);

	//implementation
public:
	virtual ~CCode3AView();

#ifdef DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//Generated message map functions
protected:
	DECLARE_MESSAGE_MAP();
public:
	void InputData(double ** a, double ** b);
	void MatMultiply(double ** c, double **a, double **b);
	void MatInverse(double **x, double **a);
};

#ifdef DEBUG	// debug version in Code3AView.cpp
inline CCode3ADoc* CCode3AView::GetDocument() const {
	return reinterpret_cast<CCode3ADoc*>(m_pDocument);
}
#endif // DEBUG


