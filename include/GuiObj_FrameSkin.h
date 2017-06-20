#pragma once
#include "MemDC.h"
#include "Object.h"
#include <Gdiplus.h>
using namespace Gdiplus;

//////////////////////////////////////////////////////////////////////////
// 宏
#define Nx_Declare_FrameSkin \
	public: \
	virtual CWnd * ThisWnd()\
	{ \
		ASSERT(IsKindOf(RUNTIME_CLASS(CWnd))); \
		return this; \
	}; \
	void SetWindowText(LPCTSTR lpszString){NxSetWindowText(lpszString);}\


#define Nx_Implement_FrameSkin \

//////////////////////////////////////////////////////////////////////////
// 常量
enum ENxFrameSkinBtnID
{
	keTitleItem_close = 0,
	keTitleItem_maximize,
	keTitleItem_minimize,
	keTitleItem_lock,
	keTitleItem_logo,
	keTitleItem_nothing
};

//////////////////////////////////////////////////////////////////////////
// 类定义
class NX_GUI_EXP CNxFrameSkin
{
public:
	CNxFrameSkin(void);
	virtual ~CNxFrameSkin(void);

public:
	virtual CWnd * ThisWnd() PURE;

public:
	static BOOL InitResource();
	static BOOL CleanResource();

public:
	void SetSkinStyle(UINT nStyle)	{ m_nSkinStyle = nStyle; };
	UINT GetSkinStyle()				{ return m_nSkinStyle; };
	//void SetActive(BOOL bActive)	{ m_bActive = bActive; };
	void SetLock(BOOL bLocked);

public:
	BOOL HasCaption();
	BOOL HasBorder();
	// 参数iBtnIndex　可以是以下值
	// 	keTitleItem_close, keTitleItem_maximize, keTitleItem_minimize,　keTitleItem_lock
	BOOL HasBtn(int iBtnIndex);

	BOOL IsActive();
	BOOL IsLocked();

public:
	BOOL NxDrawBorder(CDC * pDC, CRect rcWindow);
	BOOL NxDrawTitleBar(CDC * pDC, CRect rcTitleBar);
	BOOL NxDrawTitleItem(CDC * pDC, CRect rcTitleBar, UINT nItem, BOOL bUp);
	BOOL NxDrawHotItem(CDC * pDC, CRect rcTitleBar, UINT nItem, BOOL bHot);

	virtual CRect NxGetTitleRect(CRect rcWindow);
	virtual CRect NxGetTitleItemRect(CRect rcTitleBar, UINT nItem);

	virtual void NxOnLock();
public:
	static  HBRUSH	NxCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void	NxSetWindowText(LPCTSTR strText);
	BOOL	NxNcActivate(BOOL bActive);
	CRect	NxCalcWindowRect(LPRECT lpClient);
	void	NxNcCalcSize(BOOL bCalcValidRects, IN OUT CRect & rcFirstRGRC);
	UINT	NxNcHitTest(CPoint pt);
	BOOL	NxNcLButtonDown(UINT nHitTest, CPoint point);
	BOOL	NxNcMouseMove(UINT nHitTest, CPoint point);
	void	NxNcPaint();

	BOOL NxDrawColorTitle(CDC * pDC, CRect rcTitleBar);
protected:
	virtual BOOL NxDrawCutterTitle(CDC * pDC, CRect rcTitleBar);
	static void CalcFrameSkinRectInColor();	

	BOOL NxDrawDirectUITitle(CDC* pDC, CRect rcTitleBar);

	static Image *m_pImageLT;
	static Image *m_pImageMid;
	static Image *m_pImageRT;
	static Image *m_pImageLB;
	static Image *m_pImageRB;

    static Image *m_pImage_Close_nor;
    static Image *m_pImage_Close_down;
    static Image *m_pImage_minimize_nor;
    static Image *m_pImage_minimize_down;

	static Image *m_pImageActiveLT;
	static Image *m_pImageActiveMid;
	static Image *m_pImageActiveRT;
	static Image *m_pImageActiveLB;
	static Image *m_pImageActiveRB;
	
public:
	// 资源 成员
	static int ms_nResourceRef;
	static CNxMemDC ms_dcMem;
	static CBrush ms_oBrush;
	static Image *m_pImgBtnClose;
	static Image *m_pImgBtnMin;
	static Image *m_pImgBtnLock;
	static Image *m_pImgBtnMax;
	static Image *m_pImgBtnRestore;
	static Image *m_pImgBtnUnlock;
public:
	UINT m_nSkinStyle;
	BOOL m_bActive;
	BOOL m_bLocked;
	ENxFrameSkinBtnID m_nHotItem;
	CNxMemDC m_dcMem;
	CRect m_rcTitleBar;
	CString m_strCaption;

	int				m_iBorder;
};


#pragma warning (disable: 4100)

class INxFrameSkinExCB
{
public:
	virtual BOOL ExtendProc(HWND hwnd, WNDPROC OldProc, UINT uMsg,WPARAM wParam,LPARAM lParam, LRESULT & lResult)  {return FALSE;};
	// OldProc 为原窗口的处理函数的指针；
	// lResult 为WNDPROC函数的返回值；
	// 返回值 TRUE : 拦截此消息
	//		  FALSE : 未拦截此消息
	// 以下消息不在拦截之列
	//WM_CTLCOLORDLG
	//WM_CTLCOLOREDIT
	//WM_CTLCOLORLISTBOX
	//WM_CTLCOLORSCROLLBAR
	//WM_CTLCOLORSTATIC
	//WM_CTLCOLORBTN
	//WM_CTLCOLOR
	//WM_NCACTIVATE
	//WM_NCCALCSIZE
	//WM_NCHITTEST
	//WM_NCLBUTTONDOWN
	//WM_NCPAINT
	//WM_ERASEBKGND
	//WM_DESTROY
};
#pragma warning (default: 4100)


class NX_GUI_EXP CNxFrameSkinEx : public CNxFrameSkin
{
public:
	CNxFrameSkinEx(HWND hWnd, INxFrameSkinExCB * pExtendCB = NULL);
	virtual ~CNxFrameSkinEx(void);

	static CNxFrameSkinEx * HookWindow(HWND hWnd, INxFrameSkinExCB * pExtendCB = NULL, UINT dwAppendStyle = 0, UINT dwAppendExStyle = 0);

public:
	virtual CWnd * ThisWnd();

public:
	HWND m_hHookWnd;
	WNDPROC m_wpOld; 
	INxFrameSkinExCB * m_pIExendCallBack;
};
