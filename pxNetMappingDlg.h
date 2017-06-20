
// pxNetMappingDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "pxCommonDef.h"
#include <vector>
using namespace std;

// CpxNetMappingDlg �Ի���
class CPxNetMappingDlg : public CDialogEx
{
// ����
public:
	CPxNetMappingDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CPxNetMappingDlg();

// �Ի�������
	enum { IDD = IDD_PXNETMAPPING_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CIPAddressCtrl m_ipaddrResourceHostIP;
	//CListCtrl m_lcMapping;
	CReportCtrl m_lcMap;
	CString m_strResourceHostUserName;
	CString m_strResourceHostPassword;
	CString m_strResourceHostShareDir;
	CString m_strResourceHostShareName;
	afx_msg void OnAddMapping();
	afx_msg void OnDeleteMapping();
	bool Parse( const CString in_kstrConfFileName, vector <SPxMapNode> &out_vsMapNode );
	void CreateControls();
	CComboBox m_cbPasswordSwitch;
	CComboBox m_cbMapDisk;
	void Init();
	afx_msg void OnMapByHand();
	void RefreshMapDiskName();
	afx_msg void OnCbnSelchangeComboIsHasPassword();
	void CreateEssentialFolder();
	bool SaveSetting2ConfFile();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnSelectOppo();
	afx_msg void OnSelectAll();
	void ShowErrorMessage(const SPxMapNode *in_kpsPxMapnode);
	CButton m_checkMainSelOppo;
	CButton m_checkMainSelAll;
	CButton m_btnAddMapping;
	CButton m_btnDelMapping;
	CButton m_btnMapByHand;
};
