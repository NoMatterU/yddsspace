// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� PJTWRITER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// PJTWRITER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef PJTWRITER_EXPORTS
#define PJTWRITER_API __declspec(dllexport)
#else
#define PJTWRITER_API __declspec(dllimport)
#endif

// �����Ǵ� PJTWriter.dll ������
class PJTWRITER_API CPJTWriter {
private:
	CListCtrl *m_pListCtrl;

	int x;
	int y;
	int ImgWidth;
	int ImgHeight;

	char ImgName[MAX_PATH];

public:
	CPJTWriter();

	CPJTWriter(CListCtrl *pListCtrl);

	~CPJTWriter();

	void EditToX(CEdit *pEdit);

	void EditToY(CEdit *pEdit);

	void EditToImgWidth(CEdit *pEdit);

	void EditToImgHeight(CEdit *pEdit);
	// ��д
public:
	BOOL ToPJTFile(CString FilePath);

private:
	void WCharToByte(CString str);

};

