
// pxNetMappingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pxNetMapping.h"
#include "pxNetMappingDlg.h"
#include "afxdialogex.h"
#include <locale>
#include "pxMapManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPxMapManager g_oPxMapManager;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpxNetMappingDlg 对话框




CPxNetMappingDlg::CPxNetMappingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPxNetMappingDlg::IDD, pParent)
	, m_strResourceHostUserName(_T(""))
	, m_strResourceHostPassword(_T(""))
	, m_strResourceHostShareDir(_T(""))
	, m_strResourceHostShareName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPxNetMappingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_RESOURCE_HOST_IP, m_ipaddrResourceHostIP);
	DDX_Control(pDX, IDC_LIST_MAPPING, m_lcMap);
	DDX_Text(pDX, IDC_EDIT__RESOURCE_HOST_USERNAME, m_strResourceHostUserName);
	DDX_Text(pDX, IDC_EDIT__RESOURCE_HOST_PASSWORD, m_strResourceHostPassword);
	DDX_Text(pDX, IDC_EDIT__RESOURCE_HOST_SHARE_DIR, m_strResourceHostShareDir);
	DDX_Text(pDX, IDC_EDIT__RESOURCE_HOST_SHARE_NAME, m_strResourceHostShareName);
	DDX_Control(pDX, IDC_COMBO_IS_HAS_PASSWORD, m_cbPasswordSwitch);
	DDX_Control(pDX, IDC_COMBO_MAP_DISK_NAME, m_cbMapDisk);
	DDX_Control(pDX, IDC_CHECK_SELECT_OPPO, m_checkMainSelOppo);
	DDX_Control(pDX, IDC_CHECK_SELECT_ALL, m_checkMainSelAll);
	DDX_Control(pDX, IDC_BUTTON_ADD_MAPPING, m_btnAddMapping);
	DDX_Control(pDX, IDC_BUTTON_DELETE_MAPPING, m_btnDelMapping);
	DDX_Control(pDX, IDC_BUTTON_MAP_BY_HAND, m_btnMapByHand);
}

BEGIN_MESSAGE_MAP(CPxNetMappingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ADD_MAPPING, &CPxNetMappingDlg::OnAddMapping)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_MAPPING, &CPxNetMappingDlg::OnDeleteMapping)
	ON_BN_CLICKED(IDC_BUTTON_MAP_BY_HAND, &CPxNetMappingDlg::OnMapByHand)
	ON_CBN_SELCHANGE(IDC_COMBO_IS_HAS_PASSWORD, &CPxNetMappingDlg::OnCbnSelchangeComboIsHasPassword)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_SELECT_OPPO, &CPxNetMappingDlg::OnSelectOppo)
	ON_BN_CLICKED(IDC_CHECK_SELECT_ALL, &CPxNetMappingDlg::OnSelectAll)
END_MESSAGE_MAP()


// CpxNetMappingDlg 消息处理程序

BOOL CPxNetMappingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	CreateEssentialFolder(); // 创建必须的文件夹
	CreateControls();        // 创建控件
	Init();                  // 初始化控件

	/*vector <SPxMapNode> vsMapNode;
	Parse(g_strConfFileName, vsMapNode);
	EPxMapState eMapState = kePxMapState_NotExist;

	for (unsigned int i = 0; i < vsMapNode.size(); i++)
	{
		eMapState = g_oPxMapManager.GetMapState(&vsMapNode[i]);
		if (eMapState == kePxMapState_ExistAndOK)
		{
			continue;
		}
		
		g_oPxMapManager.map(&vsMapNode[i]);	

		bool bRet = g_oPxMapManager.upmap(&vsMapNode[i]);
		if (bRet)
		{
			AfxMessageBox("unmap success");
		}
	}*/

	m_checkMainSelAll.SetCheck(FALSE);
	m_checkMainSelOppo.SetCheck(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPxNetMappingDlg::CreateEssentialFolder()
{
	if (!IsDirExists(g_strMapResultSavePath))
	{
		CreateDir(g_strMapResultSavePath);
	}
}

void CPxNetMappingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_MINIMIZE)
	{
		//AfxMessageBox("min");
		SaveSetting2ConfFile();
		AfxGetMainWnd()->ShowWindow(SW_MINIMIZE);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPxNetMappingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPxNetMappingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CPxNetMappingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	HBRUSH rHbr = hbr;

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_EDIT__RESOURCE_HOST_USERNAME:
	case IDC_EDIT__RESOURCE_HOST_PASSWORD:
	case IDC_EDIT__RESOURCE_HOST_SHARE_DIR:
	case IDC_EDIT__RESOURCE_HOST_SHARE_NAME:
	case IDC_IPADDRESS_RESOURCE_HOST_IP:
		 pDC->SetTextColor(RGB(0,0,0));
		 pDC->SetBkColor(RGB(255,255,255));
		 rHbr = (HBRUSH)brush_white;

		break;

	case IDC_COMBO_MAP_DISK_NAME:
	case IDC_COMBO_IS_HAS_PASSWORD:
		rHbr = hbr;
		break;

    default:
		pDC->SetBkColor(RGB(96, 96, 96));
		pDC->SetTextColor(RGB(255, 255, 255));
		rHbr = (HBRUSH)static_brush_gray;
		break;
	}

	return rHbr;
	
}

void CPxNetMappingDlg::CreateControls()
{
	m_lcMap.ModifyStyle(0, LVS_REPORT);
	m_lcMap.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES);

	m_lcMap.SetBkColor(RGB(96,96,96)); 
	m_lcMap.SetTextBkColor(RGB(96,96,96));
	m_lcMap.SetTextColor(RGB(255,255,255)); //显示字体的颜色

	// 设置各列的名称及宽度等
	m_lcMap.InsertColumn(0,_T("选择"),LVCFMT_CENTER,40,-1);
	m_lcMap.InsertColumn(1,_T("资源服务器IP地址"),LVCFMT_CENTER,120,-1);
	m_lcMap.InsertColumn(2,_T("用户名"), LVCFMT_LEFT, 100,-1);
	m_lcMap.InsertColumn(3,_T("密码"), LVCFMT_LEFT, 50,-1);
	m_lcMap.InsertColumn(4,_T("资源服务器共享目录"), LVCFMT_LEFT,120,-1);
	m_lcMap.InsertColumn(5,_T("共享名"), LVCFMT_LEFT,100,-1);
	m_lcMap.InsertColumn(6,_T("本机映射盘符"), LVCFMT_CENTER,100,-1);
	m_lcMap.InsertColumn(7,_T("状态"), LVCFMT_CENTER,88,-1);

	/*CMFCButton *pAddButton = new CMFCButton; 
	pAddButton->Create(_T("添加"), WS_VISIBLE, CRect(217,83,54,19), this, IDC_BUTTON_ADD_MAPPING); 

	CMFCButton * pAddButton = (CMFCButton *)GetDlgItem(IDC_BUTTON_ADD_MAPPING);
	pAddButton->SetTextColor(RGB(255, 255, 255));

	// 更改文字 
	pAddButton->SetWindowText("添加映射"); 
	// 更改背景颜色 
	pAddButton->SetFaceColor(RGB(153, 217, 234)); 
	// 更改字体颜色 
	pAddButton->SetTextColor(RGB(255, 255, 255)); 
	// 更改高亮颜色 
	pAddButton->SetTextHotColor(RGB(63, 72, 204)); 
	// 更改位置 
	//button->MoveWindow(50, 50, 100, 70); 
	// 鼠标经过时变成手势 
	pAddButton->SetMouseCursorHand(); 
	// 工具提示 
	pAddButton->SetTooltip(_T("提示")); */ 
}

void CPxNetMappingDlg::Init()
{
	UpdateData();

	// Password switch.
	m_cbPasswordSwitch.ResetContent();
	m_cbPasswordSwitch.AddString("是");
	m_cbPasswordSwitch.AddString("否");
	m_cbPasswordSwitch.SelectString(0, "是");

	// map list 
	vector <SPxMapNode> vsMapNode;
	Parse(g_strConfFileName, vsMapNode);
	unsigned int nMap = vsMapNode.size();
	//vector <CString> vstrExistDiskName;
	EPxMapState eMapState = kePxMapState_NotExist;

	for (unsigned int i = 0; i < nMap; i++)
	{
		int nMaxIndex = m_lcMap.InsertItem(i, _T(""));
		m_lcMap.SetItemText(nMaxIndex, 0, "");
		m_lcMap.SetItemTextColor(nMaxIndex, 0, RGB(255,255,255));
		m_lcMap.SetItemBkColor(nMaxIndex, 0, RGB(96,96,96));
		m_lcMap.SetItemText(nMaxIndex, 1, vsMapNode[i].strResourceServerIP);
		m_lcMap.SetItemTextColor(nMaxIndex, 1, RGB(255,255,255));
		m_lcMap.SetItemBkColor(nMaxIndex, 1, RGB(96,96,96));
		m_lcMap.SetItemText(nMaxIndex, 2, vsMapNode[i].strResourceUserName);
		m_lcMap.SetItemTextColor(nMaxIndex, 2, RGB(255,255,255));
		m_lcMap.SetItemBkColor(nMaxIndex, 2, RGB(96,96,96));
		m_lcMap.SetItemText(nMaxIndex, 3, vsMapNode[i].strResourcePassword);
		m_lcMap.SetItemTextColor(nMaxIndex, 3, RGB(255,255,255));
		m_lcMap.SetItemBkColor(nMaxIndex, 3, RGB(96,96,96));
		m_lcMap.SetItemText(nMaxIndex, 4, vsMapNode[i].strResourceServerShareDir);
		m_lcMap.SetItemTextColor(nMaxIndex, 4, RGB(255,255,255));
		m_lcMap.SetItemBkColor(nMaxIndex, 4, RGB(96,96,96));
		m_lcMap.SetItemText(nMaxIndex, 5, vsMapNode[i].strResourceServerShareName);
		m_lcMap.SetItemTextColor(nMaxIndex, 5, RGB(255,255,255));
		m_lcMap.SetItemBkColor(nMaxIndex, 5, RGB(96,96,96));
		m_lcMap.SetItemText(nMaxIndex, 6, vsMapNode[i].strLocalDiskName);
		m_lcMap.SetItemTextColor(nMaxIndex, 6, RGB(255,255,255));
		m_lcMap.SetItemBkColor(nMaxIndex, 6, RGB(96,96,96));
		m_lcMap.SetItemText(nMaxIndex, 7, "映射中...");
		m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
		m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,0,255));

		/*eMapState = g_oPxMapManager.GetMapState(&vsMapNode[i]);
		if (eMapState != kePxMapState_ExistAndOK)
		{
			g_oPxMapManager.map(&vsMapNode[i]);
			eMapState = g_oPxMapManager.GetMapState(&vsMapNode[i]);
		}

		if (eMapState == kePxMapState_ExistAndOK)
		{
			m_lcMap.SetItemText(nMaxIndex, 7, "映射成功");
			m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
			m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,255,0));
		}
		else
		{
			m_lcMap.SetItemText(nMaxIndex, 7, "映射失败");
			m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(255,255,255));
			m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(255,0,0));
		}*/

		eMapState = g_oPxMapManager.GetMapState(&vsMapNode[i]);

		if (eMapState == kePxMapState_ExistAndOK)
		{
			m_lcMap.SetItemText(nMaxIndex, 7, "映射成功");
			m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
			m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,255,0));
		}
		else
		{
			g_oPxMapManager.map(&vsMapNode[i]);
			eMapState = g_oPxMapManager.GetMapState(&vsMapNode[i]);

			if (eMapState == kePxMapState_ExistAndOK)
			{
				m_lcMap.SetItemText(nMaxIndex, 7, "映射成功");
				m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
				m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,255,0));
			}
			else
			{
				m_lcMap.SetItemText(nMaxIndex, 7, "映射失败");
				m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(255,255,255));
				m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(255,0,0));
			}
		}

		Sleep(50);
	}

	// Disk name.
	RefreshMapDiskName();

	m_strResourceHostUserName  = "Administrator";
	m_strResourceHostShareDir  = "D:\\Root";
	m_strResourceHostShareName = "Root";

	UpdateData(FALSE);
}

bool CPxNetMappingDlg::Parse( const CString in_kstrConfFileName, vector <SPxMapNode> &out_vsMapNode )
{
	// 格式转换
	CStdioFile srcFile;
	CString    strContent;
	CString    strLine;
	CString    strANSI;
	BOOL bRet = srcFile.Open(in_kstrConfFileName, CFile::modeReadWrite);
	if (bRet)
	{
		while(srcFile.ReadString(strLine))
		{
			strContent += strLine;
		}
		strANSI = Utf8ToAnsi(strContent);
		srcFile.Close();
	}
	else
	{
		return false;
	}

	vector <SPxMapNode> vsMapNode;

	// 解析文件
	setlocale(LC_ALL, "");
	CMarkup xml;
	BOOL bLoad = FALSE;
	bLoad = xml.SetDoc(strANSI);
	if (!bLoad)
	{
		TRACE("Load Err!");
		return "";
	}

	xml.ResetPos();


	if (bLoad)
	{
		xml.ResetMainPos();
		if (xml.FindChildElem("Map"))
		{
			xml.IntoElem();
			while (xml.FindChildElem("MapNode"))
			{
				xml.IntoElem();
				SPxMapNode sPxMapNode;
				xml.FindChildElem("Resource_Server_IP");
				sPxMapNode.strResourceServerIP           = xml.GetChildData();

				xml.FindChildElem("Resource_Server_UserName");
				sPxMapNode.strResourceUserName		   = xml.GetChildData();

				xml.FindChildElem("Resource_Server_Password");
				sPxMapNode.strResourcePassword           = xml.GetChildData();

				xml.FindChildElem("Resource_Server_Share_Dir");
				sPxMapNode.strResourceServerShareDir     = xml.GetChildData();

				xml.FindChildElem("Resource_Server_Share_Name");
				sPxMapNode.strResourceServerShareName    = xml.GetChildData();

				xml.FindChildElem("Local_Disk_Name");
				sPxMapNode.strLocalDiskName = xml.GetChildData();

				vsMapNode.push_back(sPxMapNode);
				xml.OutOfElem();
			}

			xml.OutOfElem();
		}
	}

	out_vsMapNode = vsMapNode;

	return true;
}

void CPxNetMappingDlg::OnAddMapping()
{
	UpdateData();

	SPxMapNode sMapNode;

	BYTE nField1;
	BYTE nField2;
	BYTE nField3;
	BYTE nField4;

	// RB Server IP.
	if (m_ipaddrResourceHostIP.IsBlank())
	{
		AfxMessageBox("资源服务器IP地址不能为空，请确认.");
		m_ipaddrResourceHostIP.SetFocus();
		return ;
	}

	m_ipaddrResourceHostIP.GetAddress(nField1, nField2, nField3, nField4);
	sMapNode.strResourceServerIP.Format("%d.%d.%d.%d", nField1, nField2, nField3, nField4);

	// RB Server username and password.
	CString strPasswordSwitch;
	m_cbPasswordSwitch.GetLBText(m_cbPasswordSwitch.GetCurSel(), strPasswordSwitch);

	if (m_strResourceHostUserName.IsEmpty())
	{
		AfxMessageBox("资源服务器的用户名不能为空，请确认.");
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_USERNAME))->SetFocus();
		return ;
	}

	sMapNode.strResourceUserName = m_strResourceHostUserName;

	if (strPasswordSwitch == "是")
	{
		if (m_strResourceHostPassword.IsEmpty())
		{
			AfxMessageBox("资源服务器的密码不能为空，请确认.");
			((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_PASSWORD))->SetFocus();
			return ;
		}

		sMapNode.strResourcePassword = m_strResourceHostPassword;
	}
	else if (strPasswordSwitch == "否")
	{
		sMapNode.strResourcePassword = "";
	}

	// RB Server share dir.
	if (m_strResourceHostShareDir.IsEmpty())
	{
		AfxMessageBox("资源服务器的共享目录不能为空，请确认.");
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_SHARE_DIR))->SetFocus();
		return ;
	}

	sMapNode.strResourceServerShareDir = m_strResourceHostShareDir;

	// RB Server share name.

	if (m_strResourceHostShareName.IsEmpty())
	{
		AfxMessageBox("资源服务器的共享名不能为空，请确认.");
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_SHARE_NAME))->SetFocus();
		return ;
	}

	sMapNode.strResourceServerShareName = m_strResourceHostShareName;

	bool bIsExist = false;
	int nMapDiskCnt = m_lcMap.GetItemCount();
	for (int i = nMapDiskCnt - 1; i >= 0; i--)
	{

		if (m_lcMap.GetItemText(i, 1) == sMapNode.strResourceServerIP
			&&m_lcMap.GetItemText(i, 5) == sMapNode.strResourceServerShareName)
		{
			bIsExist = true;
			break;
		}
	}

	if (bIsExist)
	{
		CString strMsg = "服务器 " + sMapNode.strResourceServerIP + " 上的共享名为 ";
		strMsg += sMapNode.strResourceServerShareName;
		strMsg += " 的映射已经存在";
		AfxMessageBox(strMsg);
		return ;
	}

	m_btnAddMapping.EnableWindow(FALSE);
	m_btnDelMapping.EnableWindow(FALSE);
	m_btnMapByHand.EnableWindow(FALSE);

	// Compile Server Map disk name.
	m_cbMapDisk.GetWindowText(sMapNode.strLocalDiskName);

	int nExistDiskNameCnt = m_lcMap.GetItemCount();

	int nMaxIndex = m_lcMap.InsertItem(nExistDiskNameCnt, "");
	m_lcMap.SetItemTextColor(nMaxIndex, 0, RGB(255,255,255));
	m_lcMap.SetItemBkColor(nMaxIndex, 0, RGB(96,96,96));
	m_lcMap.SetItemText(nMaxIndex, 1, sMapNode.strResourceServerIP);
	m_lcMap.SetItemTextColor(nMaxIndex, 1, RGB(255,255,255));
	m_lcMap.SetItemBkColor(nMaxIndex, 1, RGB(96,96,96));
	m_lcMap.SetItemText(nMaxIndex, 2, sMapNode.strResourceUserName);
	m_lcMap.SetItemTextColor(nMaxIndex, 2, RGB(255,255,255));
	m_lcMap.SetItemBkColor(nMaxIndex, 2, RGB(96,96,96));
	m_lcMap.SetItemText(nMaxIndex, 3, sMapNode.strResourcePassword);
	m_lcMap.SetItemTextColor(nMaxIndex, 3, RGB(255,255,255));
	m_lcMap.SetItemBkColor(nMaxIndex, 3, RGB(96,96,96));

	m_lcMap.SetItemText(nMaxIndex, 4, sMapNode.strResourceServerShareDir);
	m_lcMap.SetItemTextColor(nMaxIndex, 4, RGB(255,255,255));
	m_lcMap.SetItemBkColor(nMaxIndex, 4, RGB(96,96,96));
	m_lcMap.SetItemText(nMaxIndex, 5, sMapNode.strResourceServerShareName);
	m_lcMap.SetItemTextColor(nMaxIndex, 5, RGB(255,255,255));
	m_lcMap.SetItemBkColor(nMaxIndex, 5, RGB(96,96,96));
	m_lcMap.SetItemText(nMaxIndex, 6, sMapNode.strLocalDiskName);
	m_lcMap.SetItemTextColor(nMaxIndex, 6, RGB(255,255,255));
	m_lcMap.SetItemBkColor(nMaxIndex, 6, RGB(96,96,96));

	m_lcMap.SetItemText(nMaxIndex, 7, "映射中...");
	m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
	m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,0,255));

	EPxMapState eMapState = g_oPxMapManager.GetMapState(&sMapNode);
	Sleep(50);
	if (eMapState == kePxMapState_ExistAndOK)
	{
		m_lcMap.SetItemText(nMaxIndex, 7, "映射成功");
		m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
		m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,255,0));
	}
	else
	{
		g_oPxMapManager.map(&sMapNode);
		Sleep(100);
		eMapState = g_oPxMapManager.GetMapState(&sMapNode);

		if (eMapState == kePxMapState_ExistAndOK)
		{
			m_lcMap.SetItemText(nMaxIndex, 7, "映射成功");
			m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
			m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,255,0));
		}
		else
		{
			m_lcMap.SetItemText(nMaxIndex, 7, "映射失败");
			m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(255,255,255));
			m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(255,0,0));

			ShowErrorMessage(&sMapNode);
		}
	}

	RefreshMapDiskName();
	m_checkMainSelAll.SetCheck(FALSE);

	m_btnAddMapping.EnableWindow(TRUE);
	m_btnDelMapping.EnableWindow(TRUE);
	m_btnMapByHand.EnableWindow(TRUE);

	UpdateData(FALSE);

	SaveSetting2ConfFile();
}

void CPxNetMappingDlg::ShowErrorMessage(const SPxMapNode *in_kpsPxMapnode)
{
	CString strCommand = "net use ";

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

	CString strFilePath = g_strMapResultSavePath;
	CString strFileName = in_kpsPxMapnode->strResourceServerIP;
	strFileName.Replace(".", "_");
	strFilePath += strFileName;
	strFilePath += ".bat";

	CStdioFile srcFile;

	BOOL bRet = srcFile.Open(strFilePath, CFile::modeCreate|CFile::modeWrite);
	if (bRet)
	{
		srcFile.WriteString("@echo 当前映射操作无法正常完成, 错误显示如下(请稍后):\n ");
		srcFile.WriteString(strCommand.GetBuffer());
		srcFile.WriteString("\n pause");
		srcFile.Close();
	}

	Sleep(100);
	//ShellExecute(NULL,"open","cmd.exe","/c KillDCOMServer.bat", NULL, SW_HIDE);
	//ShellExecute(NULL,"open","cmd.exe",strFilePath, NULL, SW_NORMAL);
	int iRet = (int)ShellExecute(NULL, "open", strFilePath, NULL, NULL, SW_NORMAL);
}


void CPxNetMappingDlg::OnDeleteMapping()
{
	m_btnAddMapping.EnableWindow(FALSE);
	m_btnDelMapping.EnableWindow(FALSE);
	m_btnMapByHand.EnableWindow(FALSE);

	UpdateData();
	SPxMapNode sMapNode;

	int nMapDiskCnt = m_lcMap.GetItemCount();
	for (int i = nMapDiskCnt - 1; i >= 0; i--)
	{
		if(m_lcMap.GetCheck(i))
		{
			sMapNode.strResourceServerIP        = m_lcMap.GetItemText(i, 1);
			sMapNode.strResourceUserName        = m_lcMap.GetItemText(i, 2);
			sMapNode.strResourcePassword        = m_lcMap.GetItemText(i, 3);
			sMapNode.strResourceServerShareDir  = m_lcMap.GetItemText(i, 4);
			sMapNode.strResourceServerShareName = m_lcMap.GetItemText(i, 5);
			sMapNode.strLocalDiskName           = m_lcMap.GetItemText(i, 6);

			if (m_lcMap.GetItemText(i, 7) == "映射成功")
			{
				bool bRet = g_oPxMapManager.upmap(&sMapNode);

				if (bRet)
				{
					m_lcMap.DeleteItem(i);
				}
			}
			else
			{
				m_lcMap.DeleteItem(i);
			}
			
			Sleep(100);
		}
	}

	RefreshMapDiskName();

	m_btnAddMapping.EnableWindow(TRUE);
	m_btnDelMapping.EnableWindow(TRUE);
	m_btnMapByHand.EnableWindow(TRUE);

	UpdateData(FALSE);

	SaveSetting2ConfFile();
}

void CPxNetMappingDlg::OnMapByHand()
{	
	m_btnAddMapping.EnableWindow(FALSE);
	m_btnDelMapping.EnableWindow(FALSE);
	m_btnMapByHand.EnableWindow(FALSE);

	EmptyDirectory(g_strMapResultSavePath);
	SPxMapNode  sMapNode;
	EPxMapState eMapState;
	int nMapDiskCnt = m_lcMap.GetItemCount();
	for (int i = 0; i < nMapDiskCnt; i++)
	{
		if(m_lcMap.GetCheck(i)) // 如果被选中
		{
			sMapNode.strResourceServerIP        = m_lcMap.GetItemText(i, 1);
			sMapNode.strResourceUserName        = m_lcMap.GetItemText(i, 2);
			sMapNode.strResourcePassword        = m_lcMap.GetItemText(i, 3);
			sMapNode.strResourceServerShareDir  = m_lcMap.GetItemText(i, 4);
			sMapNode.strResourceServerShareName = m_lcMap.GetItemText(i, 5);
			sMapNode.strLocalDiskName           = m_lcMap.GetItemText(i, 6);

			eMapState = g_oPxMapManager.GetMapState(&sMapNode);
			Sleep(100);
			if (eMapState == kePxMapState_ExistAndOK)
			{
				m_lcMap.SetItemText(i, 7, "映射成功");
				m_lcMap.SetItemTextColor(i, 7, RGB(0,0,0));
				m_lcMap.SetItemBkColor(i, 7, RGB(0,255,0));

				continue;
			}
			else
			{
				g_oPxMapManager.map(&sMapNode);	
				Sleep(100);
				eMapState = g_oPxMapManager.GetMapState(&sMapNode);
				if (eMapState == kePxMapState_ExistAndOK)
				{
					m_lcMap.SetItemText(i, 7, "映射成功");
					m_lcMap.SetItemTextColor(i, 7, RGB(0,0,0));
					m_lcMap.SetItemBkColor(i, 7, RGB(0,255,0));
				}
				else
				{
					m_lcMap.SetItemText(i, 7, "映射失败");
					m_lcMap.SetItemTextColor(i, 7, RGB(255,255,255));
					m_lcMap.SetItemBkColor(i, 7, RGB(255,0,0));

					ShowErrorMessage(&sMapNode);
				}
			}

			Sleep(100);
		}
	}

	RefreshMapDiskName();

	m_btnAddMapping.EnableWindow(TRUE);
	m_btnDelMapping.EnableWindow(TRUE);
	m_btnMapByHand.EnableWindow(TRUE);

	UpdateData(FALSE);
	//AfxMessageBox("手动映射结束");
}

void CPxNetMappingDlg::RefreshMapDiskName()
{
	UpdateData();

	vector <CString> vstrExistDiskName;

	//g_oPxMapManager.GetLocalDiskList(vstrExistDiskName);

	int nExistMapDiskCnt = m_lcMap.GetItemCount();
	for (int i = 0; i < nExistMapDiskCnt; i++)
	{
		/*bool bIsExist = false;
		for (int j = 0; j < vstrExistDiskName.size(); j++)
		{
			if (vstrExistDiskName[j] == m_lcMap.GetItemText(i, 6))
			{
				bIsExist = true;
				break;
			}
		}*/

		/*if (!bIsExist)
		{*/
			vstrExistDiskName.push_back(m_lcMap.GetItemText(i, 6));
		/*}*/
	}

	m_cbMapDisk.ResetContent();
	for (char iDiskName = 'Z'; iDiskName >= 'H'; iDiskName--)
	{
		bool bDiskNameExist = false;
		CString strDiskName;
		strDiskName.Format("%c:", iDiskName);

		for (int i = 0; i < nExistMapDiskCnt; i++)
		{
			if (vstrExistDiskName[i] == strDiskName)
			{
				bDiskNameExist = true;
				break;
			}
		}

		if (!bDiskNameExist)
		{
			m_cbMapDisk.AddString(strDiskName);
		}
	}

	m_cbMapDisk.SetCurSel(0);
	UpdateData(FALSE);
}


void CPxNetMappingDlg::OnCbnSelchangeComboIsHasPassword()
{
	UpdateData();

	CString strPasswordSwitch;
	m_cbPasswordSwitch.GetLBText(m_cbPasswordSwitch.GetCurSel(), strPasswordSwitch);

	if (strPasswordSwitch == "是")
	{
		//((CStatic *)GetDlgItem(IDC_STATIC_USERNAME))->ShowWindow(SW_SHOWNORMAL);
		((CStatic *)GetDlgItem(IDC_STATIC_PASSWORD))->ShowWindow(SW_SHOWNORMAL);

		//((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_USERNAME))->ShowWindow(SW_SHOWNORMAL);
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_PASSWORD))->ShowWindow(SW_SHOWNORMAL);
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_PASSWORD))->SetFocus();
	}
	else if (strPasswordSwitch == "否")
	{
		//((CStatic *)GetDlgItem(IDC_STATIC_USERNAME))->ShowWindow(SW_HIDE);
		((CStatic *)GetDlgItem(IDC_STATIC_PASSWORD))->ShowWindow(SW_HIDE);

		//((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_USERNAME))->ShowWindow(SW_HIDE);
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_PASSWORD))->ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);
}

CPxNetMappingDlg::~CPxNetMappingDlg()
{
	
}

bool CPxNetMappingDlg::SaveSetting2ConfFile()
{
	UpdateData();
	CString strContent("");
	CString strLine("");

	strContent += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

	strContent += "<Setting>\n";

	// Map setting.
	strContent += "\t<Map>\n";
	int nMapSize = m_lcMap.GetItemCount();

	for (int i = 0; i < nMapSize; i++)
	{
		strContent += "\t\t<MapNode>\n";

		strLine = "\t\t\t<Resource_Server_IP>" + m_lcMap.GetItemText(i, 1) + "</Resource_Server_IP>\n";
		strContent += strLine;

		strLine = "\t\t\t<Resource_Server_UserName>" + m_lcMap.GetItemText(i, 2) + "</Resource_Server_UserName>\n";
		strContent += strLine;

		strLine = "\t\t\t<Resource_Server_Password>" + m_lcMap.GetItemText(i, 3) + "</Resource_Server_Password>\n";
		strContent += strLine;

		strLine = "\t\t\t<Resource_Server_Share_Dir>" + m_lcMap.GetItemText(i, 4) + "</Resource_Server_Share_Dir>\n";
		strContent += strLine;

		strLine = "\t\t\t<Resource_Server_Share_Name>" + m_lcMap.GetItemText(i, 5) + "</Resource_Server_Share_Name>\n";
		strContent += strLine;

		strLine = "\t\t\t<Local_Disk_Name>" + m_lcMap.GetItemText(i, 6) + "</Local_Disk_Name>\n";
		strContent += strLine;

		strContent += "\t\t</MapNode>\n";
	}
	strContent += "\t</Map>\n";

	strContent += "</Setting>\n";

	bool bRet = CMarkup::WriteTextFile(g_strConfFileName, strContent);

	UpdateData(FALSE);

	return bRet;
}


void CPxNetMappingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}


void CPxNetMappingDlg::OnClose()
{
	if (IDYES == ::MessageBox(NULL, "是否退出网络映射设置界面?", "信息确认", MB_YESNO))
	{
		/*if (IDYES == ::MessageBox(NULL, "是否保存当前设置?", "信息确认", MB_YESNO))
		{*/
			SaveSetting2ConfFile();
		/*}*/

		CDialogEx::OnClose();
	}
	else
	{
		return ;
	}
}


void CPxNetMappingDlg::OnSelectOppo()
{
	for (int i = 0; i < m_lcMap.GetItemCount(); i++)
	{
		if (m_lcMap.GetCheck(i) == TRUE)
		{
			m_lcMap.SetCheck(i, FALSE);//选中该Check Box
		}
		else
		{
			m_lcMap.SetCheck(i, TRUE);//选中该Check Box
		}
	}

	BOOL bFlag = TRUE; // 是否为全满
	for (int i = 0; i < m_lcMap.GetItemCount(); i++)
	{
		if (m_lcMap.GetCheck(i) != TRUE)
		{
			bFlag = FALSE;
		}
	}

	if (!bFlag)
	{
		m_checkMainSelAll.SetCheck(FALSE);
	}
}


void CPxNetMappingDlg::OnSelectAll()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_SELECT_ALL))
	{
		for (int i = 0; i < m_lcMap.GetItemCount(); i++)
		{
			m_lcMap.SetCheck(i, TRUE);//选中该Check Box
		}
	}
	else if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_SELECT_ALL))
	{
		for (int i = 0; i < m_lcMap.GetItemCount(); i++)
		{
			m_lcMap.SetCheck(i, FALSE);//取消选中该Check Box
		}
	}

	m_checkMainSelOppo.SetCheck(FALSE);
}
