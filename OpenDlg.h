#pragma once


// COpenDlg 对话框

class COpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenDlg)
private:

public:
	COpenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COpenDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void FindAllProject(CString strPath);
	afx_msg void OnBnClickedOk();
};
