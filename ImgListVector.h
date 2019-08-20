#pragma once
#include "stdafx.h"

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

_declspec(dllexport) ImgListVector *NewImgListVector();

_declspec(dllexport) void FreeImgListVector(ImgListVector *VectorPtr);