#include "StdAfx.h"
#include "pxMapManager.h"

CPxMapManager::CPxMapManager(void)
{
}


CPxMapManager::~CPxMapManager(void)
{
}

//////////////////////////////////////////////////////////////////
// Function    : 实现映射功能
// Date        : 20150127
// ReturnValue : kePxMapState_ExistAndOK -- 映射成功
//               其它返回值                -- 映射失败 
//////////////////////////////////////////////////////////////////
EPxMapState CPxMapManager::map( const SPxMapNode *in_kpsPxMapnode )
{
	ASSERT(in_kpsPxMapnode);
	if (in_kpsPxMapnode == NULL)
	{
		return kePxMapState_Fail;
	}

	bool bRet = false;

	EPxMapState eMapState = GetMapState(in_kpsPxMapnode);

	//CString strCommand = " net use ";

	if (eMapState == kePxMapState_ExistAndOK)
	{
		return kePxMapState_ExistAndOK;
	}
	else
	{
		CString strCommand = "/c net use ";

		strCommand += in_kpsPxMapnode->strLocalDiskName;
		strCommand += " \\\\";
		strCommand += in_kpsPxMapnode->strResourceServerIP;
		strCommand += "\\";
		strCommand += in_kpsPxMapnode->strResourceServerShareName;		
		strCommand += " /user:";
		strCommand += in_kpsPxMapnode->strResourceUserName;
		strCommand += " ";
		CString strAuthInfo("");
		if (in_kpsPxMapnode->strResourcePassword != "")
		{
			strAuthInfo = " \"" + in_kpsPxMapnode->strResourcePassword + "\"";
			strCommand += strAuthInfo;
		}
		else
		{
			strCommand += "\"\"";
		}

		// test
		//strCommand += " > dest.txt";
		// test
		
		int iRet = (int)ShellExecute(NULL, "open", "cmd.exe", strCommand, NULL, SW_HIDE);
		//int iRet = (int)ShellExecute(NULL, "open", "cmd.exe", strCommand, NULL, SW_NORMAL);

		Sleep(500);
	}

	eMapState = GetMapState(in_kpsPxMapnode);

	//if (eMapState != kePxMapState_ExistAndOK)
	//{
	//	//CString strFilePath = g_strMapResultSavePath;
	//	CString strFilePath("");
	//	CString strFileName = in_kpsPxMapnode->strResourceServerIP;
	//	strFileName.Replace(".", "_");
	//	strFilePath += strFileName;
	//	strFilePath += ".bat";

	//	CStdioFile srcFile;
	//	CString    strContent;
	//	CString    strLine = strCommand;
	//	CString    strANSI; 
	//	BOOL bRet = srcFile.Open(strFilePath, CFile::modeCreate|CFile::modeReadWrite);
	//	if (bRet)
	//	{
	//		//strLine += "\n";
	//		srcFile.WriteString(strLine);
	//		srcFile.Close();
	//	}

	//	Sleep(100);
	//	int iRet = (int)ShellExecute(NULL, "open", strFilePath, NULL, NULL, SW_NORMAL);
	//}

	return eMapState;
}

//////////////////////////////////////////////////////////////////
// Function    : 删除映射
// Date        : 20150127
// ReturnValue : true                 -- 删除映射成功
//               false                -- 删除映射失败 
//////////////////////////////////////////////////////////////////
bool CPxMapManager::upmap( const SPxMapNode *in_kpsPxMapnode )
{
	bool bRet = false;

	CString strCommand = "/c net use " + in_kpsPxMapnode->strLocalDiskName;
	strCommand += " /delete ";

	/*CString strCommand = "/c net use ";
	strCommand += " \\\\";
	strCommand += in_kpsPxMapnode->strResourceServerIP;
	strCommand += "\\";
	strCommand += in_kpsPxMapnode->strResourceServerShareName;	
	strCommand += " /delete ";*/

	int iRet = (int)ShellExecute(NULL, "open", "cmd.exe ", strCommand, NULL, SW_HIDE);
	//int iRet = (int)ShellExecute(NULL, "open", "cmd.exe ", strCommand, NULL, SW_NORMAL);
	Sleep(500);

	EPxMapState eMapState = GetMapState(in_kpsPxMapnode);
	if (eMapState == kePxMapState_NotExist)
	{
		bRet = true;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////
// Function    : 判断映射的状态
// Date        : 20150127
// ReturnValue : kePxMapState_ExistAndOK    -- 映射存在且OK
//               kePxMapState_ExistButFail  -- 映射存在但未正常连接
//	             kePxMapState_NotExist      -- 映射不存在
//	             kePxMapState_Fail          -- 获取映射失败
//////////////////////////////////////////////////////////////////
EPxMapState CPxMapManager::GetMapState( const SPxMapNode *in_kpsPxMapnode )
{
	EPxMapState eMapState = kePxMapState_NotExist;

	EmptyDirectory(g_strMapResultSavePath);

	// Get the current net mapping.
	CString strCmd = "/c net use > " + g_strCurMapListFilePath;
	int iRet = (int)ShellExecute(NULL, "open", "cmd.exe", strCmd, NULL, SW_HIDE);
	bool bGetMapListSuccess = false;
	Sleep(500);

	if ((iRet > 32) && IsFileExist(g_strCurMapListFilePath))
	{
		bGetMapListSuccess = true;
	}
	else
	{
		return kePxMapState_Fail;
	}

	CString strRemoteSharePath = "\\\\" + in_kpsPxMapnode->strResourceServerIP;
	strRemoteSharePath += "\\";
	strRemoteSharePath += in_kpsPxMapnode->strResourceServerShareName;

	CStdioFile srcFile;
	CString    strContent("");
	CString    strLine("");
	CString    strANSI("");
	bool       bIsMapExist = false;

	BOOL bRet = srcFile.Open(g_strCurMapListFilePath, CFile::modeReadWrite);
	if (bRet)
	{
		while(srcFile.ReadString(strLine))
		{
			if ((strLine.Find(strRemoteSharePath, 0) != -1) 
				&& (strLine.Find(in_kpsPxMapnode->strLocalDiskName, 0) != -1)) // 资源服务器的ip及共享文件名及映射盘符均相同
			{			
				bIsMapExist = true;

				if ((strLine.Find("OK", 0) != -1))
				{
					eMapState = kePxMapState_ExistAndOK;
				}
				else
				{
					eMapState = kePxMapState_ExistButFail;
				}

				break;
			}
			//else if ((strLine.Find(in_kpsPxMapnode->strLocalDiskName, 0) != -1)) // 盘符已经被占用
			//{
			//	eMapState = kePxMapState_LocalDiskNameBeTaken;
			//	break;
			//}
		}

		srcFile.Close();
	}
	else
	{
		eMapState = kePxMapState_Fail;
	}

	
	if (!bIsMapExist)
	{
		eMapState = kePxMapState_NotExist;
	}

	return eMapState;
}

// has problem
void CPxMapManager::GetLocalDiskList(vector <CString> &out_vstrDiskNameList )
{
	EmptyDirectory(g_strMapResultSavePath);

	// Get the current net mapping.
	CString strCmd = "/c net use > " + g_strCurMapListFilePath;
	int iRet = (int)ShellExecute(NULL, "open", "cmd.exe", strCmd, NULL, SW_HIDE);
	bool bGetMapListSuccess = false;
	Sleep(500);

	if ((iRet > 32) && IsFileExist(g_strCurMapListFilePath))
	{
		bGetMapListSuccess = true;
	}
	else
	{
		return ;
	}

	CStdioFile srcFile;
	CString    strContent("");
	CString    strLine("");
	CString    strANSI("");
	bool       bIsMapExist = false;

	BOOL bRet = srcFile.Open(g_strCurMapListFilePath, CFile::modeReadWrite);
	if (bRet)
	{
		while(srcFile.ReadString(strLine))
		{
			for (char iDiskName = 'Z'; iDiskName >= 'H'; iDiskName--)
			{
				bool bDiskNameExist = false;
				CString strDiskName;
				strDiskName.Format("%c:", iDiskName);

				if ((strLine.Find(iDiskName, 0) != -1)) 
				{
					out_vstrDiskNameList.push_back(strDiskName);
					break;
				}
			}
		}

		srcFile.Close();
	}
}
