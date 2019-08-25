// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 PJTREADER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// PJTREADER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#include "ImgListVector.h"

#ifdef PJTREADER_EXPORTS
#define PJTREADER_API __declspec(dllexport)
#else
#define PJTREADER_API __declspec(dllimport)
#endif

// 此类是从 PJTReader.dll 导出的
class PJTREADER_API CPJTReader {
private:
	int m_X = 0;
	int m_Y = 0;
	int m_ImgWidth = 0;
	int m_ImgHeight = 0;
	int m_ItemCount = 0;

	int m_PosFlag = 0;
	WCHAR m_ImgPath[256] = { 0 };

	CFile m_file;
	BOOL IsOpen;

	CListCtrl *m_pListCtrl = NULL;
	CImageList *m_pImgList = NULL;
	ImgListVector *m_pImgVector = NULL;

public:
	CPJTReader();

	CPJTReader(CString FilePath, CListCtrl *pListCtrl, CImageList *pImgList, ImgListVector *pImgVector);

	~CPJTReader();

	VOID Init();

	BOOL InitDlgInfo(CEdit *pEdit1, CEdit *pEdit2, CEdit *pEdit3, CEdit *pEdit4);

	int InitListInfo(CString PjtName);

protected:
	BOOL InsertListItem(CString ImgPath, int nItem, short LastTime);
};