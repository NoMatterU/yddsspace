// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PJTREADER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// PJTREADER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#include "ImgListVector.h"

#ifdef PJTREADER_EXPORTS
#define PJTREADER_API __declspec(dllexport)
#else
#define PJTREADER_API __declspec(dllimport)
#endif

// �����Ǵ� PJTReader.dll ������
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