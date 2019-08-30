// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 IMGLISTVECTOR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// IMGLISTVECTOR_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
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