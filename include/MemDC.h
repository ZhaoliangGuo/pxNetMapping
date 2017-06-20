#pragma once

#ifndef NX_GUI_EXP
#define NX_GUI_EXP __declspec(dllimport)
#endif

//#include "afxwin.h"
typedef struct SNxBlt_Factor
{
	int iFatorH[3];
	int iFatorV[3];
}SNxBlt_Factor, * PSNxBlt_Factor;

class NX_GUI_EXP CNxMemDC : public CDC
{
public:
	CNxMemDC(void);
	virtual ~CNxMemDC(void);

	BOOL CreateBlankDC(int iWidth, int iHeight, IN CDC * pDC);
	BOOL CreateWithBitmap(HINSTANCE hInst, UINT nID, IN CDC * pDC = NULL);
	void Destroy();

	void BltTo(CDC * pDestDC, CRect rcSrc, int iDestX, int iDestY);
	void BltTo(CDC * pDestDC, CRect rcSrc, CRect rcDest);
	void TransparentBltTo(CDC * pDestDC, CRect rcSrc, CRect rcDest, COLORREF clrTransparent);

	void AutoBlt(CDC * pDestDC, CRect rcSrc, CRect rcDest, PSNxBlt_Factor pFactor
		, BOOL bFillCenter, BOOL bTransParent = FALSE, COLORREF clrTrans = RGB(255,0,255));

protected:
	int m_iSaved;
	CBitmap m_bmpMem;
};
