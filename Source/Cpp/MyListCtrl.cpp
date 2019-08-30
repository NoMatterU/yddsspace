// MyListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DesktopMapper.h"
#include "MyListCtrl.h"

// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{
	m_pEdit = NULL;
	m_Item = 0;
	m_Subitem = 0;
	m_ListIndex = 0;
	m_pImgList = new CImageList();
	m_pImgVector = NewImgListVector();
}

CMyListCtrl::~CMyListCtrl()
{
	m_pImgList->DeleteImageList();

	delete m_pEdit;
	m_pEdit = NULL;

	delete m_pImgList;
	m_pImgList = NULL;

	FreeImgListVector(m_pImgVector);
}

void CMyListCtrl::SetpImgList(CImageList * pImgList)
{
	this->m_pImgList = pImgList;
}

CImageList * CMyListCtrl::GetpImgList()
{
	return this->m_pImgList;
}

void CMyListCtrl::SetpImgVector(ImgListVector * pImgVector)
{
	this->m_pImgVector = pImgVector;
}

ImgListVector * CMyListCtrl::GetpImgVector()
{
	return this->m_pImgVector;
}

void CMyListCtrl::SetListCount(int ListIndex)
{
	this->m_ListIndex = ListIndex;
}


BOOL CMyListCtrl::AddListItem(CString ImgPath)
{
	CString str;
	HBITMAP hBmp = NULL;
	Bitmap bmp(ImgPath);

	if (m_ListIndex >= MAXLISTITEM) return FALSE;

	str.Format(TEXT("%d bp X %d bp"), bmp.GetHeight(), bmp.GetWidth());

	Bitmap* pThumbnail = (Bitmap*)bmp.GetThumbnailImage(50, 50, NULL, NULL); //设定缩略图的大小
	pThumbnail->GetHBITMAP(Color(255, 255, 255), &hBmp);
	CBitmap *pImage = CBitmap::FromHandle(hBmp);         //转换成CBitmap格式位图
														 //	if (!Cbmp.Attach((HBITMAP)LoadImageW(AfxGetInstanceHandle(), str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE))) return;
	if (hBmp == NULL) return FALSE;

	int num = m_pImgVector->FindIndexByName(ImgPath);
	if(num <= -1) num = m_pImgList->Add(pImage, RGB(255, 255, 255));

	this->InsertItem(m_ListIndex, TEXT(""), num);

	this->SetItemText(m_ListIndex, 3, str);

	str = ImgPath.Right(ImgPath.GetLength() - ImgPath.ReverseFind('\\') - 1);
	this->SetItemText(m_ListIndex, 2, str);

	m_pImgVector->Add(str, num);

	str.Format(TEXT("%d"), m_ListIndex + 1);
	this->SetItemText(m_ListIndex, 1, str);

	this->SetItemText(m_ListIndex, 4, TEXT("0 ms"));

	m_ListIndex++;

	return TRUE;
}

BOOL CMyListCtrl::DeleteListItem(int ListIndex, CString ImgName)
{
	this->DeleteItem(ListIndex);
	if (m_ListIndex > 0) m_ListIndex--;
	if (m_pImgVector->Delete(ImgName)) return TRUE;
	return FALSE;
}

BOOL CMyListCtrl::Create(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT uID)
{
	CListCtrl::Create(dwStyle, rect, pParentWnd, uID);
	this->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	m_pImgList->Create(50, 50, ILC_COLOR32 | ILC_MASK, 10, 2);
	this->SetImageList(m_pImgList, LVSIL_SMALL | LVSIL_NORMAL);
	return 0;
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)

	ON_NOTIFY_REFLECT(NM_SETFOCUS, &CMyListCtrl::OnNMSetfocusList)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMyListCtrl::OnNMDblclkList)
	ON_MESSAGE(WM_LOSEFOCUS, &CMyListCtrl::EditReturnFocus)

	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CMyListCtrl 消息处理程序


void CMyListCtrl::OnNMSetfocusList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (m_pEdit != NULL) {
		CString str;
		m_pEdit->GetWindowTextW(str);

		if (m_Subitem == 4) {
			if (str.IsEmpty()) str = "0 ms";
			else str += " ms";
			this->SetItemText(m_Item, m_Subitem, str);
		}
		else if (m_Subitem == 1) {
			if (str.IsEmpty()) {
				str.Format(TEXT("%d"), m_Item + 1);
				this->SetItemText(m_Item, m_Subitem, str);
				goto end;
			}

			int index = _ttoi(str) - 1;
			if (index >= 0 && index < this->GetItemCount()) {
				this->ChangeItemPos(m_Item, index);
			}
		}

end:
		delete m_pEdit;
		m_pEdit = NULL;
		m_Item = -1;
		m_Subitem = -1;
	}
}



void CMyListCtrl::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int TopIndex = this->GetTopIndex();
	NM_LISTVIEW *pNMLISTVIEW = (NM_LISTVIEW *)pNMHDR;
	m_Item = pNMLISTVIEW->iItem;
	m_Subitem = pNMLISTVIEW->iSubItem;

	if (m_Item != -1) {
		if (m_Subitem == 4 || m_Subitem == 1) {
			int x = 0, y = 30, i = 0;

			for (i = 0; i < m_Item - TopIndex; i++) y += 51;

			for(i = 0; i < m_Subitem; i++) x += GetColumnWidth(i);

			m_EditRect.left = x;
			m_EditRect.top = y;
			m_EditRect.right = x + GetColumnWidth(m_Subitem);
			m_EditRect.bottom = y + 40;

			if (m_pEdit != NULL) {
				m_pEdit->MoveWindow(m_EditRect);
				m_pEdit->Invalidate();
				return;
			}
			else {

				m_pEdit = new CListEdit;
				m_pEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_MULTILINE | ES_NUMBER,
					m_EditRect, this, IDC_LISTEDIT);

				//设置编辑框输入字符长度
				if (m_Subitem == 1) m_pEdit->SetLimitText(2);
				else if (m_Subitem == 4) m_pEdit->SetLimitText(4);
			//	CString str = GetItemText(m_Item, m_Subitem);
			//	m_pEdit->SetWindowTextW(str);
				m_pEdit->SetFocus();

				CRect rect;
				m_pEdit->GetClientRect(rect);
				OffsetRect(&rect, 0, 8);
				m_pEdit->SendMessage(EM_SETRECT, 0, (LPARAM)&rect);
			}
		}
		else {
			m_Item = -1;
			m_Subitem = -1;
		}
	}

}



void CMyListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);
	this->ShowScrollBar(SB_HORZ, FALSE);
	// TODO: 在此处添加消息处理程序代码
}


void CMyListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	LRESULT Result;

	int TopIndex = GetTopIndex();

/*	
	CEdit *ptr = (CEdit *)ChildWindowFromPoint(CPoint(m_EditRect.left + 1, m_EditRect.top + 1));

	if (nSBCode == SB_ENDSCROLL) {
		if (ptr) ptr->ShowCaret();
		CRect rect;
		m_pEdit->GetClientRect(rect);
		OffsetRect(&rect, 0, 8);
		m_pEdit->SendMessage(EM_SETRECT, 0, (LPARAM)&rect);
//		this->GetParent()->Invalidate();
	}
//	else if(ptr) ptr->HideCaret();
*/


	if (m_pEdit != NULL) {

		int y = 30;
		for (int i = 0; i < m_Item - TopIndex; i++) y += 51;
		m_EditRect.top = y;
		m_EditRect.bottom = y + 40;

		if (TopIndex > m_Item) {
			m_pEdit->MoveWindow(LISTCTRLWIDTH, LISTCTRLHEIGHT, m_EditRect.Width(), m_EditRect.Height());
			m_pEdit->Invalidate();
		}
		else if (TopIndex + 5 <= m_Item) {
			m_pEdit->MoveWindow(LISTCTRLWIDTH, LISTCTRLHEIGHT, m_EditRect.Width(), m_EditRect.Height());
			m_pEdit->Invalidate();
		}
		else {
			m_pEdit->MoveWindow(m_EditRect.left, m_EditRect.top, m_EditRect.Width(), m_EditRect.Height());
			m_pEdit->Invalidate();
		}
	}

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}



void CMyListCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	POSITION pos = NULL;

	switch (nChar) {
	case VK_BACK:
	case VK_DELETE:
		pos = this->GetFirstSelectedItemPosition();
		if (pos == NULL) break;
		if (IDCANCEL == MessageBox(L"是否要删除选中列?", L"提示", MB_OKCANCEL | MB_ICONWARNING)) break;

		while (pos)
		{
			int nIndex = this->GetNextSelectedItem(pos);
			::SendMessage(GetParent()->GetSafeHwnd(), WM_DELLISTITEM, nIndex, 0);
			pos = this->GetFirstSelectedItemPosition(); //这步很重要，不然删除不完全
		}

		for (int i = 0; i < this->GetItemCount(); i++) {
			CString str;
			str.Format(L"%d", i + 1);
			SetItemText(i, 1, str);
		}

		break;
	default:
		break;
	}

//	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}


LRESULT CMyListCtrl::EditReturnFocus(WPARAM wParam, LPARAM lParam)
{
	Sleep(5);
	this->SetFocus();
	return LRESULT();
}


BOOL CMyListCtrl::ChangeItemPos(int PrePos, int LaterPos)
{
	if (PrePos == LaterPos) return TRUE;

	LVITEMW item;
	item.mask = LVIF_TEXT | LVIF_IMAGE;
	item.iItem = PrePos;
	item.iSubItem = 0;
	item.pszText = NULL;
	this->GetItem(&item);

	this->InsertItem(LaterPos, L"", item.iImage);

	CString str;

	for (int i = 1; i <= 4; i++) {

		if(LaterPos < PrePos) str = this->GetItemText(PrePos + 1, i);
		else str = this->GetItemText(PrePos, i);

		SetItemText(LaterPos, i, str);
	}

	if (LaterPos < PrePos) this->DeleteItem(PrePos + 1);
	else this->DeleteItem(PrePos);

	//重新设置Item的排列序号
	for (int i = 0; i < this->GetItemCount(); i++) {
		str.Format(L"%d", i + 1);
		SetItemText(i, 1, str);
	}

	return TRUE;
}


BOOL CMyListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int TopIndex = GetTopIndex();

	if (m_pEdit != NULL) {

		int y = 30;
		for (int i = 0; i < m_Item - TopIndex; i++) y += 51;
		m_EditRect.top = y;
		m_EditRect.bottom = y + 40;

		if (TopIndex > m_Item) {
			m_pEdit->MoveWindow(LISTCTRLWIDTH, LISTCTRLHEIGHT, m_EditRect.Width(), m_EditRect.Height());
			m_pEdit->Invalidate();
		}
		else if (TopIndex + 5 <= m_Item) {
			m_pEdit->MoveWindow(LISTCTRLWIDTH, LISTCTRLHEIGHT, m_EditRect.Width(), m_EditRect.Height());
			m_pEdit->Invalidate();
		}
		else {
			m_pEdit->MoveWindow(m_EditRect.left, m_EditRect.top, m_EditRect.Width(), m_EditRect.Height());
			m_pEdit->Invalidate();
		}

	}

	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}
