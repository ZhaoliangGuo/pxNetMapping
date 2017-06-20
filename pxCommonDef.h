#pragma once

extern CString g_strConfFileName;
extern CString g_strMapResultSavePath;
extern CString g_strCurMapListFilePath;

static HBRUSH static_brush_gray = ::CreateSolidBrush(RGB(96,96,96));
static HBRUSH brush_white       = ::CreateSolidBrush(RGB(255,255,255));

struct SPxMapNode
{
	CString strResourceServerIP;
	CString strResourceServerShareDir; 
	CString strResourceServerShareName; 
	CString strLocalDiskName;
	CString strResourceUserName;
	CString strResourcePassword;
};

enum EPxMapState
{
	kePxMapState_Invalid = -1,
	kePxMapState_ExistAndOK,   // ӳ�������OK
	kePxMapState_ExistButFail, // ӳ����ڵ�δ��������
	kePxMapState_LocalDiskNameBeTaken, // ��ӳ���̷��Ѿ���ռ��
	kePxMapState_NotExist,     // ӳ�䲻����
	kePxMapState_Fail,         // ��ȡӳ��ʧ��
	kePxMapState_Cnt
};

extern CString Utf8ToAnsi(const char* srcCode);
extern BOOL    IsDirExists(LPCTSTR pszPath);
extern BOOL    CreateDir(LPCTSTR pszPath);
extern BOOL    EmptyDirectory(LPCTSTR szPath);
extern BOOL    IsFileExist(LPCTSTR lpFile);