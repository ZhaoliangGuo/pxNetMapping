#include "stdafx.h"
#include "pxCommonDef.h"
#include <string>
using namespace std;

CString g_strConfFileName      = ".\\MapSetting.xml";
CString g_strMapResultSavePath = ".\\MapResult\\";
CString g_strCurMapListFilePath   = g_strMapResultSavePath + "map_list.txt";

CString Utf8ToAnsi(const char* srcCode)
{	
	int srcCodeLen = 0;
	CString strAnsiCode = ""; // 保存ansi数据

	srcCodeLen = MultiByteToWideChar(CP_UTF8, NULL, srcCode, strlen(srcCode), NULL, 0);
	wchar_t* wszBuf = new wchar_t[srcCodeLen + 1];
	MultiByteToWideChar(CP_UTF8, NULL, srcCode, strlen(srcCode), wszBuf, srcCodeLen);
	wszBuf[srcCodeLen]='\0';
	srcCodeLen = WideCharToMultiByte(CP_ACP,NULL, wszBuf, wcslen(wszBuf), NULL,0,NULL,NULL);
	char* szBuf = new char[srcCodeLen + 1];
	WideCharToMultiByte(CP_ACP, NULL, wszBuf, wcslen(wszBuf), szBuf, srcCodeLen, NULL, NULL);
	szBuf[srcCodeLen]='\0';

	// 类型转换
	strAnsiCode = szBuf;

	delete []wszBuf;
	delete []szBuf;
	wszBuf = NULL;
	szBuf  = NULL;

	return strAnsiCode;
}


BOOL IsDirExists(LPCTSTR pszPath)
{
	DWORD dwAttributes = ::GetFileAttributes(pszPath);

	if (dwAttributes == INVALID_FILE_ATTRIBUTES) {
		DWORD dwErr = ::GetLastError();

		if (dwErr == ERROR_ACCESS_DENIED)
			return TRUE;

		return FALSE;
	}

	return (dwAttributes & FILE_ATTRIBUTE_DIRECTORY) ? TRUE : FALSE;
}

BOOL CreateDir(LPCTSTR pszPath)
{
	CString str = pszPath;

	// Add a '\' to the end of the path if there is not one
	int k = str.GetLength(); 
	if (k==0 || str[k-1]!=_T('\\')) str += _T('\\');

	// We must create directory level by level, any better Win API available ?
	bool bUNCPath = false;
	CString strUNC = str.Left(2);
	if (strUNC.CompareNoCase(_T("\\\\")) == 0)
	{
		bUNCPath = true;
	}
	int i = -1;
	if (bUNCPath)
	{
		i = str.Find(_T("\\"),2);
		i = str.Find(_T("\\"),i +1);
	}
	while ((i = str.Find(_T('\\'), i+1)) >= 0)
	{
		CString s = str.Left(i+1);
		if (!IsDirExists(s) && !CreateDirectory(s, NULL))
			return FALSE;
	}

	return TRUE;
}

BOOL EmptyDirectory(LPCTSTR szPath)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	string sFullPath;
	string sFindFilter;
	DWORD dwAttributes = 0;
	sFindFilter = szPath;
	sFindFilter += "\\*.*";
	if ((hFind = FindFirstFile(sFindFilter.c_str(), &wfd)) == INVALID_HANDLE_VALUE)
		return FALSE;
	do
	{
		if (wfd.cFileName[0] == '.')
			continue;
		sFullPath = szPath;
		sFullPath += '\\';
		sFullPath += wfd.cFileName;
		//去掉只读属性
		dwAttributes = GetFileAttributes(sFullPath.c_str());
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath.c_str(), dwAttributes);
		}
		//是个目录
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EmptyDirectory(sFullPath.c_str());
			//This function deletes an existing empty directory
			RemoveDirectory(sFullPath.c_str());
		}
		else
		{
			DeleteFile(sFullPath.c_str());
		}
	}
	while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	return TRUE;
}

BOOL IsFileExist(LPCTSTR lpFile)
{
	HANDLE hFile = CreateFile( lpFile ,
		GENERIC_READ ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL , 
		OPEN_EXISTING ,
		FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED ,
		NULL ) ;

	if( hFile == INVALID_HANDLE_VALUE )
	{
		DWORD dwError = ::GetLastError() ;

		hFile = NULL ;
		return FALSE;
	}

	CloseHandle(hFile);
	hFile = NULL ;

	return TRUE;
}