#pragma once
#include "stdafx.h"

class CPJTWriter {
private:
	CListCtrl *m_pListCtrl;

	int x;
	int y;
	int ImgWidth;
	int ImgHeight;

public:
	CPJTWriter();

	CPJTWriter(CListCtrl *pListCtrl);

	~CPJTWriter();

	void GetX(CEdit *pEdit);

	void GetY(CEdit *pEdit);

	void GetImgWidth(CEdit *pEdit);

	void GetImgHeight(CEdit *pEdit);
	// ÖØÐ´
public:
	BOOL ToPJTFile(CString FilePath);
};
