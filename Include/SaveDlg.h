#pragma once


// CSaveDlg 对话框

class CSaveDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveDlg)

public:
	CSaveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSaveDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAVEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
