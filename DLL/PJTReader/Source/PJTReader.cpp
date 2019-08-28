// PJTReader.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "PJTReader.h"

#pragma comment(lib, "ImgListVector.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPJTReader::CPJTReader() {
	this->IsOpen = FALSE;
}


CPJTReader::CPJTReader(CString FilePath, CListCtrl *pListCtrl, CImageList *pImgList, ImgListVector *pImgVector) {
	try {
		IsOpen = m_file.Open(FilePath, CFile::modeRead);
	}
	catch (CFileException *e) {
		e->ReportError();
		e->Delete();
	}
	this->m_pListCtrl = pListCtrl;
	this->m_pImgList = pImgList;
	this->m_pImgVector = pImgVector;
}

CPJTReader::~CPJTReader() {
	if(IsOpen) m_file.Close();
}

void CPJTReader::Init() {
	if (IsOpen) {
		m_file.Read(&m_X, sizeof(int));
		m_file.Read(&m_Y, sizeof(int));
		m_file.Read(&m_ImgWidth, sizeof(int));
		m_file.Read(&m_ImgHeight, sizeof(int));
		m_file.Read(&m_ItemCount, sizeof(int));
	}
}

BOOL CPJTReader::InitDlgInfo(CEdit * pEditX, CEdit * pEditY, CEdit * pEditH, CEdit * pEditW)
{
	if (pEditX && pEditY && pEditH && pEditW) {

		if (IsOpen) {
			CString str;

			str.Format(L"%d bp", m_X);
			pEditX->SetWindowTextW(str);

			str.Format(L"%d bp", m_Y);
			pEditY->SetWindowTextW(str);

			str.Format(L"%d bp", m_ImgWidth);
			pEditH->SetWindowTextW(str);

			str.Format(L"%d bp", m_ImgHeight);
			pEditW->SetWindowTextW(str);

			return TRUE;
		}

	}
	return FALSE;
}

int CPJTReader::InitListInfo(CString PjtName) {
	if (IsOpen) {

		if (m_ItemCount < 0) return -1;

		CString str = L"./Projects/";
		str = str + PjtName + '/';

		for (int i = 0; i < m_ItemCount; i++) {
			unsigned char num = 0;
			short LastTime = 0;

			memset(m_ImgPath, 0, MAX_PATH);
			m_file.Read(&num, 1);
			m_file.Read(m_ImgPath, num);

			m_file.Read(&LastTime, 2);

			InsertListItem(str + ByteToWchar(m_ImgPath), i, LastTime);
		}
	}
	return m_ItemCount;
}


BOOL CPJTReader::InsertListItem(CString ImgPath, int nItem, short LastTime) {
	CString str;
	HBITMAP hBmp = NULL;
	Bitmap bmp(ImgPath);

	str = ImgPath.Right(ImgPath.GetLength() - ImgPath.ReverseFind('/') - 1);

	Bitmap *pThumbnail = (Bitmap *)bmp.GetThumbnailImage(50, 50, NULL, NULL); //�趨����ͼ�Ĵ�С
	pThumbnail->GetHBITMAP(Color(255, 255, 255), &hBmp);
	CBitmap *pImage = CBitmap::FromHandle(hBmp);         //ת����CBitmap��ʽλͼ
														 //	if (!Cbmp.Attach((HBITMAP)LoadImageW(AfxGetInstanceHandle(), str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE))) return;
	if (hBmp == NULL) return FALSE;

	int num = m_pImgVector->FindIndexByName(ImgPath);

	if (num <= -1) num = m_pImgList->Add(pImage, RGB(255, 255, 255));

	m_pImgVector->Add(str, num);

	m_pListCtrl->InsertItem(nItem, TEXT(""), num);

	m_pListCtrl->SetItemText(nItem, 2, str);

	str.Format(TEXT("%d bp X %d bp"), bmp.GetHeight(), bmp.GetWidth());
	m_pListCtrl->SetItemText(nItem, 3, str);

	str.Format(TEXT("%d"), nItem + 1);
	m_pListCtrl->SetItemText(nItem, 1, str);

	str.Format(L"%d ms", LastTime);
	m_pListCtrl->SetItemText(nItem, 4, str);

	return TRUE;
}

CString CPJTReader::ByteToWchar(char * pstr)
{
	if (pstr == NULL) return NULL;
	WCHAR arr[MAX_PATH] = { 0 };
	DWORD dwNum = MultiByteToWideChar(CP_ACP, NULL, pstr, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, NULL, pstr, -1, arr, dwNum);
	return arr;
}

// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // ��ʼ�� MFC ����ʧ��ʱ��ʾ����
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: ���Ĵ�������Է���������Ҫ
            wprintf(L"����: MFC ��ʼ��ʧ��\n");
            nRetCode = 1;
        }
        else
        {

		}
    }
    else
    {
        // TODO: ���Ĵ�������Է���������Ҫ
        wprintf(L"����: GetModuleHandle ʧ��\n");
        nRetCode = 1;
    }

    return nRetCode;
}
