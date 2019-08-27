// CreateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopMapper.h"
#include "CreateDlg.h"
#include "afxdialogex.h"


// CCreateDlg 对话框

IMPLEMENT_DYNAMIC(CCreateDlg, CDialog)

CCreateDlg::CCreateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CREATEDLG, pParent)
{
}

CCreateDlg::~CCreateDlg()
{
}

void CCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCreateDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCreateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateDlg 消息处理程序

BOOL CCreateDlg::OnInitDialog() {
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_TEXT);
	pEdit->SetLimitText(20);
	return TRUE;
}

void CCreateDlg::OnBnClickedOk()
{
	CString str;
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_TEXT);
	pEdit->GetWindowTextW(str);
	//检查字符串输入
	if (str.Find('/', 0) == -1 && str.Find('\\', 0) == -1) {
		if (!str.IsEmpty()) {
			int n = ::CreateDirectory(L"./Projects/" + str, NULL);
			if (n != 0) ::CreateFile(L"./Projects/" + str + "/Project.pjt", 0, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			else MessageBox(L"创建图案失败!", L"错误", MB_OK | MB_ICONERROR);
		}
	}
	CDialog::OnOK();
}
