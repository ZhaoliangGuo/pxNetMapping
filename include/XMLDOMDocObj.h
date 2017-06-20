/************************************************************************/
/*                                                                      */
/************************************************************************/

#pragma once

#ifndef NX_UTILS_EXP
#define NX_UTILS_EXP __declspec(dllimport)
#endif

#import <msxml4.dll> raw_interfaces_only 
//#include <comdef.h>
//#import "msxml3.dll" named_guids raw_interfaces_only
//using namespace MSXML2;

#include <Wincrypt.h>
#pragma comment(lib, "Crypt32.lib")
#include <AFXADV.H>

#define XML_SUCCEEDED(hr) ((HRESULT)(hr) == 0)
#define XML_FAILED(hr) (((HRESULT)(hr)<0) || ((HRESULT)(hr) >= S_FALSE))
#define CHECKHR(x) {hr = x; if (XML_FAILED(hr)) goto CleanUp;}
#define SAFERELEASE(p) {if (p) {(p)->Release(); p = NULL;}}


#define SERIALIZE_USE_GUID
//#define SERIALIZE_USE_MEMORY

struct SNxClassID
{
	// 我们直接使用 GUID String，而不用再通过 GUID <--> String 相互转换
	LPCTSTR lpszClassGUIDName; //GUID guidClassID;
	LPCTSTR lpszClassName;
};

struct SNxClassID2
{
	unsigned long ulClassKey; //classKey = ENxKLVProjectKeyType;
	LPCTSTR lpszClassGUIDName; //GUID guidClassID;
	LPCTSTR lpszClassName;
};
/************************************************************************/
/*                                                                      */
/************************************************************************/
//XML data type 
//i8 
//bin.base64 
//boolean 
//char 
//datetime 
//r8 
//r8 
//bin.base64 
//int 
//r8 
//string 
//string 
//string 
//r8 
//r4 
//datetime 
//i1 ====== char
//i2 
//fixed.14.4 
//string 
//string 
//ui8 
//ui1 ===== BYTE
//ui2
//bin.base64 
//string 
//uuid 

enum ENxXMLDataType
{	
	keXMLDataType_InValid = -1,
	keXMLDataType_String = 0,
	keXMLDataType_BOOL,
	keXMLDataType_Char,
	keXMLDataType_BYTE,
	keXMLDataType_SHORT,
	keXMLDataType_USHORT,
	keXMLDataType_WORD,
	keXMLDataType_INT,
	keXMLDataType_LONG,
	keXMLDataType_UINT,
	keXMLDataType_ULONG,
	keXMLDataType_DWORD,
	keXMLDataType_Float,
	keXMLDataType_Double,
	keXMLDataType_INT64,
	keXMLDataType_UINT64,
	keXMLDataType_GUID,
	keXMLDataType_BINARY,
	keXMLDataType_Count
};

struct SNxXMLBinaryData
{
	BYTE* pBuffer;
	DWORD dwBufferSize;
};

struct SNxXMLDataType
{
	ENxXMLDataType datatype;
	union
	{
		LPTSTR pszVal;
		BOOL bVal;
		CHAR chVal;
		BYTE byVal;
		SHORT shVal;
		USHORT ushVal;
		INT iVal;
		UINT uiVal;
		FLOAT fVal;
		DOUBLE dVal;
		INT64 i64Val;
		UINT64 ui64Val;
		GUID guidVal;
		SNxXMLBinaryData sBinaryDataVal;
	};

	SNxXMLDataType()
	{
		Reset();
	}

	~SNxXMLDataType()
	{
		//Empty();
	}

	void Empty()
	{
		if(pszVal != NULL && datatype == keXMLDataType_String)
		{
			delete [] pszVal; pszVal = NULL;
		}
		if(sBinaryDataVal.pBuffer != NULL && datatype == keXMLDataType_BINARY)
		{
			GlobalFree(sBinaryDataVal.pBuffer);
			sBinaryDataVal.pBuffer = NULL;
			sBinaryDataVal.dwBufferSize = (DWORD)-1;
		}
		datatype = keXMLDataType_InValid;
		guidVal = GUID_NULL;
	}

	void Reset()
	{
		datatype = keXMLDataType_InValid;
        pszVal = NULL;
		bVal = 0;
		chVal = '0';
		byVal = 0;
		shVal = 0;
		ushVal = 0;
		iVal = 0;
		uiVal = 0;
		fVal = 0.;
		dVal = 0.;
		i64Val = 0;
		ui64Val = 0;
		guidVal = GUID_NULL;
		sBinaryDataVal.pBuffer = NULL;
		sBinaryDataVal.dwBufferSize = 0;
	}
};

enum ENxXMLDocMode
{
	keXMLDocMode_Save = 0,
	keXMLDocMode_Load,
	keXMLDocMode_Count
};

#undef GetNextSibling

#pragma warning (disable: 4100)
class NX_UTILS_EXP CNxXMLDOMDocObj
{
public:
	CNxXMLDOMDocObj(ENxXMLDocMode eMode);
	CNxXMLDOMDocObj(IXMLDOMDocument* pDoc, ENxXMLDocMode eMode);
	virtual ~CNxXMLDOMDocObj();

public:
	void SetXMLDocMode(ENxXMLDocMode eMode) { m_eDocMode = eMode; }
	IXMLDOMDocumentPtr GetXMLDOMDocPtr() { return m_pXMLDOMDoc; }
	BOOL IsStoring();
	// 这个函数使用说明：当保存的时候，这个函数需要将 pRootNode 传进来，
	// 同时这个函数将返回pRootNode给 Caller， 当打开的时候， Caller 可以
	// 传 pRootNode = NULL, 这个函数将返回一个根节点。这样做是因为：
	// 对于 PTR 这样的 COM Pointer, 当用 & 操作符，将把这个 PTR pointer 
	// release ！！！
	IXMLDOMNodePtr Serialize(CString strFileName, IXMLDOMNode* pRootNode);

public:
	IXMLDOMNodePtr CreateDOMNode(IXMLDOMNode* pParentNode, CString strName, int type = NODE_ELEMENT);
	IXMLDOMNodePtr CreateDOMNodeWithAttribute(IXMLDOMNode* pParentNode, CString strNodeName, CString strAttrName, SNxXMLDataType sAttriData, int type = NODE_ELEMENT);
	IXMLDOMNodePtr CreateDOMNodeWithValue(IXMLDOMNode* pParentNode, CString strNodeName, SNxXMLDataType sNodeData, int type = NODE_ELEMENT);
	
	HRESULT InsertChildNode(IXMLDOMNode* pParentNode, IXMLDOMNode* pNode);
	HRESULT RemoveChildNode(IXMLDOMNode* pParentNode, IXMLDOMNode* pNode);
	HRESULT AppendChildNode(IXMLDOMNode* pParentNode, IXMLDOMNode* pNode);
	HRESULT ReplaceNode(IXMLDOMNode* pParentNode, IXMLDOMNode* pNewChild, IXMLDOMNode* pOldChild);
	HRESULT CloneNode(IXMLDOMNode* pSrcNode, IXMLDOMNode** ppCloneNode, BOOL bCloneChild = TRUE);
	// 注意：Microsoft CloneNode 有一个问题，如果通过 DOM 
	// Clone 一个节点，DOM 将 Hold 整个 doc 对象的内存，如果
	// 这个 doc 对象占用内存很大的时候，将严重影响内存的使用。
	// 为了避免这种情况的出现，我们提供这个函数，通过文件读取
	// 的方式，来实现 Node 的 Clone。
	// 这个方法只能全部备份所有节点，即所有子节点。
	// 这种方式可能比使用 Microsoft CloneNode 要慢一些。但是现在
	// ONLY .....
	IXMLDOMNodePtr CloneNodeWithNewDoc(IXMLDOMNode* pSrcNode);
	HRESULT RemoveAllChildNode(IXMLDOMNode* pParentNode);

	HRESULT SetNodeAttribute(IXMLDOMNode* pNode, CString strAttrName, SNxXMLDataType sAttriData);
	HRESULT GetNodeAttribute(IXMLDOMNode* pNode, CString strAttrName, SNxXMLDataType& rAttriData);
	HRESULT GetNodeAttribute(IXMLDOMNode* pParentNode, CString strNodeName, CString strAttrName, SNxXMLDataType& rAttriData);
	HRESULT GetNodeValue(IXMLDOMNode* pNode, SNxXMLDataType& rAttriData);
	HRESULT SetNodeValue(IXMLDOMNode* pNode, SNxXMLDataType sAttriData);

	HRESULT GetNodeName(IXMLDOMNode* pNode, CString &strNodeName);
	HRESULT GetNodeText(IXMLDOMNode* pNode, CString &strNodeText);

	HRESULT GetXMLStringFromNode(BSTR* pbstrXMLDoc, IXMLDOMNode* pNode);
	HRESULT GetNodeFromXMLString(BSTR bstrXMLDoc, IXMLDOMNode** ppNode);
	HRESULT GetXMLStringFromNode(LPTSTR* ppstrXMLDoc, IXMLDOMNode* pNode);
	HRESULT GetNodeFromXMLString(LPTSTR pstrXMLDoc, IXMLDOMNode** ppNode);
	
	IXMLDOMNodePtr GetFirstNameMatchChildNode(CString strNodeName);
	IXMLDOMNodeListPtr GetNameMatchChildNodeList(CString strNodeName);
	IXMLDOMNodePtr GetFirstNameMatchChildNode(IXMLDOMNode* pParentNode, CString strNodeName);
	IXMLDOMNodeListPtr GetNameMatchChildNodeList(IXMLDOMNode* pParentNode, CString strNodeName);
	IXMLDOMNodePtr GetFirstAttrNameMatchChildNode(IXMLDOMNode* pParentNode, CString strNodeName, CString strAttrName);
	
	IXMLDOMNodeListPtr GetChildNodeList(IXMLDOMNode* pNode);
	IXMLDOMNodePtr GetFirstChildNode(IXMLDOMNode* pNode);
	IXMLDOMNodePtr GetLastChildNode(IXMLDOMNode* pNode);
	IXMLDOMNodePtr GetPreviousSiblingNode(IXMLDOMNode* pNode);
	IXMLDOMNodePtr GetNextSiblingNode(IXMLDOMNode* pNode);
	IXMLDOMNodePtr GetParentNode(IXMLDOMNode* pNode);


	BOOL IsNodeSameType(IXMLDOMNode* pNode, ENxXMLDataType eDataType);
	BOOL IsNodeAttributeSameType(IXMLDOMNode* pNode, CString strAttrName, ENxXMLDataType eDataType);
	

public:
	IXMLDOMNodePtr GetItem(IXMLDOMNodeList* pNodeList, long lindex);
	long GetLength(IXMLDOMNodeList* pNodeList);
	IXMLDOMNodePtr GetNextNode(IXMLDOMNodeList* pNodeList);


public:
	static CRuntimeClass* GetRunTimeClass(LPCSTR lpszClassName);

protected:
	HRESULT SaveFile(CString strFileName, IXMLDOMNode* pRootNode, BOOL bUseMemory = FALSE);
	HRESULT LoadFile(CString strFileName, IXMLDOMNode** ppRootNode, BOOL bUseMemory = FALSE);

	// 下面这两个函数主要对 Doc 与 BSTR 进行相互转换，这些数据都在内存中
	// 没有形成一个物理文件
public:
	// 如果pRootNode 不为NULL,则将其作为m_pXMLDOMDoc的根节点生成DOMDoc,
	// 然后将数据保存到 string （pbstrXMLDoc）中，若pRootNode为NULL,
	// 则将 Doc 中的数据保存到 string （pbstrXMLDoc）中
	HRESULT SaveXMLString(BSTR* pbstrXMLDoc, IXMLDOMNode* pRootNode = NULL);
	// 将 BSTR （BSTR bstrXMLDoc）中的数据读到 Doc 中，并返回有用的根节点
	HRESULT LoadXMLString(BSTR bstrXMLDoc, IXMLDOMNode** ppRootNode);
	// 这个函数统一调用 SaveXMLString 和 LoadXMLString。
	// 这个函数使用说明：当保存的时候，这个函数需要将 pbstrXMLDoc 传进来，
	// 同时这个函数将返回NULL Node PTR 给 Caller， 当打开的时候， 这个函数
	// 将把 *pbstrXMLDoc 存到 Doc 中，并且返回一个 Doc 根节点。这样做是因为：
	// 对于 PTR 这样的 COM Pointer, 当用 & 操作符，将把这个 PTR pointer 
	// release ！！！
	IXMLDOMNodePtr SerializeXMLString(BSTR* pbstrXMLDoc, BSTR* out_pbstrXMLDoc );

protected:
	IXMLDOMDocumentPtr m_pXMLDOMDoc;
	ENxXMLDocMode m_eDocMode;


};

/************************************************************************/
/*                                                                      */
/************************************************************************/
template <class T> class _unknown : public T
{
private:    long _refcount;

public:        
	_unknown() 
	{ 
		_refcount = 1;
	}

	virtual ~_unknown()
	{
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject)
	{
		if (riid == IID_IUnknown)
		{
			*ppvObject = static_cast<IUnknown*>(this);
		}
		else if (riid == __uuidof(T))
		{
			*ppvObject = static_cast<T*>(this);
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
		reinterpret_cast<IUnknown*>(*ppvObject)->AddRef();
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef( void)
	{
		return InterlockedIncrement(&_refcount);
	}

	virtual ULONG STDMETHODCALLTYPE Release( void)
	{
		if (InterlockedDecrement(&_refcount) == 0)
		{
			delete this;
			return 0;
		}
		return _refcount;
	}
};    

/************************************************************************/
/*                                                                      */
/************************************************************************/

class CFileStream : public _unknown<IStream>
{
public:
	CFileStream() 
	{ 
		m_hFile = NULL;
		m_bRead = true;
	}

	~CFileStream() 
	{ 
		::CloseHandle(m_hFile);
	}

	bool open(char* name, bool bRead = true)
	{
		this->m_bRead = bRead;

#ifdef _UNICODE 
		long len;
		for (len = 0; name[len] != 0; len++);
		WCHAR* name2 = new WCHAR[len+1];
		for (len = 0; name[len] != 0; len++)
			name2[len] = name[len];
		name2[len] = '\0';  

		if (m_bRead)
		{
			m_hFile = ::CreateFile( 
				name2,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		}
		else
		{
			m_hFile = ::CreateFile(
				name2,
				GENERIC_WRITE,
				FILE_SHARE_READ,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		}
		delete[] name2;
#else
		if (m_bRead)
		{
			m_hFile = ::CreateFile( 
				name,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		}
		else
		{
			m_hFile = ::CreateFile(
				name,
				GENERIC_WRITE,
				FILE_SHARE_READ,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		}

#endif

		return (m_hFile == INVALID_HANDLE_VALUE) ? false : true;
	}

	long size()
	{   
		DWORD high;
		DWORD dw = GetFileSize(m_hFile, &high);
		return dw;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
		/* [out] */ void __RPC_FAR *pv,
		/* [in] */ ULONG cb,
		/* [out] */ ULONG __RPC_FAR *pcbRead)
	{	
		if (!m_bRead) return E_FAIL;

		DWORD len;
		BOOL rc = ReadFile(
			m_hFile,	// handle of file to read 
			pv,	// address of buffer that receives data  
			cb,	// number of bytes to read 
			&len,	// address of number of bytes read 
			NULL 	// address of structure for data 
			);
		if (pcbRead)
			*pcbRead = len;
		return (rc) ? S_OK : E_FAIL;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
		/* [size_is][in] */ const void __RPC_FAR *pv,
		/* [in] */ ULONG cb,
		/* [out] */ ULONG __RPC_FAR *pcbWritten)
	{
		if (m_bRead) return E_FAIL;

		BOOL rc = WriteFile(
			m_hFile,	// handle of file to write 
			pv,	// address of buffer that contains data  
			cb,	// number of bytes to write 
			pcbWritten,	// address of number of bytes written 
			NULL 	// address of structure for overlapped I/O  
			);

		return (rc) ? S_OK : E_FAIL;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Seek( 
		/* [in] */ LARGE_INTEGER dlibMove,
		/* [in] */ DWORD dwOrigin,
		/* [out] */ ULARGE_INTEGER __RPC_FAR *plibNewPosition) { return E_FAIL; }

		virtual HRESULT STDMETHODCALLTYPE SetSize( 
		/* [in] */ ULARGE_INTEGER libNewSize) { return E_FAIL; }

		virtual /* [local] */ HRESULT STDMETHODCALLTYPE CopyTo( 
		/* [unique][in] */ IStream __RPC_FAR *pstm,
		/* [in] */ ULARGE_INTEGER cb,
		/* [out] */ ULARGE_INTEGER __RPC_FAR *pcbRead,
		/* [out] */ ULARGE_INTEGER __RPC_FAR *pcbWritten) { return E_FAIL; }

		virtual HRESULT STDMETHODCALLTYPE Commit( 
		/* [in] */ DWORD grfCommitFlags) { return E_FAIL; }

		virtual HRESULT STDMETHODCALLTYPE Revert( void) { return E_FAIL; }

		virtual HRESULT STDMETHODCALLTYPE LockRegion( 
			/* [in] */ ULARGE_INTEGER libOffset,
			/* [in] */ ULARGE_INTEGER cb,
			/* [in] */ DWORD dwLockType) { return E_FAIL; }

			virtual HRESULT STDMETHODCALLTYPE UnlockRegion( 
			/* [in] */ ULARGE_INTEGER libOffset,
			/* [in] */ ULARGE_INTEGER cb,
			/* [in] */ DWORD dwLockType) { return E_FAIL; }

			virtual HRESULT STDMETHODCALLTYPE Stat( 
			/* [out] */ STATSTG __RPC_FAR *pstatstg,
			/* [in] */ DWORD grfStatFlag) { return E_FAIL; }

			virtual HRESULT STDMETHODCALLTYPE Clone( 
			/* [out] */ IStream __RPC_FAR *__RPC_FAR *ppstm) { return E_FAIL; }
private:
	HANDLE m_hFile;
	bool m_bRead;
};


/************************************************************************/
/*                                                                      */
/************************************************************************/

// this callback func use to determine the file is one suitable recent 
// file or not
typedef BOOL (CALLBACK* pfnFilterCallback)(LPCTSTR);

#define MAX_DISPLAY_FILENAME_LEN 50

// herit our recent file class from MFC class CRecentFileList
class NX_UTILS_EXP CRecentFileList2 : public CRecentFileList 
{
public:
	CRecentFileList2(UINT nBaseID,
					 LPCTSTR pszSection,
					 LPCTSTR pszEntryFormat,
					 pfnFilterCallback pfnCB,
					 int nMaxMRU = 4,
					 int nMaxDispLen = MAX_DISPLAY_FILENAME_LEN,
					 UINT uiStart = 0);
	~CRecentFileList2();

	virtual void UpdateMenu(CCmdUI* pCmdUI);
	virtual BOOL IsKindOfFile(LPCTSTR pszPathName);

public:
	// base command ID
	UINT m_uiBaseID;
	// function to test filename
	pfnFilterCallback m_pfnCB;
};

// Recent file Manager class manages multiple recent file lists. To be use:
// * instantiate one of these in your app;
// * override your app's OnCmdMsg to pass to mru file manager;
// * override your app's AddToRecentFileList to call mru manager;
// * in your InitInstance, call Add to add each type of file.
//
class NX_UTILS_EXP CMRUFileManager : public CCmdTarget
{
	friend class CNxDesktop;
	friend class CNxMainMenuBar;
	friend class CNxEditStartDlg;
	friend class CNxEditApp;
	friend class CNxNewsApp;
	friend class CNxProjectManager;

public:
	CMRUFileManager(CWinApp* pApp);
	virtual ~CMRUFileManager();

	// call this from your app's AddToRecentFileList, return if TRUE
	BOOL AddToRecentFileList(int i, LPCTSTR pszPathName);

	// call from InitInstance to add each MRU file type (eg, file/project)
	BOOL Add(UINT uiBaseID,								// base command ID
			 LPCTSTR pszSection,						// registry section (keyname)
			 LPCTSTR pszEntryFormat,					// registry value name
			 pfnFilterCallback pfnCB,					// filter function
			 int nMaxMRU = 4,							// max num menu entries
			 BOOL bNoLoad = FALSE,						// don't load from reg (rarely used)
			 int nMaxDispLen = MAX_DISPLAY_FILENAME_LEN, // display length
			 UINT uiStart = 0);							// size to start

protected:
	CRecentFileList2* FindRFL(UINT uiID);

protected:
	// list of CRecentFileList2's
	CPtrList m_lstRFL;
	// backup ptr to app
	CWinApp* m_pApp;

	DECLARE_DYNAMIC(CMRUFileManager)
protected:
	afx_msg void OnUpdateRecentFileMenu(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};
#pragma warning (default: 4100)
