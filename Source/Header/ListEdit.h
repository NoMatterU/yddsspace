#pragma once


// CListEdit 对话框

class CListEdit : public CEdit
{
	DECLARE_DYNAMIC(CListEdit)

public:
	CListEdit();   // 标准构造函数
	virtual ~CListEdit();

protected:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnEnKillFocus(WPARAM wParam, LPARAM lParam);

	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
