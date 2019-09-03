
// MFCApplication1Dlg.cpp : implementation file


#include "stdafx.h"
#include "DesktopMapper.h"
#include "DesktopMapperDlg.h"
#include "OpenDlg.h"
#include "CreateDlg.h"
#include "SaveDlg.h"
#include "afxdialogex.h"
#include "PJTWriter.h"
#include <TlHelp32.h>

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
	//初始化编辑框值

	m_DeskWidth = GetSystemMetrics(SM_CXSCREEN);
	m_DeskHeight = GetSystemMetrics(SM_CYSCREEN);

	str.Format(TEXT("%d"), m_DeskHeight);
	str += " bp";
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT1);
	pEdit->SetWindowTextW(str);
	pEdit->EnableWindow(FALSE);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT5);
	pEdit->SetWindowTextW(str);

	str.Format(TEXT("%d"), m_DeskWidth);
	str += " bp";
	pEdit = (CEdit *)GetDlgItem(IDC_EDIT2);
	pEdit->SetWindowTextW(str);
	pEdit->EnableWindow(FALSE);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT6);
	pEdit->SetWindowTextW(str);

	pEdit = (CEdit *)GetDlgItem(IDC_EDIT7);
	str.Format(L"1 : %.1f", (FLOAT)m_DeskWidth / m_DeskHeight);
	pEdit->SetWindowTextW(str);
	pEdit->EnableWindow(FALSE);

	m_pEditX->SetWindowTextW(TEXT("0 bp"));
	m_pEditY->SetWindowTextW(TEXT("0 bp"));
	return TRUE;
}

bool CMFCApplication1Dlg::EnableDebugPriv() {
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;


	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)
		)
	{
		return false;
	}


	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}


	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
		return false;
	}


	return true;
}


DWORD CMFCApplication1Dlg::ProcessNameToId(LPCTSTR lpszProcessName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);


	if (!Process32First(hSnapshot, &pe))
	{
		return 0;
	}


	while (Process32Next(hSnapshot, &pe))
	{
		if (!StrCmpCW(lpszProcessName, pe.szExeFile))
		{
			return pe.th32ProcessID;
		}
	}
	return 0;
}


bool CMFCApplication1Dlg::InjectDLL(char *szDll) {
	// 定义线程体的大小
	const DWORD dwThreadSize = 5 * 1024;
	DWORD dwWriteBytes;
	// 提升进程访问权限
	EnableDebugPriv();
	// 等待输入进程名称，注意大小写匹配
//	char szExeName[MAX_PATH] = { 0 };

	DWORD dwProcessId = ProcessNameToId(L"explorer.exe");
	if (dwProcessId == 0) return false;

	// 根据进程ID得到进程句柄
	HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (!hTargetProcess) return false;

	// 在宿主进程中为线程体开辟一块存储区域
	// 在这里需要注意MEM_COMMIT内存非配类型以及PAGE_EXECUTE_READWRITE内存保护类型
	// 其具体含义请参考MSDN中关于VirtualAllocEx函数的说明
	void* pRemoteThread = VirtualAllocEx(hTargetProcess, 0, dwThreadSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!pRemoteThread) return false;

	// 拷贝注入DLL内容到宿主空间
	if (!WriteProcessMemory(hTargetProcess,
		pRemoteThread,
		(LPVOID)szDll,
		dwThreadSize,
		0)) return false;

	LPVOID pFunc = LoadLibraryA;
	//在宿主进程中创建线程
	HANDLE hRemoteThread = CreateRemoteThread(hTargetProcess,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)pFunc,
		pRemoteThread,
		0,
		&dwWriteBytes);

	if (!hRemoteThread) return false;

	// 等待LoadLibraryA加载完毕
	WaitForSingleObject(hRemoteThread, INFINITE);
	VirtualFreeEx(hTargetProcess, pRemoteThread, dwThreadSize, MEM_COMMIT);

	CloseHandle(hRemoteThread);
	CloseHandle(hTargetProcess);

	return true;
}

bool CMFCApplication1Dlg::DetchDLL()
{
	BOOL bMore = FALSE, bFound = FALSE;
	HANDLE hSnapshot, hProcess, hThread;
	HMODULE hModule = NULL;
	MODULEENTRY32 me = { sizeof(me) };
	LPTHREAD_START_ROUTINE pThreadProc;


	DWORD dwProcessId = ProcessNameToId(L"explorer.exe");
	if (dwProcessId == 0) return false;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	bMore = Module32First(hSnapshot, &me);
	for (; bMore; bMore = Module32Next(hSnapshot, &me))
	{
		if (!_tcsicmp((LPCTSTR)me.szModule, L"Hook.dll") || !_tcsicmp((LPCTSTR)me.szExePath, L"Hook.dll"))
		{
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
	{
		CloseHandle(hSnapshot);
		return false;
	}
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId)))
	{
		return false;
	}
	hModule = GetModuleHandle(L"Kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "FreeLibrary");
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, me.modBaseAddr, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);
	CloseHandle(hSnapshot);
	return true;
}

bool CMFCApplication1Dlg::CheckSuffix(CString FileName)
{
	CString str = FileName.Right(FileName.GetLength() - FileName.ReverseFind('.') - 1);
	if (str == "jpg" || str == "png" || str == "bmp") return true;
	if (str == "JPG" || str == "PNG" || str == "BMP") return true;
	return false;
}

bool CMFCApplication1Dlg::CheckInput()
{
	CString str;

	m_pEditX->GetWindowTextW(str);
	int x = _ttoi(str);
	m_pEditY->GetWindowTextW(str);
	int y = _ttoi(str);
	m_pEditH->GetWindowTextW(str);
	int Height = _ttoi(str);
	m_pEditW->GetWindowTextW(str);
	int Width = _ttoi(str);

	if (x + Width > m_DeskWidth && y + Height > m_DeskHeight) return false;

	return true;
}

bool CMFCApplication1Dlg::TranstToBMP(CString srcPath, CString destPath)
{
	Image img(srcPath);//这里的图片可以是其它格式Graphics 
	CLSID pngClsid;

//	Bitmap Png(srcPath);
//	Bitmap Jpg(Png.GetWidth(), Png.GetHeight());

	Graphics g(&img);

	TextureBrush br(&img);
	g.Clear(0xFFFFFFFF);  // 白色背景
	g.FillRectangles((Brush *)&br, &Rect(0, 0, img.GetWidth(), img.GetHeight()), 1);
//	g.DrawLine(&p, Point(0, 0), Point(20, 20));
	g.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight());

	//这里的图片可以是其它格式，此处转化为BMP格式
	if (!GetImageCLSID(L"image/bmp", &pngClsid)) return false;

	img.Save(destPath, &pngClsid, NULL);

	return true;
}

bool CMFCApplication1Dlg::GetImageCLSID(const WCHAR * format, CLSID * pCLSID)
{
	UINT num = 0, size = 0;
	ImageCodecInfo *pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if (size == 0) return false;
	pImageCodecInfo = (ImageCodecInfo *)malloc(size);
	if (pImageCodecInfo == NULL) return false;
	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT i = 0; i < num; i++) {
		if (wcscmp(pImageCodecInfo->MimeType, format) == 0) {
			*pCLSID = pImageCodecInfo->Clsid;
			free(pImageCodecInfo);
			return true;
		}
	}
	free(pImageCodecInfo);
	return false;
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
				if (!IsWindowsServer()) {
					str += "Microsoft Windows 8.1";
				}
			}
			else if (dwMajorVer == 10 && dwMinorVer == 0) {
				if (!IsWindowsServer()) {
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

			switch (os.dwMajorVersion) {

			case 5:
				switch (os.dwMinorVersion) {
				case 0:
					str += _T("Microsoft Windows 2000");
					break;
				case 1:
					str += _T("Microsoft Windows XP");
					break;
				}

				break;

			case 6:
				switch (os.dwMinorVersion) {

				case 1:
					if (os.wProductType == VER_NT_WORKSTATION)
						str += _T("Microsoft Windows 7");
					break;

				case 2:
					if (os.wProductType == VER_NT_WORKSTATION)
						str += _T("Microsoft Windows 8");
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
		X = (m_DeskWidth - DIALOGWIDTH) / 2;
		Y = (m_DeskHeight - DIALOGHEIGTH) / 2;
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
	theApp.WriteProfileStringW(L"Project", L"ImgPath", L"");

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
		TEXT("所有文件(*.*)|*.*|JPEG文件(*.jpg)|*.jpg|PNG文件(*.png)|*.png|BMP文件(*.bmp)|*.bmp||"), this);
	if (IDOK == FileDlg.DoModal()) {
		POSITION pos = FileDlg.GetStartPosition();
		if (pos == NULL) return;
		
		do {

			str = FileDlg.GetNextPathName(pos);

			CString FileName = str.Right(str.GetLength() - str.ReverseFind('\\') - 1);

			if (!CheckSuffix(FileName)) break;

			DWORD dwAttr = ::GetFileAttributes(str);
			if ((INVALID_FILE_ATTRIBUTES == dwAttr) || (FILE_ATTRIBUTE_DIRECTORY == dwAttr)) {
				MessageBox(TEXT("打开文件 "+FileName+" 失败!"), TEXT("错误"), MB_OK | MB_ICONERROR);
				return;
			}
			//C://User//admin//Desktop//MFCapplication//MFCApplication1///
			if (m_pListCtrl->AddListItem(str)) {
				CString Suffix = FileName.Right(FileName.GetLength() - FileName.ReverseFind('.') - 1);
				if(Suffix == L"bmp" || Suffix == L"BMP")
					::CopyFile(str, L"./Projects/" + m_PjtName + L'/' + FileName, FALSE);
				else
					TranstToBMP(str, L".//Projects//"+m_PjtName+L'/'+ FileName);
//				BOOL iflag = 
////				::CopyFile(str, CString("./Projects/") + m_PjtName + "/" + FileName, FALSE);
//				if (!iflag) MessageBox(L"复制文件失败!", L"错误", MB_OK | MB_ICONERROR);
			}
			else {
				MessageBox(L"插入图片 "+FileName+" 失败", L"错误", MB_OK | MB_ICONERROR);
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

			writer.EditToX(m_pEditX);
			writer.EditToY(m_pEditY);
			writer.EditToImgHeight(m_pEditH);
			writer.EditToImgWidth(m_pEditW);

			writer.ToPJTFile(str);

			if (IsInject) if (!DetchDLL()) MessageBox(L"卸载DLL失败", L"错误", MB_OK | MB_ICONERROR);
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
	else {
		int num = _ttoi(str);
		if (num > m_DeskWidth) str.Format(L"%d", m_DeskWidth);
		m_pEditX->SetWindowTextW(str + " bp");
	}
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
	else {
		int num = _ttoi(str);
		if (num > m_DeskHeight) str.Format(L"%d", m_DeskWidth);
		m_pEditY->SetWindowTextW(str + " bp");
	}
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
	else {
		int num = _ttoi(str);
		if (num > m_DeskHeight) str.Format(L"%d", m_DeskWidth);
		m_pEditH->SetWindowTextW(str + " bp");
	}
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
	else {
		int num = _ttoi(str);
		if (num > m_DeskWidth) str.Format(L"%d", m_DeskWidth);
		m_pEditW->SetWindowTextW(str + " bp");
	}
}


void CMFCApplication1Dlg::OnBnClickedStart()
{
	if (m_PjtName.IsEmpty()) {
		MessageBox(L"No Project Exits", L"提示", MB_OK | MB_ICONQUESTION);
		return;
	}

	CString str;
	WCHAR arr[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, arr, MAX_PATH);
	str = arr;


#ifdef DEBUG
//	str = str.Left(str.ReverseFind('\\'));
	str = str.Left(str.ReverseFind('\\'));
#endif // DEBUG
#ifndef DEBUG
	str = str.Left(str.ReverseFind('\\'));
#endif

	CString MainPath = str;

	str = MainPath + "\\Projects\\" + m_PjtName;

	//x y Width Height 输入检测
	if (!CheckInput()) {
		MessageBox(L"参数范围过大或过小", L"提示", MB_OK | MB_ICONINFORMATION);
		return;
	}

	CPJTWriter writer(m_pListCtrl);

	writer.EditToX(m_pEditX);
	writer.EditToY(m_pEditY);
	writer.EditToImgHeight(m_pEditH);
	writer.EditToImgWidth(m_pEditW);

	if (!writer.ToPJTFile(str + L"\\Project.pjt")) {
		MessageBox(L"保存图案失败", L"错误", MB_OK | MB_ICONERROR);
		return;
	}


	if(!m_PjtName.IsEmpty()) theApp.WriteProfileStringW(L"Project", L"ImgPath", str);
	else theApp.WriteProfileStringW(L"Project", L"ImgPath", L"");

	if (IsInject) {
		if (DetchDLL()) IsInject = FALSE;
		else MessageBox(L"卸载DLL失败!", L"提示", MB_OK | MB_ICONINFORMATION);
	}
	else {
		if (InjectDLL(MainPath + "\\Hook.dll")) {
//			MessageBox(L"注入DLL成功!", L"提示", MB_OK | MB_ICONINFORMATION);
			IsInject = TRUE;
		}
		else {
			MessageBox(L"注入DLL失败!", L"错误", MB_OK | MB_ICONERROR);
		}
	}
/*	HKEY hKey;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\YDDApp\\DesktopMapper\\Project", 0, KEY_READ, &hKey)) {
		DWORD dwValue = 0;
		LPBYTE ptr = NULL;
		RegQueryValueEx(hKey, L"ImgPath", 0, NULL, ptr, &dwValue);
		ptr = new BYTE[dwValue];
		RegQueryValueEx(hKey, L"ImgPath", 0, NULL, ptr, &dwValue);
		CString str = (WCHAR *)ptr;
		AfxMessageBox(str, MB_OK);
		delete ptr;
		RegCloseKey(hKey);
	}
*/
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
		if (!str.IsEmpty()) if (!::DeleteFile(CString("./Projects/") + m_PjtName + "/" + str)) MessageBox(L"删除 " + str + " 失败!", L"错误", MB_OK | MB_ICONERROR);
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
		CString FileName = str.Right(str.GetLength() - str.ReverseFind('\\') - 1);
		if (m_pListCtrl->AddListItem(str)) {
//			BOOL iflag = 
			::CopyFile(str, CString("./Projects/") + m_PjtName + "/" + FileName, FALSE);
//			if (!iflag) MessageBox(L"复制文件失败!", L"错误", MB_OK);
		}
		else {
			MessageBox(L"插入图片"+FileName+"失败", L"错误", MB_OK | MB_ICONERROR);
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