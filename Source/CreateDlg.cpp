// CreateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DesktopMapper.h"
#include "CreateDlg.h"
#include "afxdialogex.h"


// CCreateDlg �Ի���

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


// CCreateDlg ��Ϣ�������

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
	//����ַ�������
	if (str.Find('/', 0) == -1 && str.Find('\\', 0) == -1) {
		if (!str.IsEmpty()) {
			int n = ::CreateDirectory(L"./Projects/" + str, NULL);
			if (n != 0) ::CreateFile(L"./Projects/" + str + "/Project.pjt", 0, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			else MessageBox(L"����ͼ��ʧ��!", L"����", MB_OK | MB_ICONERROR);
		}
	}
	CDialog::OnOK();
}
