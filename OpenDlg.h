#pragma once


// COpenDlg �Ի���

class COpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COpenDlg)
private:

public:
	COpenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COpenDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	void FindAllProject(CString strPath);
	afx_msg void OnBnClickedOk();
};
