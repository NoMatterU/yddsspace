
// MFCApplication1Dlg.cpp : implementation file


#include "stdafx.h"
#include "DesktopMapper.h"
#include "DesktopMapperDlg.h"
#include "OpenDlg.h"
#include "CreateDlg.h"
#include "SaveDlg.h"
#include "afxdialogex.h"
#include "PJTWriter.h"

#pragma comment(lib, "Lib/PJTWriter.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4996)
// CMFCApplication1Dlg dialog


CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_EditStr = "0 bp";
	m_pListCtrl = new CMyListCtrl();
}

CMFCApplication1Dlg::~CMFCApplication1Dlg() {
	delete m_pListCtrl;
	m_pListCtrl = NULL;
}


void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	//编辑框的聚焦和失焦消息映射
	ON_EN_SETFOCUS(IDC_EDIT3, &CMFCApplication1Dlg::OnEnSetfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CMFCApplication1Dlg::OnEnKillfocusEdit3)
	ON_EN_SETFOCUS(IDC_EDIT4, &CMFCApplication1Dlg::OnEnSetfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CMFCApplication1Dlg::OnEnKillfocusEdit4)
	ON_EN_SETFOCUS(IDC_EDIT5, &CMFCApplication1Dlg::OnEnSetfocusEdit5)
	ON_EN_KILLFOCUS(IDC_EDIT5, &CMFCApplication1Dlg::OnEnKillfocusEdit5)
	ON_EN_SETFOCUS(IDC_EDIT6, &CMFCApplication1Dlg::OnEnSetfocusEdit6)
	ON_EN_KILLFOCUS(IDC_EDIT6, &CMFCApplication1Dlg::OnEnKillfocusEdit6)

	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_INSERT, &CMFCApplication1Dlg::OnBnClickedInsert)
	ON_BN_CLICKED(IDC_EXIT, &CMFCApplication1Dlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_START, &CMFCApplication1Dlg::OnBnClickedStart)
	ON_MESSAGE(WM_SHOWTASK, &CMFCApplication1Dlg::OnShowTask)
	ON_MESSAGE(WM_DELLISTITEM, &CMFCApplication1Dlg::OnDelList)
	ON_WM_LBUTTONUP()
	ON_WM_DROPFILES()
	ON_COMMAND(IDM_START_CREATE, &CMFCApplication1Dlg::OnStartCreate)
	ON_COMMAND(IDM_START_OPEN, &CMFCApplication1Dlg::OnStartOpen)
	ON_COMMAND(IDM_START_SAVE, &CMFCApplication1Dlg::OnStartSave)
END_MESSAGE_MAP()


// CMFCApplication1Dlg message handlers

BOOL CMFCApplication1Dlg::SetReslutionRate() {
	CString str;

	int Width = GetSystemMetrics(SM_CXSCREEN);
	int Height = GetSystemMetrics(SM_CYSCREEN);

	str.Format(TEXT("%d"), Height);
	str += " bp";
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT1);
	pEdit->SetWindowTextW(str);
	pEdit->EnableWindow(FALSE);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT5);
	pEdit->SetWindowTextW(str);

	str.Format(TEXT("%d"), Width);
	str += " bp";
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT2);
	pEdit->SetWindowTextW(str);
	pEdit->EnableWindow(FALSE);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT6);
	pEdit->SetWindowTextW(str);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT7);
	str.Format(L"1 : %.1f", (FLOAT)Width / Height);
	pEdit->SetWindowTextW(str);
	pEdit->EnableWindow(FALSE);
	return TRUE;
}


BOOL CMFCApplication1Dlg::SetTitleText() {
	CString str = TEXT("DesktopMapper   当前Windows系统 ：");
	BOOL bRet = FALSE;

	if (IsWindows8OrGreater()) {

		DWORD dwMajorVer = 0, dwMinorVer = 0, dwBuildNumber = 0;
		HMODULE hModNtdll = NULL;
		if (hModNtdll = ::LoadLibraryA("ntdll.dll"))
		{
			typedef void (WINAPI *pfRTLGETNTVERSIONNUMBERS)(DWORD*, DWORD*, DWORD*);
			pfRTLGETNTVERSIONNUMBERS pfRtlGetNtVersionNumbers;
			pfRtlGetNtVersionNumbers = (pfRTLGETNTVERSIONNUMBERS)::GetProcAddress(hModNtdll, "RtlGetNtVersionNumbers");
			if (pfRtlGetNtVersionNumbers)
			{
				pfRtlGetNtVersionNumbers(&dwMajorVer, &dwMinorVer, &dwBuildNumber);
				dwBuildNumber &= 0xffff0000;
				bRet = TRUE;
			}
			if (dwMajorVer == 6 && dwMinorVer == 3) {
				if (IsWindowsServer()) {
					str += "Microsoft Windows Server 2012 R2";
				}
				else {
					str += "Microsoft Windows 8.1";
				}
			}
			else if (dwMajorVer == 10 && dwMinorVer == 0) {
				if (IsWindowsServer()) {
					str += "Microsoft Windows Server 2016";
				}
				else {
					str += "Microsoft Windows 10";
				}

				::FreeLibrary(hModNtdll);
				hModNtdll = NULL;
			}
		}

	}
	else {

		SYSTEM_INFO info;
		GetSystemInfo(&info);
		OSVERSIONINFOEX os;
		os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if (GetVersionEx((OSVERSIONINFO *)&os)) {
			//下面根据版本信息判断操作系统名称
		//	str.Format(TEXT("mainVersion: %d secondVersion: %d"), os.dwMinorVersion, os.dwMajorVersion);
			switch (os.dwMajorVersion) {

			case 4:
				switch (os.dwMinorVersion) {
				case 0:
					if (os.dwPlatformId == VER_PLATFORM_WIN32_NT)
						str += _T("Microsoft Windows NT 4.0");
					else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
						str += _T("Microsoft Windows 95");
					break;
				case 10:
					str += _T("Microsoft Windows 98");
					break;
				case 90:
					str += _T("Microsoft Windows Me");
					break;
				}

				break;

			case 5:
				switch (os.dwMinorVersion) {
				case 0:
					str += _T("Microsoft Windows 2000");
					break;
				case 1:
					str += _T("Microsoft Windows XP");
					break;
				case 2:
					if (os.wProductType == VER_NT_WORKSTATION   && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
						str += _T("Microsoft Windows XP Professional x64 Edition");
					}
					else if (GetSystemMetrics(SM_SERVERR2) == 0)
						str += _T("Microsoft Windows Server 2003");
					else if (GetSystemMetrics(SM_SERVERR2) != 0)
						str += _T("Microsoft Windows Server 2003 R2");
					break;
				}

				break;

			case 6:
				switch (os.dwMinorVersion) {
				case 0:
					if (os.wProductType == VER_NT_WORKSTATION)
						str += _T("Microsoft Windows Vista");
					else  str += _T("Microsoft Windows Server 2008");
					break;
				case 1:
					if (os.wProductType == VER_NT_WORKSTATION)
						str += _T("Microsoft Windows 7");
					else  str += _T("Microsoft Windows Server 2008 R2");
					break;
				case 2:
					if (os.wProductType == VER_NT_WORKSTATION) str += _T("Microsoft Windows 8");
					else  str += _T("Microsoft Windows Server 2012");
					break;
				}

				break;

			default:
				str += _T("Unknown Microsoft Windows Version");
				break;
			}
		}
	}

	this->SetWindowTextW(str);
	return bRet;

}
/*
bool CMFCApplication1Dlg::CStringToPchar(CString cstring, string &s)
{
	char ps[200] = { 0 };
	if (ps == NULL) return false;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, cstring.GetBuffer(), -1, NULL, 0, NULL, FALSE);
	WideCharToMultiByte(CP_OEMCP, NULL, cstring.GetBuffer(), -1, ps, dwNum, NULL, FALSE);
	s = ps;
	cstring.ReleaseBuffer();
	return true;
}

bool CMFCApplication1Dlg::PcharToCString(const char *ps, CString &cstring)
{
	if (ps == NULL) return false;
	WCHAR arr[100] = { 0 };
	DWORD dwNum = MultiByteToWideChar(CP_ACP, NULL, ps, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, NULL, ps, -1, arr, dwNum);
	cstring = arr;
	cstring.ReleaseBuffer();
	return true;
}
*/
void CMFCApplication1Dlg::InitList() {

//	m_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
//	m_pListCtrl->Arrange(LVA_ALIGNTOP);
//	m_pImgList->Create(50, 50, ILC_COLOR32 | ILC_MASK, 10, 2);
//	m_pListCtrl->SetImageList(m_pImgList, LVSIL_SMALL | LVSIL_NORMAL);

	RECT rect;
	m_pListCtrl->GetClientRect(&rect);

	m_pListCtrl->InsertColumn(0, TEXT("图片"), LVCFMT_CENTER, 60);
	m_pListCtrl->InsertColumn(1, TEXT("序号"), LVCFMT_CENTER, 60);
	m_pListCtrl->InsertColumn(2, TEXT("文件名"), LVCFMT_CENTER, 120);
	m_pListCtrl->InsertColumn(3, TEXT("分辨率"), LVCFMT_CENTER, 160);
	m_pListCtrl->InsertColumn(4, TEXT("时间间隔"), LVCFMT_CENTER, rect.right - rect.left - 405);

}

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//注册表记录对话框XY坐标
	int X = theApp.GetProfileIntW(L"DialogPositoin", L"SrceenX", -1);
	int Y = theApp.GetProfileIntW(L"DialogPositoin", L"SrceenY", -1);

	if (X == -1 || Y == -1) {
		X = GetSystemMetrics(SM_CXSCREEN) / 2 - DIALOGWIDTH / 2;
		Y = GetSystemMetrics(SM_CYSCREEN) / 2 - DIALOGHEIGTH / 2;
	}

	if (!PathFileExistsA(".//Projects")) CreateDirectoryA(".//Projects", NULL);

	this->MoveWindow(X, Y, DIALOGWIDTH, DIALOGHEIGTH, true);

	m_pGroup = (CEdit *)GetDlgItem(IDC_TITLE);
	m_pGroup->SetWindowTextW(m_PjtName);

	m_pEditX = (CEdit *)GetDlgItem(IDC_EDIT3);
	m_pEditX->SetLimitText(4);

	m_pEditY = (CEdit *)GetDlgItem(IDC_EDIT4);
	m_pEditY->SetLimitText(4);

	m_pEditH = (CEdit *)GetDlgItem(IDC_EDIT5);
	m_pEditH->SetLimitText(4);

	m_pEditW = (CEdit *)GetDlgItem(IDC_EDIT6);
	m_pEditW->SetLimitText(4);


	m_pListCtrl->Create(WS_CHILD | LVS_REPORT, CRect(40, 190, 40 + LISTCTRLWIDTH, 190 + LISTCTRLHEIGHT), this, IDC_LIST);
//	m_pListCtrl->EnableScrollBar(SB_HORZ, ESB_DISABLE_BOTH);
//	m_pListCtrl->ShowScrollBar(SB_VERT, TRUE);
	m_pListCtrl->ShowWindow(true);


	//初始化编辑框值
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT3);
	pEdit->SetWindowTextW(TEXT("0 bp"));
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT4);
	pEdit->SetWindowTextW(TEXT("0 bp"));

	this->SetTitleText();

	this->InitList();

	this->SetReslutionRate();

	m_pListCtrl->SetFocus();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return FALSE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
 // to draw the icon.  For MFC applications using the document/view model,
 // this is automatically done for you by the framework.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

 //The system calls this function to obtain the cursor to display while the user drags
 // the minimized window.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCApplication1Dlg::OnBnClickedOk()
{
	this->SetFocus();
	//	CDialogEx::OnOK();
}


void CMFCApplication1Dlg::OnCancel() {

	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = WM_SHOWTASK;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	lstrcpy(nid.szTip, TEXT("DesktopMapper"));
	Shell_NotifyIcon(NIM_ADD, &nid);

	this->ShowWindow(SW_HIDE);
	//	CDialogEx::OnCancel();
}


BOOL CMFCApplication1Dlg::DestroyWindow() {
	CRect rect;

	GetWindowRect(rect);
	if(!theApp.WriteProfileInt(L"DialogPositoin", L"SrceenX", rect.left))	MessageBox(L"读取注册表失败!", L"错误", MB_OK | MB_ICONERROR);
	if (!theApp.WriteProfileInt(L"DialogPositoin", L"SrceenY", rect.top))	MessageBox(L"读取注册表失败!", L"错误", MB_OK | MB_ICONERROR);

	return CDialog::DestroyWindow();
}


void CMFCApplication1Dlg::OnBnClickedInsert()
{
	CString str;

	if (m_PjtName.IsEmpty()) {
		MessageBox(L"No Project Exits", L"提示", MB_OK | MB_ICONQUESTION);
		return;
	}

	CFileDialog FileDlg(true, NULL, L"*", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK,
		TEXT("JPEG文件(*.jpg)|*.jpg|PNG文件(*.png)|*.png|BMP文件(*.bmp)|*.bmp|所有文件(*.*)|*.*||"), this);
	if (IDOK == FileDlg.DoModal()) {
		POSITION pos = FileDlg.GetStartPosition();
		if (pos == NULL) return;
		
		do {

			str = FileDlg.GetNextPathName(pos);
			DWORD dwAttr = ::GetFileAttributes(str);
			if ((INVALID_FILE_ATTRIBUTES == dwAttr) || (FILE_ATTRIBUTE_DIRECTORY == dwAttr)) {
				MessageBox(TEXT("打开图片文件失败!"), TEXT("错误"), MB_OK | MB_ICONERROR);
				return;
			}

			if (m_pListCtrl->AddListItem(str)) {
				CString FileName = str.Right(str.GetLength() - str.ReverseFind('\\') - 1);
				BOOL iflag = ::CopyFile(str, CString("./Projects/") + m_PjtName + "/" + FileName, FALSE);
				if (!iflag) MessageBox(L"复制文件失败!", L"错误", MB_OK | MB_ICONERROR);
			}
			else {
				MessageBox(L"插入图片失败!", L"错误", MB_OK | MB_ICONERROR);
			}

			if (pos == NULL) str.Empty();
		} while (!str.IsEmpty());
		
	}
	//使标题栏变黑
	this->SendMessage(WM_NCACTIVATE, true, 0);
}


void CMFCApplication1Dlg::OnBnClickedExit()
{
	if (IDOK == MessageBox(TEXT("确定要退出吗?"), TEXT("提示"), MB_ICONINFORMATION | MB_OKCANCEL)) {
		if (!m_PjtName.IsEmpty()) {
			CString str = L"./Projects/" + m_PjtName;
			str += "/Project.pjt";

			CPJTWriter writer(m_pListCtrl);

			writer.GetX((CEdit *)GetDlgItem(IDC_EDIT3));
			writer.GetY((CEdit *)GetDlgItem(IDC_EDIT4));
			writer.GetImgHeight((CEdit *)GetDlgItem(IDC_EDIT5));
			writer.GetImgWidth((CEdit *)GetDlgItem(IDC_EDIT6));

			writer.ToPJTFile(str);

		}
		CDialog::OnCancel();
	}
}

CMyListCtrl * CMFCApplication1Dlg::GetpListCtrl()
{
	return this->m_pListCtrl;
}

CEdit * CMFCApplication1Dlg::GetEditX()
{
	return this->m_pEditX;
}

CEdit * CMFCApplication1Dlg::GetEditY()
{
	return this->m_pEditY;
}

CEdit * CMFCApplication1Dlg::GetEditH()
{
	return this->m_pEditH;
}

CEdit * CMFCApplication1Dlg::GetEditW()
{
	return this->m_pEditW;
}

CString CMFCApplication1Dlg::GetPjtName()
{
	return this->m_PjtName;
}

void CMFCApplication1Dlg::SetPjtName(CString PjtName)
{
	this->m_PjtName = PjtName;
	m_pGroup->SetWindowTextW(PjtName);
}

/*
java并发机制 线程池，JUC下的工具类（阻塞队列）
java集合框架
类加载机制
java gc实现
动态代理
反射
java性能调优

spring

hadoop->spark->storm->flink
*/

//IDC_EDIT3消息
void CMFCApplication1Dlg::OnEnSetfocusEdit3()
{
	m_pEditX->GetWindowTextW(m_EditStr);
	m_pEditX->SetWindowTextW(TEXT(""));
}


void CMFCApplication1Dlg::OnEnKillfocusEdit3()
{
	CString str;

	m_pEditX->GetWindowTextW(str);
	if (str.IsEmpty()) m_pEditX->SetWindowTextW(m_EditStr);
	else m_pEditX->SetWindowTextW(str + " bp");
}


//IDC_EDIT4消息
void CMFCApplication1Dlg::OnEnSetfocusEdit4()
{
	m_pEditY->GetWindowTextW(m_EditStr);
	m_pEditY->SetWindowTextW(TEXT(""));
}


void CMFCApplication1Dlg::OnEnKillfocusEdit4()
{
	CString str;

	m_pEditY->GetWindowTextW(str);
	if (str.IsEmpty()) m_pEditY->SetWindowTextW(m_EditStr);
	else m_pEditY->SetWindowTextW(str + " bp");
}


//IDC_EDIT5消息
void CMFCApplication1Dlg::OnEnSetfocusEdit5()
{
	m_pEditH->GetWindowTextW(m_EditStr);
	m_pEditH->SetWindowTextW(TEXT(""));
}


void CMFCApplication1Dlg::OnEnKillfocusEdit5()
{
	CString str;

	m_pEditH->GetWindowTextW(str);
	if (str.IsEmpty()) m_pEditH->SetWindowTextW(m_EditStr);
	else m_pEditH->SetWindowTextW(str + " bp");
}


//IDC_EDIT6消息
void CMFCApplication1Dlg::OnEnSetfocusEdit6()
{
	m_pEditW->GetWindowTextW(m_EditStr);
	m_pEditW->SetWindowTextW(TEXT(""));
}


void CMFCApplication1Dlg::OnEnKillfocusEdit6()
{
	CString str;

	m_pEditW->GetWindowTextW(str);
	if (str.IsEmpty()) m_pEditW->SetWindowTextW(m_EditStr);
	else m_pEditW->SetWindowTextW(str + " bp");
}


void CMFCApplication1Dlg::OnBnClickedStart()
{
	CString str;
	WCHAR arr[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, arr, MAX_PATH);
	str = arr;

#ifdef DEBUG
	str = str.Left(str.ReverseFind('\\'));
	str = str.Left(str.ReverseFind('\\'));
#endif // DEBUG
#ifndef DEBUG
	str = str.Left(str.ReverseFind('\\'));
#endif

	str += "\\Projects\\";
	str += m_PjtName;
	if(!m_PjtName.IsEmpty()) theApp.WriteProfileStringW(L"Project", L"ImgPath", str);
	else theApp.WriteProfileStringW(L"Project", L"ImgPath", L"");
}

LRESULT CMFCApplication1Dlg::OnShowTask(WPARAM wParam, LPARAM lParam) {
	if (wParam != IDR_MAINFRAME) return 1;
	switch (lParam)
	{
	case WM_LBUTTONDBLCLK:
	{
		NOTIFYICONDATA nid;
		nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
		nid.hWnd = m_hWnd;
		nid.uID = IDR_MAINFRAME;
		nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		nid.uCallbackMessage = WM_SHOWTASK;
		nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

		lstrcpy(nid.szTip, TEXT("DesktopMapper"));
		Shell_NotifyIcon(NIM_DELETE, &nid);
		this->ShowWindow(SW_SHOW);
		/*
		HWND hWnd = FindWindowA("Shell_TrayWnd", NULL);
		if (hWnd) hWnd = FindWindowExA(hWnd, NULL, "TrayNotifyWnd", NULL);
		if (hWnd) hWnd = FindWindowExA(hWnd, NULL, "Button", NULL);
		
		if (hWnd) {
			::SendMessage(hWnd, WM_NCACTIVATE, 0, 0);
			::SendMessage(hWnd, WM_LBUTTONDOWN, 0, 0);
			Sleep(5);
			::SendMessage(hWnd, WM_NCACTIVATE, 1, 0);
			::SendMessage(hWnd, WM_LBUTTONUP, 0, 0);
		}
		*/
		break;
	}
	default:
		break;
	}
	return 0;
}

LRESULT CMFCApplication1Dlg::OnDelList(WPARAM wParam, LPARAM lParam)
{
	int ItemIndex = (int)wParam;

	CString str = m_pListCtrl->GetItemText(ItemIndex, 2);

	if (m_pListCtrl->DeleteListItem(ItemIndex, str)) {
		if(!str.IsEmpty()) if(!::DeleteFile(CString("./Projects/") + m_PjtName + "/" + str)) AfxMessageBox(L"删除文件失败!", MB_OK);
	}

	return NULL;
}


void CMFCApplication1Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	this->SetFocus();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCApplication1Dlg::OnDropFiles(HDROP hDropInfo)
{
	UINT DropCount = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	
	for (UINT i = 0; i < DropCount; i++) {
		WCHAR wcStr[MAX_PATH] = { 0 };
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);
		CString str = wcStr;
		if (m_pListCtrl->AddListItem(str)) {
			BOOL iflag = ::CopyFile(str, CString("./Projects/") + m_PjtName + "/" + str.Right(str.GetLength() - str.ReverseFind('\\') - 1), FALSE);
			if (!iflag) MessageBox(L"复制文件失败!", L"错误", MB_OK);
		}
	}
	DragFinish(hDropInfo);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CMFCApplication1Dlg::OnStartCreate()
{
	CCreateDlg dlg;
	dlg.DoModal();
}


void CMFCApplication1Dlg::OnStartOpen()
{
	COpenDlg dlg;
	dlg.DoModal();
}


void CMFCApplication1Dlg::OnStartSave()
{
	CSaveDlg dlg;
	dlg.DoModal();
}