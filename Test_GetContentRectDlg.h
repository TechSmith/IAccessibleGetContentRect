
// Test_GetContentRectDlg.h : header file
//

#pragma once


// CTest_GetContentRectDlg dialog
class CTest_GetContentRectDlg : public CDialogEx
{
// Construction
public:
	CTest_GetContentRectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TEST_GETCONTENTRECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
   static BOOL CALLBACK EnumDesktopWindowsProc( HWND hwnd, LPARAM lParam );
   CString GetAccessiblitity(HWND hwnd);

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedRefreshWindows();
   afx_msg void OnCbnSelectionChanged();
};
