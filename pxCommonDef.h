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
	kePxMapState_ExistAndOK,   // 映射存在且OK
	kePxMapState_ExistButFail, // 映射存在但未正常连接
	kePxMapState_LocalDiskNameBeTaken, // 该映射盘符已经被占用
	kePxMapState_NotExist,     // 映射不存在
	kePxMapState_Fail,         // 获取映射失败
	kePxMapState_Cnt
};

extern CString Utf8ToAnsi(const char* srcCode);
extern BOOL    IsDirExists(LPCTSTR pszPath);
extern BOOL    CreateDir(LPCTSTR pszPath);
extern BOOL    EmptyDirectory(LPCTSTR szPath);
extern BOOL    IsFileExist(LPCTSTR lpFile);