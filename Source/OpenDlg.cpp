// OpenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopMapper.h"
#include "DesktopMapperDlg.h"
#include "OpenDlg.h"
#include "afxdialogex.h"
#include "PJTReader.h"
#include "PJTWriter.h"

#pragma comment(lib, "Lib/PJTReader.lib")
// COpenDlg 对话框

IMPLEMENT_DYNAMIC(COpenDlg, CDialogEx)

COpenDlg::COpenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENFILE, pParent)
{

}

COpenDlg::~COpenDlg()
{
}

void COpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COpenDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &COpenDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COpenDlg 消息处理程序


BOOL COpenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	FindAllProject(L"./Projects");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void COpenDlg::FindAllProject(CString strPath)
{
	WIN32_FIND_DATA findData;

	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO1);

	if (pCombo) {
		CString strTemp = strPath;
		strTemp.Format(_T("%s\\*.*"), strPath);//查找指定目录下的直接的所有文件和目录

		HANDLE hFile = FindFirstFile(strTemp, &findData);
		while (hFile != INVALID_HANDLE_VALUE)
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//如果是目录
			{
				if (findData.cFileName[0] != _T('.'))//排除.和..文件夹
				{

//					strTemp = strPath + CString("\\") + findData.cFileName;//获得完整路径
					pCombo->AddString(findData.cFileName);
				}
			}

			if (!FindNextFile(hFile, &findData))
			{
				break;
			}
		}
	}

}


void COpenDlg::OnBnClickedOk()
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_COMBO1);

	if (pCombo) {
		CString str;
		pCombo->GetWindowTextW(str);

		if (!str.IsEmpty()) {
			CMFCApplication1Dlg *pDlg = (CMFCApplication1Dlg *)GetParent();
			CMyListCtrl *pListCtrl = pDlg->GetpListCtrl();
			if (pListCtrl == NULL) return;

			if (!pDlg->GetPjtName().IsEmpty()) {
				CPJTWriter writer(pListCtrl);

				writer.GetX(pDlg->GetEditX());
				writer.GetY(pDlg->GetEditY());
				writer.GetImgHeight(pDlg->GetEditH());
				writer.GetImgWidth(pDlg->GetEditW());

				writer.ToPJTFile(CString("./Projects/") + pDlg->GetPjtName() + L"/Project.pjt");
			}

			CImageList *pImgList = pListCtrl->GetpImgList();
			ImgListVector *pImgVector = pListCtrl->GetpImgVector();

			if (pDlg->GetPjtName() != str) {

				pDlg->SetPjtName(str);

				int ImgCount = pImgList->GetImageCount();
				for (int i = 0; i < ImgCount; i++) pImgList->Remove(0);
				pListCtrl->DeleteAllItems();

				FreeImgListVector(pImgVector);
				pListCtrl->SetpImgVector(NewImgListVector());
				
				pListCtrl->SetListCount(0);

				CPJTReader reader(CString("./Projects/") + str + L"/Project.pjt", pListCtrl, pImgList, pImgVector);

				if (!reader.InitDlgInfo(pDlg->GetEditX(), pDlg->GetEditY(),
					pDlg->GetEditH(), pDlg->GetEditW())) return;

				int ItemCount = reader.InitListInfo(str);
				if (ItemCount >= 0) pListCtrl->SetListCount(ItemCount);
				else return;

			}
//			else {
//				AfxMessageBox(L"打开同一个图案", MB_OK);
//			}
		}
	}

	CDialogEx::OnOK();
}
