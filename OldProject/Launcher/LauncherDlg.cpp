// LauncherDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "afxdialogex.h"

/////////////////////////////////////////////////////////////////////////////
// CTextView construction/destruction

CTextView::CTextView()
{
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CTextView::~CTextView()
{
}

IMPLEMENT_DYNAMIC(CTextView, CStatic)

BEGIN_MESSAGE_MAP(CTextView, CStatic)
	//{{AFX_MSG_MAP(CTextView)
	ON_WM_DRAWITEM_REFLECT()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextView drawing

BOOL CTextView::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	return afxGlobalData.DrawParentBackground(this, pDC, rect);
}

HBRUSH CTextView::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CTextView::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_OWNERDRAW);
}

void CTextView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	Gdiplus::Graphics grpx(lpDrawItemStruct->hDC);
	grpx.Flush(Gdiplus::FlushIntention::FlushIntentionSync);
	grpx.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
	grpx.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	Gdiplus::StringFormat gdiSF;
	gdiSF.SetAlignment(Gdiplus::StringAlignmentFar);
	gdiSF.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsNoFontFallback | Gdiplus::StringFormatFlagsNoClip);
	gdiSF.SetHotkeyPrefix(Gdiplus::HotkeyPrefixNone);
	gdiSF.SetTrimming(Gdiplus::StringTrimmingNone);

	int iDpi = GetDpiForSystem();
	Gdiplus::FontFamily fontFamily(L"Arial");
	//Gdiplus::RectF rectF(MulDiv(292, iDpi, 96), MulDiv(311, iDpi, 96), MulDiv(370, iDpi, 96), MulDiv(27, iDpi, 96));
	Gdiplus::RectF rectF(0, 0, MulDiv(370, iDpi, 96), MulDiv(27, iDpi, 96));
	const wchar_t* text;

	if (m_launcherState == LauncherState::Connecting) {
		text = L"Connecting to Login Server";
	}
	else {
		text = L"Login Server Connection failed! Please retry connecting!";
	}

	Gdiplus::GraphicsPath path;
	path.AddString(text, wcslen(text), &fontFamily, Gdiplus::FontStyleBold, 23, rectF, &gdiSF);
	Gdiplus::Pen pen(Gdiplus::Color(0, 0, 0), MulDiv(2, iDpi, 96));
	grpx.DrawPath(&pen, &path);
	Gdiplus::SolidBrush brush(Gdiplus::Color(255, 247, 222));
	grpx.FillPath(&brush, &path);
	grpx.Flush(Gdiplus::FlushIntention::FlushIntentionSync);
}


int CTextView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// side-step CView's implementation since we don't want to activate
	//  this view
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

CLauncherDlg::CLauncherDlg(CWnd* pParent /*=nullptr*/)
	: CBDialog(IDD_LAUNCHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CBDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START, m_bStart);
	DDX_Control(pDX, IDC_CLOSE, m_bClose);
	DDX_Control(pDX, IDC_WEBBROWSER, m_cBrowser);
	DDX_Control(pDX, IDC_HOMEPAGE, m_bHomePage);
	DDX_Control(pDX, IDC_OPTION, m_bOption);
	DDX_Control(pDX, IDC_PROGRESS, m_progressTiled);
	DDX_Control(pDX, IDC_INFO_TEXT, m_infoText);
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CBDialog)
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CLOSE, &CLauncherDlg::OnBnClickedClose)
END_MESSAGE_MAP()

LRESULT CLauncherDlg::OnNcHitTest(CPoint point)
{
	ScreenToClient(&point);

	CRect rc;
	GetClientRect(&rc);

	if (rc.PtInRect(point))
		return HTCAPTION;

	return CDialog::OnNcHitTest(point);
}

void CLauncherDlg::startNetworkMonitoring()
{
	// construct a REQ (request) socket and connect to interface
	m_zSocket = zmq::socket_t{ m_zContext, zmq::socket_type::req };
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	m_infoText.m_launcherState = LauncherState::Connecting;
	m_infoText.Invalidate();

	try
	{
		socketMonitor monitor;
		monitor.init(m_zSocket, "inproc://monitor");
		m_zSocket.connect("tcp://localhost:5555");
		while (monitor.eventID != ZMQ_EVENT_CONNECTED) {
			monitor.check_event(1);

			if (monitor.eventID == ZMQ_EVENT_CONNECTED) {
				TRACE("socket connected \n");
			}
			else {
				std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
				auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

				if (elapsedSeconds > 3) {
					m_infoText.m_launcherState = LauncherState::CannotConnect;
					m_infoText.Invalidate();
					break;
				}

				TRACE("socket not connected \n");
			}

			Sleep(250);
		}
		m_zSocketThread = std::jthread(&CLauncherDlg::startNetworking, this);
	}
	catch (const zmq::error_t& error)
	{
		int asd = 1;
		throw error;
	}
}

void CLauncherDlg::startNetworking()
{
	// set up some static data to send
	const std::string data{ "Hello" };

	for (auto request_num = 0; request_num < 10; ++request_num)
	{
		// send the request message
		TRACE("Sending Hello %d", request_num);
		//m_zSocket.send(zmq::buffer(data), zmq::send_flags::none);

		// wait for reply from server
		zmq::message_t reply{};

		try
		{
			//auto result = m_zSocket.recv(reply, zmq::recv_flags::none);
			
			TRACE("Received: %s", reply.to_string());

			std::cout << "Received " << reply.to_string();
			std::cout << " (" << request_num << ")";
			std::cout << std::endl;
		}
		catch (const zmq::error_t& ex)
		{
			std::cerr
				<< "[" << this << "] zmq::error_t(send_thread): "
				<< ex.what()
				<< std::endl;

			break;
		}
	}
}

BOOL CLauncherDlg::OnInitDialog()
{
	CBDialog::OnInitDialog();
	auto st = Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	assert(st == Gdiplus::Ok);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	m_zSocketMonitorThread = std::jthread(&CLauncherDlg::startNetworkMonitoring, this);
	auto startButton = (NButton*)GetDlgItem(IDC_START);
	auto homePageButton = (NButton*)GetDlgItem(IDC_HOMEPAGE);
	auto optionButton = (NButton*)GetDlgItem(IDC_OPTION);
	auto progress = (CProgressCtrlST*)GetDlgItem(IDC_PROGRESS);
	progress->SetBitmap(IDB_PROGRESS_TRACK);
	progress->SetRange(0, 100);
	progress->SetPos(0);
	m_bClose.m_normalResourceId = IDB_CLOSE_DEFAULT;
	m_bClose.m_hoverResourceId = IDB_CLOSE_HOVER;
	m_bClose.m_pressedResourceId = IDB_CLOSE_PRESSED;
	startButton->m_bDisabled = true;
	startButton->m_normalResourceId = IDB_START_DEFAULT;
	startButton->m_hoverResourceId = IDB_START_HOVER;
	startButton->m_disabledResourceId = IDB_START_DISABLED;
	startButton->m_pressedResourceId = IDB_START_PRESSED;
	startButton->name = "startButton";
	homePageButton->m_normalResourceId = IDB_HOMEPAGE_DEFAULT;
	homePageButton->m_hoverResourceId = IDB_HOMEPAGE_HOVER;
	homePageButton->m_pressedResourceId = IDB_HOMEPAGE_PRESSED;
	homePageButton->name = "homePageButton";
	optionButton->m_normalResourceId = IDB_OPTIONS_DEFAULT;
	optionButton->m_hoverResourceId = IDB_OPTIONS_HOVER;
	optionButton->m_pressedResourceId = IDB_OPTIONS_PRESSED;
	optionButton->name = "optionButton";
	long style = GetWindowLong(progress->m_hWnd, GWL_EXSTYLE);
	style &= ~WS_EX_STATICEDGE;
	SetWindowLong(progress->m_hWnd, GWL_EXSTYLE, style);
	int iDpi = GetDpiForSystem();
	::SetWindowPos(GetDlgItem(IDC_INFO_TEXT)->m_hWnd, HWND_TOP, MulDiv(292, iDpi, 96), MulDiv(311, iDpi, 96), MulDiv(370, iDpi, 96), MulDiv(18, iDpi, 96), NULL);
	::SetWindowPos(GetDlgItem(IDC_CLOSE)->m_hWnd, HWND_TOP, MulDiv(656, iDpi, 96), MulDiv(15, iDpi, 96), MulDiv(29, iDpi, 96), MulDiv(20, iDpi, 96), NULL);
	::SetWindowPos(GetDlgItem(IDC_WEBBROWSER)->m_hWnd, HWND_TOP, MulDiv(291, iDpi, 96), MulDiv(50, iDpi, 96), MulDiv(384, iDpi, 96), MulDiv(250, iDpi, 96), NULL);
	::SetWindowPos(startButton->m_hWnd, HWND_TOP, MulDiv(294, iDpi, 96), MulDiv(366, iDpi, 96), MulDiv(118, iDpi, 96), MulDiv(27, iDpi, 96), NULL);
	::SetWindowPos(homePageButton->m_hWnd, HWND_TOP, MulDiv(424, iDpi, 96), MulDiv(366, iDpi, 96), MulDiv(118, iDpi, 96), MulDiv(27, iDpi, 96), NULL);
	::SetWindowPos(optionButton->m_hWnd, HWND_TOP, MulDiv(554, iDpi, 96), MulDiv(366, iDpi, 96), MulDiv(118, iDpi, 96), MulDiv(27, iDpi, 96), NULL);
	::SetWindowPos(GetDlgItem(IDC_PROGRESS)->m_hWnd, HWND_TOP, MulDiv(296, iDpi, 96), MulDiv(333, iDpi, 96), MulDiv(374, iDpi, 96), MulDiv(16, iDpi, 96), NULL);
	m_cBrowser.Navigate(CString("http://board.nttgameonline.com/knight"), NULL, NULL, NULL, NULL);
	SetBitmap(IDB_BKG);
	SetBitmapStyle(StyleStretch);
	return TRUE;
}

int CLauncherDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	RECT rect;
	int iDpi = GetDpiForSystem();
	int dpiScaledWidth = MulDiv(700, iDpi, 96);
	int dpiScaledHeight = MulDiv(430, iDpi, 96);
	rect.left = (::GetSystemMetrics(SM_CYSCREEN) - dpiScaledWidth) / 2;
	rect.top = (::GetSystemMetrics(SM_CYSCREEN) - dpiScaledHeight) / 2;
	rect.right = rect.left + dpiScaledWidth;
	rect.bottom = rect.top + dpiScaledHeight;
	MoveWindow(&rect, TRUE);
	return 0;
}

void CLauncherDlg::OnPaint()
{
	CPaintDC dc(this);
	int hDCLast = dc.SaveDC();
	Gdiplus::Graphics grpx(dc.GetSafeHdc());
	grpx.SetTextRenderingHint(Gdiplus::TextRenderingHintClearTypeGridFit);
	grpx.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	Gdiplus::StringFormat gdiSF;
	gdiSF.SetAlignment(Gdiplus::StringAlignmentFar);
	gdiSF.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsNoFontFallback | Gdiplus::StringFormatFlagsNoClip);
	gdiSF.SetHotkeyPrefix(Gdiplus::HotkeyPrefixNone);
	gdiSF.SetTrimming(Gdiplus::StringTrimmingNone);

	int iDpi = GetDpiForSystem();
	Gdiplus::FontFamily fontFamily(L"Arial");
	Gdiplus::RectF rectF(MulDiv(292, iDpi, 96), MulDiv(311, iDpi, 96), MulDiv(370, iDpi, 96), MulDiv(27, iDpi, 96));
	const wchar_t* text;

	if (m_launcherState == LauncherState::Connecting) {
		text = L"Connecting to Login Server";
	}
	else {
		text = L"Login Server Connection failed! Please retry connecting!";
	}

	//Gdiplus::GraphicsPath path;
	//path.AddString(text, wcslen(text), &fontFamily, Gdiplus::FontStyleBold, 23, rectF, &gdiSF);
	//Gdiplus::Pen pen(Gdiplus::Color(0, 0, 0), MulDiv(2, iDpi, 96));
	//grpx.DrawPath(&pen, &path);
	//Gdiplus::SolidBrush brush(Gdiplus::Color(255, 247, 222));
	//grpx.FillPath(&brush, &path);
}

void CLauncherDlg::OnBnClickedClose()
{
	int linger = 0;
	m_zSocket.set(zmq::sockopt::linger, linger);
	m_zSocket.close();
	m_zContext.close();
	PostMessage(WM_CLOSE);
}
