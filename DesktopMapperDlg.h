
// MFCApplication1Dlg.h : header file
//

#pragma once
#include "MyListCtrl.h"


// CMFCApplication1Dlg dialog
class CMFCApplication1Dlg : public CDialogEx
{
private:
	CMyListCtrl *m_pListCtrl;

	CEdit *m_pEditX;
	CEdit *m_pEditY;
	CEdit *m_pEditH;
	CEdit *m_pEditW;
	CEdit *m_pGroup;

	CString m_PjtName;
	CString m_EditStr;

	// Construction
public:
	CMFCApplication1Dlg(CWnd* pParent = NULL);	// standard constructor

	void OnCancel();
	BOOL DestroyWindow();

	~CMFCApplication1Dlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

														// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CMyListCtrl *GetpListCtrl();

	CEdit *GetEditX();
	CEdit *GetEditY();
	CEdit *GetEditH();
	CEdit *GetEditW();

	CString GetPjtName();

	void SetPjtName(CString PjtName);
private:
	void InitList();

	BOOL SetTitleText();

	//	bool CStringToPchar(CString cstring, string &s);

	BOOL SetReslutionRate();

//	BOOL InsertListItem(CString ImgPath);
public:
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedInsert();
	afx_msg void OnBnClickedExit();

	afx_msg void OnEnSetfocusEdit3();
	afx_msg void OnEnKillfocusEdit3();
	afx_msg void OnEnSetfocusEdit4();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnEnSetfocusEdit5();
	afx_msg void OnEnKillfocusEdit5();
	afx_msg void OnEnSetfocusEdit6();
	afx_msg void OnEnKillfocusEdit6();
	afx_msg void OnBnClickedStart();
	afx_msg LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDelList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnStartCreate();
	afx_msg void OnStartOpen();
	afx_msg void OnStartSave();
};
