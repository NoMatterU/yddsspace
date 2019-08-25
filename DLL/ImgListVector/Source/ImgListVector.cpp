// ImgListVector.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ImgListVector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ImgListVectorImp::ImgListVectorImp()
{
	this->ListIndex = 0;
//	memset(m_List, 0, sizeof(Path2Index) * MAXLISTIMG);
}

ImgListVectorImp::~ImgListVectorImp()
{
}

int ImgListVectorImp::FindIndexByName(CString ImgName) {
	for (int i = 0; i < ListIndex; i++) {
		if (m_List[i].ImgName == ImgName) {
			return i;
		}
	}
	return -1;
}

bool ImgListVectorImp::Add(CString ImgName, int ImgListIndex) {
	if (ListIndex >= MAXLISTIMG) return false;

	for (int i = 0; i < ListIndex; i++) {
		if (m_List[i].ImgName == ImgName) {
			m_List[i].Count++;
			return false;
		}
	}

	m_List[ListIndex].Count = 1;
	m_List[ListIndex].ImgListIndex = ImgListIndex;
	m_List[ListIndex].ImgName = ImgName;

	ListIndex++;
	return true;
}

bool ImgListVectorImp::Delete(CString ImgName) {
	int index = -1;
	for (int i = 0; i < ListIndex; i++) {
		if (m_List[i].ImgName == ImgName) {
			index = i;
			break;
		}
	}

	if (ListIndex > 0 && ListIndex < MAXLISTIMG) {
		if (index >= 0 && index < ListIndex) {
			if (m_List[index].Count > 0) m_List[index].Count--;
			if (m_List[index].Count == 0) {
				for (int i = index; i < ListIndex; i++) {
					m_List[i] = m_List[i + 1];
				}

				m_List[ListIndex].Count = 0;
				m_List[ListIndex].ImgListIndex = 0;
				m_List[ListIndex].ImgName = "";

				ListIndex--;
				return true;
			}
		}
	}
	return false;
}

ImgListVector *NewImgListVector() {
	ImgListVector *ptr = new ImgListVectorImp;
	return ptr;
}

void FreeImgListVector(ImgListVector *VectorPtr) {
	delete VectorPtr;
}

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 初始化 MFC 并在失败时显示错误
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 更改错误代码以符合您的需要
            wprintf(L"错误: MFC 初始化失败\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 在此处为应用程序的行为编写代码。
        }
    }
    else
    {
        // TODO: 更改错误代码以符合您的需要
        wprintf(L"错误: GetModuleHandle 失败\n");
        nRetCode = 1;
    }

    return nRetCode;
}
