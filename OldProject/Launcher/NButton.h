#pragma once
#include "pch.h"
#include "resource.h"

// NButton
class NButton : public CButton
{
	// Construction
public:
	NButton();	// standard constructor

	// Implementation
	std::string name = "closeButton";
	bool m_bHover = false;
	bool m_bEnabledPressedState = true;
	bool m_bDisabled = false;

	int m_normalResourceId;
	int m_hoverResourceId;
	int m_pressedResourceId;
	int m_disabledResourceId;

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
