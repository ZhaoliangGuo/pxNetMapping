//////////////////////////////////////////////////////////////////////
// Customized GUI Button Control
//////////////////////////////////////////////////////////////////////

#pragma once

#include "GuiBase_Wnd.h"

#ifndef NX_GUI_EXP
#define NX_GUI_EXP __declspec(dllimport)
#endif

// The properties to create a button
struct SNxButtonProperties
{
	// Button Type
	enum {
		PushButton = 0,
		PushCheck,
		PushRadio,
		CheckButton,
		RadioButton
	} eButtonType;

	// Button backgnd
	BOOL bDrawButtonBox; // TRUE/FALSE - draw push/check/radio button backgnd
	HBRUSH hBackBrush;   // Backgnd brush
	COLORREF clrBackgnd; // Used when hBackBrush is NULL  

	// TRUE/FALSE the button is a bitmap/text button
	BOOL bBitmapButton;

	// Text button attributes (used when bBitmapButton is FALSE)
	TCHAR szText[48];    // Text string. if empty, the resource ID is used to load.
	UINT nTextIDS;       // Text string resource ID.
	COLORREF clrText[3]; // Text color (up, down, disabled).
	HFONT hFont;         // Font. If NULL, the font name/size are used to create.
	TCHAR szFontName[32];// If to create the font, font name.
	UINT nFontSize;      // If to create the font, font points x 10 (e.g. 125 for 12.5 points.)

	// Bitmap button attributes (used when bBitmapButton is TRUE)
	HBITMAP hBitmap[4];  // Bitmap handles (up, down, disabled). if NULL, load from resource.
	UINT nBitmapIDB[4];  // Bitmap resource IDs if to load the bitmap.
	BOOL bBitmapTransparent;       // If the bitmap has a color to be transparent.
	COLORREF clrBitmapTransparent; // The transparent color in the bitmap.

	// 资源句柄
	HINSTANCE hResInst;
};

// Button class
class NX_GUI_EXP CNxButton : public CWnd
{
	DECLARE_DYNAMIC(CNxButton)

public:
	CNxButton();
	virtual ~CNxButton();

	// Button status (also the index in the color/font/bitmap arraies in the attr)
	enum
	{
		BTN_ST_U = 0,
		BTN_ST_D,
		BTN_ST_X,
		BTN_ST_H
	};

	// Create a button.
	// pParentWnd - Parent window 
	// rect - rect in the parent window 
	// nID - ID of this button
	// dwStyle - WS_CHILD/WS_VISIBLE/WS_TABSTOP/WS_GROUP
	// pProp - properties of this button. the font and bitmap handles in it
	//         (if any) must be valid during the lifetime of this button. 
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, 
		DWORD dwStyle, SNxButtonProperties* pProp);

	// Simulate CButton functions often used
	void SetCheck(int nCheck);
	int GetCheck();
	void SetState(BOOL bHighlight);
	UINT GetState();
	void SetBackColor(COLORREF clr);

	bool DrawInColor();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT pCS);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg LRESULT OnWmSetText(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnWmGetText(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnWmGetTextLength(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBmSetCheck(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBmGetCheck(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBmSetState(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnBmGetState(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

public:
	static void DrawPartOfDDB(CDC& dc, CRect rcDraw, CBitmap* pBitmap, CRect rcSrc);
	static void DrawPushButtonBox(CDC & dc, CRect rcDraw, BOOL bEnable, BOOL bPressed, BOOL bChecked);
	static void DrawCheckButtonBox(CDC& dc, CRect rcDraw, BOOL bEnable, BOOL bPressed, BOOL bChecked, COLORREF clrBk);
	static void DrawRadioButtonBox(CDC& dc, CRect rcDraw, BOOL bEnable, BOOL bPressed, BOOL bChecked, COLORREF clrBk);
    static void DrawCheckButtonBoxEx(CDC& dc, CRect &rcDraw, unsigned long ulFlag, BOOL bEnable, BOOL bPressed, BOOL bChecked, COLORREF clrBk);

protected:
	void DrawButtonBox(CDC& dc, CRect rcDraw);
	void DrawBackgnd(CDC& dc, CRect rcDraw);
	void DrawText(CDC& dc, CRect rcDraw);
	void DrawBitmap(CDC& dc, CRect rcDraw);
	void DoCheck();
	void DrawPushButtonBoxHot(CDC & dc, CRect rcDraw, BOOL bEnable);
	void DrawCheckButtonBoxHot(CDC& dc, CRect rcDraw, BOOL bEnable, BOOL bChecked, COLORREF clrBk);
	void DrawRadioButtonBoxHot(CDC& dc, CRect rcDraw, BOOL bEnable, BOOL bChecked, COLORREF clrBk);

protected:
	SNxButtonProperties m_sProp;
	BOOL m_bDrag;
	UINT m_nHighlight;
	UINT m_nCheck;
	BOOL m_bMultiLine; // 仅用于TextButton
	BOOL m_bHasFocusLine; // 仅用于BitmapButton
	BOOL m_bHot,m_bTracking;

    static Image *m_pImageOver;
	static Image *m_pImageNor;
	static Image *m_pImageDis;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
};

// This class uses the colors and fonts defined in GuiColor.h
class NX_GUI_EXP CNxTextButton : public CNxButton
{
	DECLARE_DYNAMIC(CNxTextButton)

public:
	// 创建一个TextButton，取代pParentWnd中对应ID的控件
	BOOL CreateFromDlgItem(CWnd* pParentWnd, UINT nID, BOOL bCheckBox = FALSE, BOOL bMultiLine = FALSE);
	BOOL Create(CWnd* pParentWnd, UINT nID, BOOL bCheckBox = FALSE, BOOL bMultiLine = FALSE);
	// pszText/nTextIDS - the string or its resource ID.
	// bCheckBox - TRUE/FALSE - push-like checkbutton or normal push button.
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, 
		LPCTSTR pszText, BOOL bCheckBox = FALSE, BOOL bMultiLine = FALSE);
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, 
		UINT nTextIDS, BOOL bCheckBox = FALSE, BOOL bMultiLine = FALSE);

	// Update the text (Obsolete, call SetWindowText())
	void SetText(LPCTSTR pszText);
	void SetText(UINT nTextIDS);

	void AutoWidth(int iMinWidth, BOOL bReDraw);
};

// This class uses the colors and fonts defined in GuiColor.h
class NX_GUI_EXP CNxBitmapButton : public CNxButton
{
	DECLARE_DYNAMIC(CNxBitmapButton)

public:
	// nIDB_U/D/X - the Up/Down/Disabled bitmap resource IDs. 
	//              down and disabled IDB can be 0.
	// The bitmaps are assumed to have pure white color as transparent color.
	// bCheckBox - TRUE/FALSE - push-like checkbutton or normal push button.
	// bButtonBox - if to draw the backgnd button box.
	BOOL CreateFromDlgItem(CWnd* pParentWnd, UINT nID, HINSTANCE hResInst
							, UINT nIDB_U, UINT nIDB_D, UINT nIDB_X
							, BOOL bCheckBox = FALSE, BOOL bButtonBox = FALSE, CToolTipCtrl * pToolTip = NULL);

	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, HINSTANCE hResInst, 
		UINT nIDB_U, UINT nIDB_D, UINT nIDB_X, BOOL bCheckBox = FALSE, BOOL bButtonBox = FALSE);

	//BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, 
	//	UINT nIDB_U, UINT nIDB_D, UINT nIDB_X, 
	//	COLORREF clrTransparent = RGB(192,192,192),
	//	BOOL bCheckBox = FALSE, BOOL bButtonBox = FALSE);

	void HasFocusLine(BOOL bHasFocusLine) {m_bHasFocusLine = bHasFocusLine;};
	BOOL SetBitmap(HINSTANCE hResInst, UINT nIDB_U, UINT nIDB_D, UINT nIDB_X, BOOL bRedraw);
};


class NX_GUI_EXP CNxBitmapButtonEx :  public CNxBitmapButton
{
	DECLARE_DYNAMIC(CNxBitmapButtonEx)
public:
	BOOL CreateFromDlgItem(CWnd* pParentWnd, UINT nID, HINSTANCE hResInst
		, UINT nIDB_U, UINT nIDB_D, UINT nIDB_X , UINT nIDB_H = NULL
		, BOOL bCheckBox = FALSE, BOOL bButtonBox = FALSE, CToolTipCtrl * pToolTip = NULL);
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, HINSTANCE hResInst, 
		UINT nIDB_U, UINT nIDB_D, UINT nIDB_X, UINT nIDB_H = NULL, BOOL bCheckBox = FALSE, BOOL bButtonBox = FALSE);
	BOOL SetBitmap(HINSTANCE hResInst, UINT nIDB_U, UINT nIDB_D, UINT nIDB_X, UINT nIDB_H = NULL, BOOL bRedraw = FALSE) ;
};

class NX_GUI_EXP CNxCheckButton : public CNxButton
{
	DECLARE_DYNAMIC(CNxCheckButton)

public:
	BOOL CreateFromDlgItem(CWnd* pParentWnd, UINT nID);
	// pszText/nTextIDS - the string or its resource ID.
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, LPCTSTR pszText);
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, UINT nTextIDS);
};

class NX_GUI_EXP CNxRadioButton : public CNxButton
{
	DECLARE_DYNAMIC(CNxRadioButton)

public:
	BOOL CreateFromDlgItem(CWnd* pParentWnd, UINT nID);
	// pszText/nTextIDS - the string or its resource ID.
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, LPCTSTR pszText, BOOL bGroup);
	BOOL Create(CWnd* pParentWnd, const CRect& rect, UINT nID, UINT nTextIDS, BOOL bGroup);
};

// eof
