#pragma once

#include "N3UIStatic.h"

class CN3UITooltip: public CN3UIStatic
{
public:
	CN3UITooltip();
	virtual ~CN3UITooltip();

// Attributes
public:
protected:
	float			m_fHoverTime;
	bool			m_bSetText;
	POINT			m_ptCursor;

// Operations
public:
	void			SetText(const std::string& szText);
	virtual void	Release();
	virtual void	Tick();
	virtual void	Render();

	virtual uint32_t	MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
};
