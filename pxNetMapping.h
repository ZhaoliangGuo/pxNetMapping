
// pxNetMapping.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CpxNetMappingApp:
// �йش����ʵ�֣������ pxNetMapping.cpp
//

class CpxNetMappingApp : public CWinApp
{
public:
	CpxNetMappingApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CpxNetMappingApp theApp;