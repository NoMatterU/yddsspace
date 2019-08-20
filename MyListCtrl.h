#pragma once
#include "ListEdit.h"
#include "ImgListVector.h"

#pragma comment(lib, "Lib/ImgListVector.lib")
// CMyListCtrl

class CMyListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CMyListCtrl)

public:
	CMyListCtrl();
	virtual ~CMyListCtrl();

	void SetpImgList(CImageList *pImgList);
	CImageList *GetpImgList();

	void SetpImgVector(ImgListVector *pImgVector);
	ImgListVector *GetpImgVector();

	void SetListCount(int ListIndex);

	BOOL AddListItem(CString ImgPath);

	BOOL DeleteListItem(int ListIndex, CString ImgName);

	BOOL Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT uID);
protected:
	DECLARE_MESSAGE_MAP()

private:
	CListEdit *m_pEdit;
	CImageList *m_pImgList;
	ImgListVector *m_pImgVector;

	int m_ListIndex;

	int m_Item;
	int m_Subitem;

	CRect m_EditRect;

public:
	afx_msg void OnNMSetfocusList(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT EditReturnFocus(WPARAM wParam, LPARAM lParam);

protected:
	BOOL ChangeItemPos(int PrePos, int LaterPos);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

