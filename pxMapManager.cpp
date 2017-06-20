#include "StdAfx.h"
#include "pxMapManager.h"

CPxMapManager::CPxMapManager(void)
{
}


CPxMapManager::~CPxMapManager(void)
{
}

//////////////////////////////////////////////////////////////////
// Function    : ʵ��ӳ�书��
// Date        : 20150127
// ReturnValue : kePxMapState_ExistAndOK -- ӳ��ɹ�
//               ��������ֵ                -- ӳ��ʧ�� 
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
// Function    : ɾ��ӳ��
// Date        : 20150127
// ReturnValue : true                 -- ɾ��ӳ��ɹ�
//               false                -- ɾ��ӳ��ʧ�� 
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
// Function    : �ж�ӳ���״̬
// Date        : 20150127
// ReturnValue : kePxMapState_ExistAndOK    -- ӳ�������OK
//               kePxMapState_ExistButFail  -- ӳ����ڵ�δ��������
//	             kePxMapState_NotExist      -- ӳ�䲻����
//	             kePxMapState_Fail          -- ��ȡӳ��ʧ��
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
				&& (strLine.Find(in_kpsPxMapnode->strLocalDiskName, 0) != -1)) // ��Դ��������ip�������ļ�����ӳ���̷�����ͬ
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
			//else if ((strLine.Find(in_kpsPxMapnode->strLocalDiskName, 0) != -1)) // �̷��Ѿ���ռ��
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
