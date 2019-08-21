#pragma once


// CCreateDlg 对话框

class CCreateDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateDlg)

public:
	CCreateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateDlg();

	BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
