// LauncherDlg.h : header file
//

#pragma once

#include "NButton.h"
#include "CWEBBROWSER2.h"
#include "ProgressCtrlST.h"
#include <assert.h>
#include <gdiplus.h>
#include <stdio.h>
#include <atomic>
#include <zmq.hpp>
#include <iostream>
#include <thread>
#include "BDialog.h"

#pragma comment (lib, "gdiplus.lib")

enum class LauncherState { Connecting, Downloading, CannotConnect };

class socketMonitor : public zmq::monitor_t {
public:
    // listening for the on_event_connected event, notify user if successful. 
    void on_event_connected(const zmq_event_t& event, const char* addr) override {
        eventID = ZMQ_EVENT_CONNECTED;
        eventName = "Connected";
    }

    void on_event_disconnected(const zmq_event_t& event, const char* addr) override {
        eventID = ZMQ_EVENT_DISCONNECTED;
        eventName = "Disconnected";
    }

    void on_event_connect_retried(const zmq_event_t& event, const char* addr) override {
        eventID = ZMQ_EVENT_CONNECT_RETRIED;
        eventName = "Connection Retired";
    }

    void on_event_listening(const zmq_event_t& event, const char* addr) override {
        eventID = ZMQ_EVENT_LISTENING;
        eventName = "Listening";
    }

    void on_event_connect_delayed(const zmq_event_t& event, const char* addr) override {
        eventID = ZMQ_EVENT_CONNECT_DELAYED;
        eventName = "Connect Delayed";
    }

    void on_event_accept_failed(const zmq_event_t& event, const char* addr) override {
        eventID = ZMQ_EVENT_ACCEPT_FAILED;
        eventName = "Accept Failed";
    }

    void on_event_closed(const zmq_event_t& event, const char* addr) override {
        eventID = ZMQ_EVENT_CLOSED;
        eventName = "Closed";
    }

    void on_event_bind_failed(const zmq_event_t& event, const char* addr) override {
        eventID = ZMQ_EVENT_BIND_FAILED;
        eventName = "Bind Failed";
    }

    int eventID;
    std::string eventName;
};

class CTextView : public CStatic
{
    DECLARE_DYNAMIC(CTextView)
public:
    CTextView();
    virtual ~CTextView();

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    std::atomic<LauncherState> m_launcherState{ LauncherState::Connecting };
    std::atomic<int> m_currentDownloadingVersion;
protected:
    //{{AFX_MSG(CTextView)
    afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
    afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void PreSubclassWindow();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

class CLauncherDlg : public CBDialog
{
public:
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_LAUNCHER_DIALOG };
#endif

	CLauncherDlg(CWnd* pParent = nullptr);

    virtual void DoDataExchange(CDataExchange* pDX);
    afx_msg virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnBnClickedClose();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void startNetworking();
    void startNetworkMonitoring();

	NButton m_bStart;
	CProgressCtrlST m_progressTiled;
	NButton m_bClose;
	CWEBBROWSER2 m_cBrowser;
	NButton m_bHomePage;
	NButton m_bOption;

	zmq::context_t m_zContext{ 1 };
	zmq::socket_t m_zSocket;
	std::jthread m_zSocketThread{};
    std::jthread m_zSocketMonitorThread{};
    std::atomic<LauncherState> m_launcherState{LauncherState::Connecting};
    std::atomic<int> m_currentDownloadingVersion;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    HICON m_hIcon;

    DECLARE_MESSAGE_MAP()
    CTextView m_infoText;
};
