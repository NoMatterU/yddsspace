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
	int x;
	int y;
	int ImgWidth;
	int ImgHeight;

	CFile m_file;
	BOOL IsOpen;

	CListCtrl *m_pListCtrl;
	CImageList *m_pImgList;
	ImgListVector *m_pImgVector;

public:
	CPJTReader();

	CPJTReader(CString FilePath, CListCtrl *pListCtrl, CImageList *pImgList, ImgListVector *pImgVector);

	~CPJTReader();
	// TODO:  �ڴ�������ķ�����

	BOOL InitDlgInfo(CEdit *pEdit1, CEdit *pEdit2, CEdit *pEdit3, CEdit *pEdit4);

	int InitListInfo(CString PjtName);

protected:
	BOOL InsertListItem(CString ImgPath, int nItem, CString LastTime);
};