#pragma once
#include "GuiObj_FrameSkin.h"
#include "afxtempl.h"
#include "afxmt.h"

#ifndef NX_GUI_EXP
#define NX_GUI_EXP __declspec(dllimport)
#endif


#define Nx_Declare_Dlg_ResHandle(hResHandle) \
	protected: virtual HINSTANCE GetResourceHandle() {return hResHandle;}; \

// CNxDialog dialog

class NX_GUI_EXP CNxDialog : public CDialog , 
							 public CNxFrameSkin
{
	DECLARE_DYNAMIC(CNxDialog)
	Nx_Declare_FrameSkin

protected:
	virtual HINSTANCE GetResourceHandle()  { return NULL; };

public:
	CNxDialog();
	virtual ~CNxDialog();

	explicit CNxDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	explicit CNxDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);

public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);

public:
	// modal processing
	virtual INT_PTR DoModal();

public:
	// 用于DoModal状态时 锁定其他窗口和解除
	static UINT_PTR LockThreadWindows();
		// nLockID 为LockThreadWindows的返回值
	static BOOL UnlockThreadWindows(UINT_PTR nLockID);

	//
	static BOOL IsModalMode(){return ms_bModalMode;};

protected:
	// 根据传入的 Instance handle 创建对话框
	INT_PTR NxDoModal(HINSTANCE hInst = NULL);

protected:
	static BOOL CALLBACK EnumThreadWndProc(HWND hwnd, LPARAM lParam);

protected:
	static CCriticalSection ms_oCritSec;
	static CList<HWND, HWND> ms_lstHwndDisabled;
	static BOOL ms_bModalMode;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
