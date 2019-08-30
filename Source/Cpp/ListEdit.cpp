// ListEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DesktopMapper.h"
#include "ListEdit.h"


// CListEdit �Ի���

IMPLEMENT_DYNAMIC(CListEdit, CEdit)

CListEdit::CListEdit()
{
}

CListEdit::~CListEdit()
{
}



BEGIN_MESSAGE_MAP(CListEdit, CEdit)

	ON_MESSAGE(WM_KILLFOCUS, &CListEdit::OnEnKillFocus)
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CListEdit ��Ϣ�������


LRESULT CListEdit::OnEnKillFocus(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(GetParent()->GetSafeHwnd(), WM_SETFOCUS, 0, 0);

	return (LRESULT)0;
}




void CListEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch (nChar) {

	case VK_RETURN:
		::SendMessage(GetParent()->GetSafeHwnd(), WM_LOSEFOCUS, 0, 0);
		break;

	default:
		break;
	}

}
