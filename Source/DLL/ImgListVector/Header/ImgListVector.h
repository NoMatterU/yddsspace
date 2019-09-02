// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� IMGLISTVECTOR_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// IMGLISTVECTOR_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef IMGLISTVECTOR_EXPORTS
#define IMGLISTVECTOR_API __declspec(dllexport)
#else
#define IMGLISTVECTOR_API __declspec(dllimport)
#endif

#define MAXLISTIMG 30

typedef struct ImgListInfo {
	CString ImgName;
	int ImgListIndex;
	int Count;
} Path2Index;

class ImgListVector {
public:
	ImgListVector() {};

	virtual ~ImgListVector() = 0 {};

	virtual int FindIndexByName(CString ImgName) = 0;

	virtual bool Add(CString ImgName, int ImgListIndex) = 0;

	virtual bool Delete(CString ImgName) = 0;

};

class ImgListVectorImp : public ImgListVector {

private:
	Path2Index m_List[MAXLISTIMG];
	int ListIndex;

public:
	ImgListVectorImp();
	~ImgListVectorImp();

	int FindIndexByName(CString ImgName);

	bool Add(CString ImgName, int ImgListIndex);

	bool Delete(CString ImgName);

};


IMGLISTVECTOR_API ImgListVector *NewImgListVector();

IMGLISTVECTOR_API void FreeImgListVector(ImgListVector *VectorPtr);