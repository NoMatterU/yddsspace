// SaveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DesktopMapper.h"
#include "SaveDlg.h"
#include "afxdialogex.h"


// CSaveDlg �Ի���

IMPLEMENT_DYNAMIC(CSaveDlg, CDialog)

CSaveDlg::CSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SAVEFILE, pParent)
{

}

CSaveDlg::~CSaveDlg()
{
}

void CSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaveDlg, CDialog)
END_MESSAGE_MAP()


// CSaveDlg ��Ϣ�������
