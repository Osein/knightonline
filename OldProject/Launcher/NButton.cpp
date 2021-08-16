#include "pch.h"
#include <afxwin.h>
#include "NButton.h"

BEGIN_MESSAGE_MAP(NButton, CButton)
	ON_WM_MOUSEHOVER()
    ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

NButton::NButton(): CButton() {}

void NButton::OnMouseMove(UINT nFlags, CPoint point)
{

    //start tracking of Hover and Leave Event
    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(TRACKMOUSEEVENT);
    tme.dwFlags = TME_HOVER | TME_LEAVE;
    tme.hwndTrack = m_hWnd;
    tme.dwHoverTime = 1;
    TrackMouseEvent(&tme);

    CButton::OnMouseMove(nFlags, point);
}

void NButton::OnMouseHover(UINT nFlags, CPoint point)
{
    if (!m_bHover)
    {
        TRACE("OnMouseHover\n");
        m_bHover = true;
        Invalidate();
    }
	CButton::OnMouseHover(nFlags, point);
}

void NButton::OnMouseLeave()
{
    if (m_bHover)
    {
        TRACE("OnMouseLeave\n");
        m_bHover = false;
        Invalidate();
    }
	CButton::OnMouseLeave();
}

void NButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    int backgroundItem = m_bDisabled ? m_disabledResourceId : m_normalResourceId;

    if (!m_bDisabled) {
        switch (lpDrawItemStruct->itemAction)
        {
        case ODA_DRAWENTIRE:
            if (m_bHover)
            {
                backgroundItem = m_hoverResourceId;
            }
            else
            {
                backgroundItem = m_normalResourceId;
            }
            break;
        case ODA_SELECT:
            if (m_bEnabledPressedState)
            {
                if (lpDrawItemStruct->itemState & ODS_SELECTED)
                {
                    TRACE("backgroundItem assign pressed \n");
                    backgroundItem = m_pressedResourceId;
                }
                else
                {
                    backgroundItem = m_hoverResourceId;
                }
            }
            break;
        }
    }

    TRACE("backgroundItem: %d \n", backgroundItem);

    CBitmap buttonBG;
    buttonBG.LoadBitmap(backgroundItem);

    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);

    CDC dcMem;
    dcMem.CreateCompatibleDC(&dc);
    dcMem.SelectObject(buttonBG);

    dc.SetStretchBltMode(HALFTONE);

    int iDpi = GetDpiForSystem();

    if (name == "closeButton")
    {
        dc.StretchBlt(0, 0, MulDiv(29, iDpi, 96), MulDiv(24, iDpi, 96), &dcMem, 0, 0, 29, 24, SRCCOPY);
    }
    else
    {
        dc.StretchBlt(0, 0, MulDiv(119, iDpi, 96), MulDiv(28, iDpi, 96), &dcMem, 0, 0, 119, 28, SRCCOPY);
    }

    dc.Detach();
}
