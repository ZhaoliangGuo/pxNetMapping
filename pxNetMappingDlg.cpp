
// pxNetMappingDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CpxNetMappingDlg �Ի���




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


// CpxNetMappingDlg ��Ϣ�������

BOOL CPxNetMappingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	CreateEssentialFolder(); // ����������ļ���
	CreateControls();        // �����ؼ�
	Init();                  // ��ʼ���ؼ�

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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPxNetMappingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	m_lcMap.SetTextColor(RGB(255,255,255)); //��ʾ�������ɫ

	// ���ø��е����Ƽ���ȵ�
	m_lcMap.InsertColumn(0,_T("ѡ��"),LVCFMT_CENTER,40,-1);
	m_lcMap.InsertColumn(1,_T("��Դ������IP��ַ"),LVCFMT_CENTER,120,-1);
	m_lcMap.InsertColumn(2,_T("�û���"), LVCFMT_LEFT, 100,-1);
	m_lcMap.InsertColumn(3,_T("����"), LVCFMT_LEFT, 50,-1);
	m_lcMap.InsertColumn(4,_T("��Դ����������Ŀ¼"), LVCFMT_LEFT,120,-1);
	m_lcMap.InsertColumn(5,_T("������"), LVCFMT_LEFT,100,-1);
	m_lcMap.InsertColumn(6,_T("����ӳ���̷�"), LVCFMT_CENTER,100,-1);
	m_lcMap.InsertColumn(7,_T("״̬"), LVCFMT_CENTER,88,-1);

	/*CMFCButton *pAddButton = new CMFCButton; 
	pAddButton->Create(_T("���"), WS_VISIBLE, CRect(217,83,54,19), this, IDC_BUTTON_ADD_MAPPING); 

	CMFCButton * pAddButton = (CMFCButton *)GetDlgItem(IDC_BUTTON_ADD_MAPPING);
	pAddButton->SetTextColor(RGB(255, 255, 255));

	// �������� 
	pAddButton->SetWindowText("���ӳ��"); 
	// ���ı�����ɫ 
	pAddButton->SetFaceColor(RGB(153, 217, 234)); 
	// ����������ɫ 
	pAddButton->SetTextColor(RGB(255, 255, 255)); 
	// ���ĸ�����ɫ 
	pAddButton->SetTextHotColor(RGB(63, 72, 204)); 
	// ����λ�� 
	//button->MoveWindow(50, 50, 100, 70); 
	// ��꾭��ʱ������� 
	pAddButton->SetMouseCursorHand(); 
	// ������ʾ 
	pAddButton->SetTooltip(_T("��ʾ")); */ 
}

void CPxNetMappingDlg::Init()
{
	UpdateData();

	// Password switch.
	m_cbPasswordSwitch.ResetContent();
	m_cbPasswordSwitch.AddString("��");
	m_cbPasswordSwitch.AddString("��");
	m_cbPasswordSwitch.SelectString(0, "��");

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
		m_lcMap.SetItemText(nMaxIndex, 7, "ӳ����...");
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
			m_lcMap.SetItemText(nMaxIndex, 7, "ӳ��ɹ�");
			m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
			m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,255,0));
		}
		else
		{
			m_lcMap.SetItemText(nMaxIndex, 7, "ӳ��ʧ��");
			m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(255,255,255));
			m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(255,0,0));
		}*/

		eMapState = g_oPxMapManager.GetMapState(&vsMapNode[i]);

		if (eMapState == kePxMapState_ExistAndOK)
		{
			m_lcMap.SetItemText(nMaxIndex, 7, "ӳ��ɹ�");
			m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
			m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,255,0));
		}
		else
		{
			g_oPxMapManager.map(&vsMapNode[i]);
			eMapState = g_oPxMapManager.GetMapState(&vsMapNode[i]);

			if (eMapState == kePxMapState_ExistAndOK)
			{
				m_lcMap.SetItemText(nMaxIndex, 7, "ӳ��ɹ�");
				m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
				m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,255,0));
			}
			else
			{
				m_lcMap.SetItemText(nMaxIndex, 7, "ӳ��ʧ��");
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
	// ��ʽת��
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

	// �����ļ�
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
		AfxMessageBox("��Դ������IP��ַ����Ϊ�գ���ȷ��.");
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
		AfxMessageBox("��Դ���������û�������Ϊ�գ���ȷ��.");
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_USERNAME))->SetFocus();
		return ;
	}

	sMapNode.strResourceUserName = m_strResourceHostUserName;

	if (strPasswordSwitch == "��")
	{
		if (m_strResourceHostPassword.IsEmpty())
		{
			AfxMessageBox("��Դ�����������벻��Ϊ�գ���ȷ��.");
			((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_PASSWORD))->SetFocus();
			return ;
		}

		sMapNode.strResourcePassword = m_strResourceHostPassword;
	}
	else if (strPasswordSwitch == "��")
	{
		sMapNode.strResourcePassword = "";
	}

	// RB Server share dir.
	if (m_strResourceHostShareDir.IsEmpty())
	{
		AfxMessageBox("��Դ�������Ĺ���Ŀ¼����Ϊ�գ���ȷ��.");
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_SHARE_DIR))->SetFocus();
		return ;
	}

	sMapNode.strResourceServerShareDir = m_strResourceHostShareDir;

	// RB Server share name.

	if (m_strResourceHostShareName.IsEmpty())
	{
		AfxMessageBox("��Դ�������Ĺ���������Ϊ�գ���ȷ��.");
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
		CString strMsg = "������ " + sMapNode.strResourceServerIP + " �ϵĹ�����Ϊ ";
		strMsg += sMapNode.strResourceServerShareName;
		strMsg += " ��ӳ���Ѿ�����";
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

	m_lcMap.SetItemText(nMaxIndex, 7, "ӳ����...");
	m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
	m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,0,255));

	EPxMapState eMapState = g_oPxMapManager.GetMapState(&sMapNode);
	Sleep(50);
	if (eMapState == kePxMapState_ExistAndOK)
	{
		m_lcMap.SetItemText(nMaxIndex, 7, "ӳ��ɹ�");
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
			m_lcMap.SetItemText(nMaxIndex, 7, "ӳ��ɹ�");
			m_lcMap.SetItemTextColor(nMaxIndex, 7, RGB(0,0,0));
			m_lcMap.SetItemBkColor(nMaxIndex, 7, RGB(0,255,0));
		}
		else
		{
			m_lcMap.SetItemText(nMaxIndex, 7, "ӳ��ʧ��");
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
		srcFile.WriteString("@echo ��ǰӳ������޷��������, ������ʾ����(���Ժ�):\n ");
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

			if (m_lcMap.GetItemText(i, 7) == "ӳ��ɹ�")
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
		if(m_lcMap.GetCheck(i)) // �����ѡ��
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
				m_lcMap.SetItemText(i, 7, "ӳ��ɹ�");
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
					m_lcMap.SetItemText(i, 7, "ӳ��ɹ�");
					m_lcMap.SetItemTextColor(i, 7, RGB(0,0,0));
					m_lcMap.SetItemBkColor(i, 7, RGB(0,255,0));
				}
				else
				{
					m_lcMap.SetItemText(i, 7, "ӳ��ʧ��");
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
	//AfxMessageBox("�ֶ�ӳ�����");
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

	if (strPasswordSwitch == "��")
	{
		//((CStatic *)GetDlgItem(IDC_STATIC_USERNAME))->ShowWindow(SW_SHOWNORMAL);
		((CStatic *)GetDlgItem(IDC_STATIC_PASSWORD))->ShowWindow(SW_SHOWNORMAL);

		//((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_USERNAME))->ShowWindow(SW_SHOWNORMAL);
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_PASSWORD))->ShowWindow(SW_SHOWNORMAL);
		((CEdit *)GetDlgItem(IDC_EDIT__RESOURCE_HOST_PASSWORD))->SetFocus();
	}
	else if (strPasswordSwitch == "��")
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
	if (IDYES == ::MessageBox(NULL, "�Ƿ��˳�����ӳ�����ý���?", "��Ϣȷ��", MB_YESNO))
	{
		/*if (IDYES == ::MessageBox(NULL, "�Ƿ񱣴浱ǰ����?", "��Ϣȷ��", MB_YESNO))
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
			m_lcMap.SetCheck(i, FALSE);//ѡ�и�Check Box
		}
		else
		{
			m_lcMap.SetCheck(i, TRUE);//ѡ�и�Check Box
		}
	}

	BOOL bFlag = TRUE; // �Ƿ�Ϊȫ��
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
			m_lcMap.SetCheck(i, TRUE);//ѡ�и�Check Box
		}
	}
	else if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_SELECT_ALL))
	{
		for (int i = 0; i < m_lcMap.GetItemCount(); i++)
		{
			m_lcMap.SetCheck(i, FALSE);//ȡ��ѡ�и�Check Box
		}
	}

	m_checkMainSelOppo.SetCheck(FALSE);
}
