
// Test_GetContentRectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test_GetContentRect.h"
#include "Test_GetContentRectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTest_GetContentRectDlg dialog




CTest_GetContentRectDlg::CTest_GetContentRectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTest_GetContentRectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest_GetContentRectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTest_GetContentRectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
   ON_BN_CLICKED(IDC_REFRESH_WINDOWS, OnBnClickedRefreshWindows)
   ON_CBN_SELCHANGE(IDC_COMBO_WINDOWLIST, OnCbnSelectionChanged)
END_MESSAGE_MAP()


// CTest_GetContentRectDlg message handlers

BOOL CTest_GetContentRectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTest_GetContentRectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTest_GetContentRectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTest_GetContentRectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CALLBACK CTest_GetContentRectDlg::EnumDesktopWindowsProc( HWND hwnd, LPARAM lParam )
{
   CTest_GetContentRectDlg* pThis = (CTest_GetContentRectDlg*)lParam;

   if ( ::IsWindowVisible( hwnd ) && ::IsWindowEnabled( hwnd ) && !::IsIconic( hwnd ) && ( GetWindowLong( hwnd, GWL_EXSTYLE ) & WS_EX_TRANSPARENT ) == 0 )
   {
      TCHAR szClassName[256];

      if ( GetClassName( hwnd, szClassName, _countof(szClassName) - 1 ) != 0 )
      {
         CString strClassName(szClassName);
         CComboBox* pCombo = (CComboBox*)pThis->GetDlgItem(IDC_COMBO_WINDOWLIST);

         int nIndex = pCombo->AddString(strClassName);
         pCombo->SetItemData(nIndex, (DWORD_PTR)hwnd);
      }
   }

   return TRUE;
}

void CTest_GetContentRectDlg::OnBnClickedRefreshWindows()
{
   CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_WINDOWLIST);
   pCombo->ResetContent();

   ::EnumWindows( &EnumDesktopWindowsProc, (LPARAM)this );

   if( pCombo->GetCount() > 0 )
   {
      pCombo->SetCurSel(0);
      OnCbnSelectionChanged();
   }
}

CString NUM(int n)
{
   CString str;
   str.Format(_T("%d"), n);
   return str;
}

CString GetProcessModuleFileName( DWORD dwProcessId )
{
#define PATHSIZE 1024
   TCHAR szPath[PATHSIZE] = {0};

   if ( dwProcessId )
   {
      // Need to do this to work with UAC
      HANDLE hProcess = OpenProcess( PROCESS_QUERY_LIMITED_INFORMATION, TRUE, dwProcessId );
      if ( hProcess )
      {
         HMODULE hModule = LoadLibrary( _T("kernel32.dll" ) );

         typedef BOOL (WINAPI *LPQUERYFULLPROCESSIMAGENAME)( HANDLE, DWORD, LPTSTR, PDWORD );

         LPQUERYFULLPROCESSIMAGENAME pFuncQueryFullProcessImageName;
#ifdef UNICODE
         pFuncQueryFullProcessImageName = (LPQUERYFULLPROCESSIMAGENAME)GetProcAddress( hModule, "QueryFullProcessImageNameW" );      
#else
         pFuncQueryFullProcessImageName = (LPQUERYFULLPROCESSIMAGENAME)GetProcAddress( hModule, "QueryFullProcessImageNameA" );
#endif

         DWORD dwLen = PATHSIZE;
         if ( pFuncQueryFullProcessImageName )
         {
            pFuncQueryFullProcessImageName( hProcess, 0, (LPTSTR)szPath, &dwLen );
         }

         ::FreeLibrary( hModule );

         ::CloseHandle( hProcess );
      }

   }

   return szPath;
}

HWND GetFrameWindow( HWND hWnd, BOOL bTopLevel /* = FALSE */ )
{
   LONG  WndStyle;
   HWND  hFrameWnd   = hWnd;
   HWND  hNextWnd    = hFrameWnd;
   HWND  hDeskTopWnd = ::GetDesktopWindow();

   // Find the nearest "frame window" for the window we want to pop up.
   while ( hNextWnd != NULL && hNextWnd != hDeskTopWnd )
   {
      hFrameWnd = hNextWnd;
      if ( ! bTopLevel )
      {
         WndStyle = ::GetWindowLong( hNextWnd, GWL_STYLE );
         if ( (WndStyle & WS_VISIBLE) && !(WndStyle & WS_DISABLED) )
         {
            if ( (WndStyle & WS_SYSMENU)|| (WndStyle & WS_CAPTION) == WS_CAPTION )
            {
               break;
            }
         }
      }
      hNextWnd = ::GetParent( hNextWnd );
   }

   return( hFrameWnd );
}

HWND GetTopLevelFromWindow( HWND hWnd )
{
   return GetAncestor( hWnd, GA_ROOT );
}

BOOL GetObjectPosition( IAccessible *pObject, CRect &rect, long lID = CHILDID_SELF )
{
   rect.SetRectEmpty();
   if ( pObject == NULL )
   {
      ASSERT( FALSE );
      return FALSE;
   }

   RECT    rectPos;
   VARIANT varObjectID;

   varObjectID.vt   = VT_I4;
   varObjectID.lVal = lID;

   HRESULT hr = pObject->accLocation( &rectPos.left, &rectPos.top, &rectPos.right, &rectPos.bottom, varObjectID ); 
   if ( SUCCEEDED(hr) )
   {
      //accLocation returns the height and width, not bottom and right coords.
      rectPos.right  += rectPos.left;
      rectPos.bottom += rectPos.top;   
      rect = CRect( rectPos );

      return TRUE;
   }

   return FALSE;
}

CString CTest_GetContentRectDlg::GetAccessiblitity(HWND hwnd)
{
   CString strRet;
   if( hwnd == NULL )
   {
      return _T("Window's handle was NULL; so not accessible");
   }

   CComPtr<IAccessible> pAccessible;
   if ( SUCCEEDED( AccessibleObjectFromWindow( hwnd , (DWORD)OBJID_CLIENT, IID_IAccessible, (void**)&pAccessible ) ) )
   {
      strRet += _T("Able to get OBJID_CLIENT for Window\r\n");
   }
   else
   {
      return _T("Unable to get OBJID_CLIENT for Window; so not accessible");
   }

   CRect rect;
   if( GetObjectPosition(pAccessible, rect) )
   {
      CString strRect;
      strRet.Format(_T("Got rect: Left: %d, Top: %d, Right: %d, Bottom: %d\r\n"), rect.left, rect.top, rect.right, rect.bottom);
      strRet += strRect;
   }
   else
   {
      strRet += _T("Unable to get rect\r\n");
   }

   return strRet;
}

void CTest_GetContentRectDlg::OnCbnSelectionChanged()
{
   CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_WINDOWLIST);
   int nIndex = pCombo->GetCurSel();

   SetDlgItemText(IDC_EDIT_OUTPUT, CString());

   if( nIndex == CB_ERR )
      return;

   HWND hwnd = (HWND)pCombo->GetItemData(nIndex);

   CString strOutput;

   {
      //Process ID:
      DWORD dwProcessId = 0;
      ::GetWindowThreadProcessId( hwnd, &dwProcessId );

      CString strProcessID;
      strProcessID.Format(_T("Process ID:\t%d"), dwProcessId);
      strOutput += strProcessID + _T("\r\n");

      //ProcessModule Filename
      CString strProcessFilename;
      strProcessFilename.Format(_T("Process filename:\t%s"), GetProcessModuleFileName(dwProcessId));
      strOutput += strProcessFilename + _T("\r\n");
   }

   //GetFrameWindow
   strOutput += _T("GetFrameWindow:\r\n");
   strOutput += _T("---------------------\r\n");
   strOutput += GetAccessiblitity(GetFrameWindow(hwnd, TRUE)) + _T("\r\n");

   //GetParent
   strOutput += _T("GetTopLevelFromWindow:\r\n");
   strOutput += _T("---------------------\r\n");
   strOutput += GetAccessiblitity(::GetTopLevelFromWindow(hwnd)) + _T("\r\n");

   SetDlgItemText(IDC_EDIT_OUTPUT, strOutput);
}
