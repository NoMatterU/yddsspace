// PJTWriter.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "PJTWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPJTWriter::CPJTWriter() {
	x = 0;
	y = 0;
	ImgHeight = 0;
	ImgWidth = 0;
	m_pListCtrl = NULL;
}

CPJTWriter::CPJTWriter(CListCtrl *pListCtrl) {
	x = 0;
	y = 0;
	ImgHeight = 0;
	ImgWidth = 0;
	this->m_pListCtrl = pListCtrl;
}

CPJTWriter::~CPJTWriter() {};

void CPJTWriter::EditToX(CEdit *pEdit) {
	CString str;
	if (m_pListCtrl) {
//		CEdit *pEdit = (CEdit *)GetDlgItem(m_pListCtrl->GetParent()->GetSafeHwnd(), IDC_EIDT);
		if (pEdit == NULL) return;
		pEdit->GetWindowTextW(str);
		if (!str.IsEmpty()) {
			str = str.Left(str.GetLength() - 3);
			this->x = _ttoi(str);
		}
	}
}

void CPJTWriter::EditToY(CEdit *pEdit) {
	CString str;
	if (m_pListCtrl) {
//		CEdit *pEdit = (CEdit *)GetDlgItem(m_pListCtrl->GetParent()->GetSafeHwnd(), IDC_EIDT);
		if (pEdit) {
			pEdit->GetWindowTextW(str);
			if (!str.IsEmpty()) {
				str = str.Left(str.GetLength() - 3);
				this->y = _ttoi(str);
			}
		}
	}
}

void CPJTWriter::EditToImgHeight(CEdit *pEdit) {
	CString str;
	if (m_pListCtrl) {
//		CEdit *pEdit = (CEdit *)GetDlgItem(m_pListCtrl->GetParent()->GetSafeHwnd(), IDC_EIDT);
		if (pEdit) {
			pEdit->GetWindowTextW(str);
			if (!str.IsEmpty()) {
				str = str.Left(str.GetLength() - 3);
				this->ImgHeight = _ttoi(str);
			}
		}
	}
}

void CPJTWriter::EditToImgWidth(CEdit *pEdit) {
	CString str;
	if (m_pListCtrl) {
//		CEdit *pEdit = (CEdit *)GetDlgItem(m_pListCtrl->GetParent()->GetSafeHwnd(), IDC_EIDT);
		if (pEdit) {
			pEdit->GetWindowTextW(str);
			if (!str.IsEmpty()) {
				str = str.Left(str.GetLength() - 3);
				this->ImgWidth = _ttoi(str);
			}
		}
	}
}

BOOL CPJTWriter::ToPJTFile(CString FilePath) {
	if (m_pListCtrl == NULL) return FALSE;

	try {
		CFile file;
		file.Open(FilePath, CFile::modeWrite | CFile::modeCreate);

		int ItemCount = m_pListCtrl->GetItemCount();

		//20�ֽڹ̶��ļ���Ϣ
		file.Write(&x, sizeof(int));
		file.Write(&y, sizeof(int));
		file.Write(&ImgHeight, sizeof(int));
		file.Write(&ImgWidth, sizeof(int));
		file.Write(&ItemCount, sizeof(int));

		for (int i = 0; i < ItemCount; i++) {
			CString str = m_pListCtrl->GetItemText(i, 2);
			memset(ImgName, 0, MAX_PATH);
			this->WCharToByte(str);

			unsigned char len = str.GetLength();
			if (len > 255) len = 255;
			file.Write(&len, 1);
			file.Write(ImgName, len);

			str = m_pListCtrl->GetItemText(i, 4);
			str = str.Left(str.GetLength() - 3);
			short LastTime = _ttoi(str);
			file.Write(&LastTime, 2);
		}
		file.Close();
	}
	catch (CFileException *e) {
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

void CPJTWriter::WCharToByte(CString str)
{
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str.GetBuffer(), -1, NULL, 0, NULL, FALSE);
	WideCharToMultiByte(CP_OEMCP, NULL, str.GetBuffer(), -1, ImgName, dwNum, NULL, FALSE);
	str.ReleaseBuffer();
}

// Ψһ��Ӧ�ó������

CWinApp theApp;

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
            // TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
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
